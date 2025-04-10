#pragma once
#include "xx_space_.h"

namespace Game {

	// space index with size limited items
	// requires:
	/*
		XY pos{};
		float radius{};
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		T *prev{}, *next{};		// if enableDoubleLink == true
	*/
	template<typename T, bool enableDoubleLink = true>
	struct Space {
		XYi gridSize{};								// gridSize = cellSize * numCR
		int32_t cellSize{};
		float _1_cellSize{};						// 1 / cellSize
		int32_t numRows{}, numCols{};
		xx::Listi32<xx::Shared<T>> items;
		int32_t cellsLen{};
		std::unique_ptr<T*[]> cells;
		std::unique_ptr<int32_t[]> counts;
		xx::SpaceGridRingDiffuseData const* rdd{};	// ref to requires data

		void Init(xx::SpaceGridRingDiffuseData const* rdd_, int32_t numRows_, int32_t numCols_, int32_t cellSize_, int32_t cap = 10000) {
			assert(!cells);
			assert(numCols_ > 0 && numRows_ > 0 && cellSize_ > 0);
			rdd = rdd_;
			numRows = numRows_;
			numCols = numCols_;
			cellSize = cellSize_;
			gridSize = { cellSize_ * numCols_, cellSize_ * numRows_ };
			_1_cellSize = 1.f / cellSize;

			cellsLen = numRows * numCols;
			cells = std::make_unique_for_overwrite<T*[]>(cellsLen);
			memset(cells.get(), 0, sizeof(T*) * cellsLen);
			counts = std::make_unique_for_overwrite<int32_t[]>(cellsLen);
			memset(counts.get(), 0, sizeof(int32_t) * cellsLen);

			items.Reserve(cap);
		}

		template<typename U>
		xx::Shared<U>& Add(xx::Shared<U> c) requires std::is_base_of_v<T, U> {
			assert(c && c->indexAtItems == -1 && c->indexAtCells == -1);
			auto cri = PosToColRowIndex(c->pos);
			auto ci = ColRowIndexToCellIndex(cri);

			if constexpr (enableDoubleLink) {
				// link
				assert(!c->prev && !c->next);
				assert(!cells[ci] || !cells[ci]->prev);
				if (cells[ci]) {
					cells[ci]->prev = c;
				}
				c->next = cells[ci];
				c->indexAtCells = ci;
				cells[ci] = c;
				assert(!cells[ci]->prev);
				assert(c->next != c);
				assert(c->prev != c);
			} else {
				assert(!cells[ci]);
				c->indexAtCells = ci;
				cells[ci] = c;
			}
			++counts[ci];

			// store
			c->indexAtItems = items.len;
			return (xx::Shared<U>&)items.Emplace(std::move(c));
		}

		void Remove(T* c) {
			assert(c);
			assert(c->indexAtItems != -1 && c->indexAtCells != -1);

			if constexpr (enableDoubleLink) {
				assert(c->next != c && c->prev != c);
				assert(!c->prev && cells[c->indexAtCells] == c || c->prev->next == c && cells[c->indexAtCells] != c);
				assert(!c->next || c->next->prev == c);

				// unlink
				if (c->prev) {	// isn't header
					assert(cells[c->indexAtCells] != c);
					c->prev->next = c->next;
					if (c->next) {
						c->next->prev = c->prev;
						c->next = {};
					}
					c->prev = {};
				} else {
					assert(cells[c->indexAtCells] == c);
					cells[c->indexAtCells] = c->next;
					if (c->next) {
						c->next->prev = {};
						c->next = {};
					}
				}
				assert(cells[c->indexAtCells] != c);
			} else {
				assert(cells[c->indexAtCells] == c);
				cells[c->indexAtCells] = nullptr;
			}
			--counts[c->indexAtCells];

			// clear
			auto ii = c->indexAtItems;
			items.Back()->indexAtItems = ii;
			items.SwapRemoveAt(ii);
		}

		void Update(T* c) requires enableDoubleLink {
			assert(c);
			assert(c->indexAtItems != -1 && c->indexAtCells != -1);
			assert(c->next != c && c->prev != c);
			assert(!c->prev && cells[c->indexAtCells] == c || c->prev->next == c && cells[c->indexAtCells] != c);
			assert(!c->next || c->next->prev == c);

			auto cri = PosToColRowIndex(c->pos);
			auto ci = ColRowIndexToCellIndex(cri);
			if (ci == c->indexAtCells) return;	// no change
			assert(!cells[ci] || !cells[ci]->prev);

			// unlink
			if (c->prev) {	// isn't header
				assert(cells[c->indexAtCells] != c);
				c->prev->next = c->next;
				if (c->next) {
					c->next->prev = c->prev;
				}
			} else {
				assert(cells[c->indexAtCells] == c);
				cells[c->indexAtCells] = c->next;
				if (c->next) {
					c->next->prev = {};
				}
			}
			--counts[c->indexAtCells];
			assert(cells[c->indexAtCells] != c);
			assert(ci != c->indexAtCells);

			// link
			if (cells[ci]) {
				cells[ci]->prev = c;
			}
			c->prev = {};
			c->next = cells[ci];
			cells[ci] = c;
			c->indexAtCells = ci;
			++counts[ci];
			assert(!cells[ci]->prev);
			assert(c->next != c);
			assert(c->prev != c);
		}

		XX_INLINE XYi PosToColRowIndex(XYi const& pos) const {
			auto cri = pos / cellSize;
			assert(!(cri.x < 0 || cri.x >= numCols || cri.y < 0 || cri.y >= numRows));
			return cri;
		}
		XX_INLINE XYi PosToColRowIndex(XY const& pos) const {
			auto cri = (pos * _1_cellSize).As<int32_t>();
			assert(!(cri.x < 0 || cri.x >= numCols || cri.y < 0 || cri.y >= numRows));
			return cri;
		}

		XX_INLINE int32_t ColRowIndexToCellIndex(XYi const& cri) const {
			auto ci = cri.y * numCols + cri.x;
			assert(ci >= 0 && ci < cellsLen);
			return ci;
		}

		XX_INLINE XYi CellIndexToColRowIndex(int32_t ci) const {
			assert(ci >= 0 && ci < cellsLen);
			auto ri = ci / numCols;
			return { ci - ri * numCols, ri };
		}

		// out of range: return nullptr
		XX_INLINE T* TryAt(XYi const& cri) const {
			if (cri.x < 0 || cri.x >= numCols) return nullptr;
			if (cri.y < 0 || cri.y >= numRows) return nullptr;
			auto ci = cri.y * numCols + cri.x;
			assert(ci >= 0 && ci < cellsLen);
			return cells[ci];
		}

		XX_INLINE T* At(XYi const& cri) const {
			assert(cri.x >= 0 && cri.x < numCols);
			assert(cri.y >= 0 && cri.y < numRows);
			auto ci = cri.y * numCols + cri.x;
			assert(ci >= 0 && ci < cellsLen);
			return cells[ci];
		}

		// todo: more search funcs

		// ring diffuse search   nearest edge   best one and return
		template<bool enableExcept = false>
		T* FindNearestByRange(float x, float y, float maxDistance, T* except = {}) {
			int32_t cIdxBase = (int32_t)(x * _1_cellSize);
			if (cIdxBase < 0 || cIdxBase >= numCols) return nullptr;
			int32_t rIdxBase = (int32_t)(y * _1_cellSize);
			if (rIdxBase < 0 || rIdxBase >= numRows) return nullptr;
			auto searchRange = maxDistance + cellSize;

			T* rtv{};
			float maxV{};

			auto& lens = rdd->lens;
			auto& idxs = rdd->idxs;
			for (int32_t i = 1, e = lens.len; i < e; i++) {
				auto offsets = lens[i - 1].count;
				auto siz = lens[i].count - lens[i - 1].count;
				for (int32_t j = 0; j < siz; ++j) {
					auto& tmp = idxs[offsets + j];
					auto cIdx = cIdxBase + tmp.x;
					if (cIdx < 0 || cIdx >= numCols) continue;
					auto rIdx = rIdxBase + tmp.y;
					if (rIdx < 0 || rIdx >= numRows) continue;
					auto cidx = rIdx * numCols + cIdx;

					auto c = cells[cidx];
					while (c) {
						auto nex = c->next;
						if constexpr (enableExcept) {
							if (c == except) {
								c = nex;
								continue;
							}
						}

						auto vx = c->pos.x - x;
						auto vy = c->pos.y - y;
						auto dd = vx * vx + vy * vy;
						auto r = maxDistance + c->radius;
						auto v = r * r - dd;
						if (v > maxV) {
							rtv = c;
							maxV = v;
						}

						c = nex;
					}
				}
				if (lens[i].radius > searchRange) break;
			}

			return rtv;
		}

		// foreach target cell + round 8 = 9 cells find first cross and return ( tested )
		template<bool enableExcept = false>
		T* FindFirstCrossBy9(float x, float y, float radius, T* except = {}) {
			int cIdx = (int)(x * _1_cellSize);
			if (cIdx < 0 || cIdx >= numCols) return nullptr;
			int rIdx = (int)(y * _1_cellSize);
			if (rIdx < 0 || rIdx >= numRows) return nullptr;

			// 5
			int idx = rIdx * numCols + cIdx;
			auto c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 6
			++cIdx;
			if (cIdx >= numCols) return nullptr;
			++idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 3
			++rIdx;
			if (rIdx >= numRows) return nullptr;
			idx += numCols;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 2
			--idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 1
			cIdx -= 2;
			if (cIdx < 0) return nullptr;
			--idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 4
			idx -= numCols;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 7
			rIdx -= 2;
			if (rIdx < 0) return nullptr;
			idx -= numCols;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 8
			++idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			// 9
			++idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;

				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				auto vx = c->pos.x - x;
				auto vy = c->pos.y - y;
				auto r = c->radius + radius;
				if (vx * vx + vy * vy < r * r) return c;

				c = nex;
			}

			return nullptr;
		}


	};

}
