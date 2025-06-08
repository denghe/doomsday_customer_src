#pragma once
#include "xx_space_.h"

namespace Game {

	// space index with size limited items
	// requires:
	/*
		XY pos{};
		???????????????????
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Base *prev{}, *next{};		// if enableDoubleLink == true
	*/
	template<typename T, bool enableDoubleLink>
	struct SpaceIndex {
		XYi gridSize{};								// gridSize = cellSize * numCR
		int32_t cellSize{};							// diameter
		float _1_cellSize{};						// 1 / cellSize
		int32_t numRows{}, numCols{};
		xx::Listi32<xx::Shared<T>> items;
		int32_t cellsLen{};							// numRows * numCols;
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
					cells[c->indexAtCells] = (T*)c->next;
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
				cells[c->indexAtCells] = (T*)c->next;
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

		XX_INLINE XYi PosToColRowIndex(XYi pos) const {
			auto cri = pos / cellSize;
			assert(!(cri.x < 0 || cri.x >= numCols || cri.y < 0 || cri.y >= numRows));
			return cri;
		}

		XX_INLINE XYi PosToColRowIndex(XY pos) const {
			auto cri = (pos * _1_cellSize).As<int32_t>();
			assert(!(cri.x < 0 || cri.x >= numCols || cri.y < 0 || cri.y >= numRows));
			return cri;
		}

		XX_INLINE int32_t ColRowIndexToCellIndex(XYi cri) const {
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
		XX_INLINE T* TryAt(XYi cri) const {
			if (cri.x < 0 || cri.x >= numCols) return nullptr;
			if (cri.y < 0 || cri.y >= numRows) return nullptr;
			auto ci = cri.y * numCols + cri.x;
			assert(ci >= 0 && ci < cellsLen);
			return cells[ci];
		}

		XX_INLINE T* At(XYi cri) const {
			assert(cri.x >= 0 && cri.x < numCols);
			assert(cri.y >= 0 && cri.y < numRows);
			auto ci = cri.y * numCols + cri.x;
			assert(ci >= 0 && ci < cellsLen);
			return cells[ci];
		}

		void Clear() {
			items.Clear();
			memset(cells.get(), 0, sizeof(T*) * cellsLen);
			memset(counts.get(), 0, sizeof(int32_t) * cellsLen);
		}


		// foreach target cell + round 8 = 9 cells
		// .Foreach9All([](T& o)->void {  all  });
		// .Foreach9All([](T& o)->bool {  return false == break  });
		template <bool enableExcept = false, typename F, typename R = std::invoke_result_t<F, T*>>
		void Foreach9All(XYi pos, F&& func, T* except = {}) {
			auto cIdx = pos.x / cellSize;
			if (cIdx < 0 || cIdx >= numCols) return;
			auto rIdx = pos.y / cellSize;
			if (rIdx < 0 || rIdx >= numRows) return;

			// 5
			auto idx = rIdx * numCols + cIdx;
			auto c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (enableExcept) {
					if (c == except) {
						c = nex;
						continue;
					}
				}
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 6
			++cIdx;
			if (cIdx >= numCols) return;
			++idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 3
			++rIdx;
			if (rIdx >= numRows) return;
			idx += numCols;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 2
			--idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 1
			cIdx -= 2;
			if (cIdx < 0) return;
			--idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 4
			idx -= numCols;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 7
			rIdx -= 2;
			if (rIdx < 0) return;
			idx -= numCols;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 8
			++idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}

			// 9
			++idx;
			c = cells[idx];
			while (c) {
				auto nex = c->next;
				if constexpr (std::is_void_v<R>) {
					func(c);
				}
				else {
					if (func(c)) return;
				}
				c = nex;
			}
		}


		// ring diffuse foreach ( usually for update logic or range search )
		// .ForeachByRange([](T* o)->void {  all  });
		// .ForeachByRange([](T* o)->bool {  break  });
		template <bool enableExcept = false, typename F, typename R = std::invoke_result_t<F, T*>>
		void ForeachByRange(xx::SpaceGridRingDiffuseData const& d, XYi pos, int32_t maxDistance, F&& func, T* except = {}) {
			auto cIdxBase = pos.x / cellSize;
			if (cIdxBase < 0 || cIdxBase >= numCols) return;
			auto rIdxBase = pos.y / cellSize;
			if (rIdxBase < 0 || rIdxBase >= numRows) return;
			auto searchRange = maxDistance + cellSize;

			auto& lens = d.lens;
			auto& idxs = d.idxs;
			for (int32_t i = 1, e = lens.len; i < e; i++) {
				auto offsets = lens[i - 1].count;
				auto size = lens[i].count - lens[i - 1].count;
				for (int32_t j = 0; j < size; ++j) {
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
						if constexpr (std::is_void_v<R>) {
							func(c);
						}
						else {
							if (func(c)) return;
						}
						c = nex;
					}
				}
				if (lens[i].radius > searchRange) break;
			}
		}

	};

}
