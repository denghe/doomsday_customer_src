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

		void Clear() {
			items.Clear();
			memset(cells.get(), 0, sizeof(T*) * cellsLen);
			memset(counts.get(), 0, sizeof(int32_t) * cellsLen);
		}

	};

}
