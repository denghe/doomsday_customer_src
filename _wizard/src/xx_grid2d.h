#pragma once
#include "xx_space_.h"

namespace xx {

	template <typename T>
	struct Grid2dNode {
		int32_t next, prev;		// nodes index
		int32_t bucketsIndex;	// used by. -1 mean not use
		T value;				// pointer?
	};

	template <typename T, typename C>
	struct Grid2dNodeEx : Grid2dNode<T> {
		int32_t next, prev;		// nodes index
		int32_t bucketsIndex;	// used by. -1 mean not use
		C cache;
		T value;				// pointer?
	};

	template <typename T, typename C = void>
	struct Grid2d {
		using Node = std::conditional_t<std::is_void_v<C>, Grid2dNode<T>, Grid2dNodeEx<T, C>>;

		int32_t numRows{}, numCols{};
		int32_t freeList{ -1 }, freeCount{}, count{}, itemsCapacity{};
		Node* nodes{};
		int32_t* buckets{};	// 2d array space. nodes index

		Grid2d() = default;
		Grid2d(Grid2d const& o) = delete;
		Grid2d& operator=(Grid2d const& o) = delete;

		XX_INLINE void Init(int32_t numRows_, int32_t numCols_, int32_t capacity_ = 0) {
			assert(!nodes && !buckets && numRows_ > 0 && numCols_ > 0 && capacity_ >= 0);
			numRows = numRows_;
			numCols = numCols_;
			itemsCapacity = capacity_;
			freeList = -1;
			freeCount = count = 0;
			if (capacity_ > 0) {
				nodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			}
			auto bucketsLen = numRows_ * numCols_;
			buckets = new int32_t[bucketsLen];
			memset(buckets, -1, bucketsLen * sizeof(int32_t));
		}

		~Grid2d() {
			if (!buckets) return;
			Clear();
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = {};
			delete[]buckets;
			buckets = {};
		}

		void Clear() {
			assert(buckets);
			if (!count) return;
			for (int32_t i = 0; i < count; ++i) {
				auto& o = nodes[i];
				if (o.bucketsIndex != -1) {
					buckets[o.bucketsIndex] = -1;
					o.bucketsIndex = -1;
					o.value.~T();
				}
			}
			freeList = -1;
			freeCount = 0;
			count = 0;
		}

		void Reserve(int32_t capacity_) {
			assert(buckets && capacity_ > 0);
			if (capacity_ <= itemsCapacity) return;
			auto newItems = (Node*)new MyAlignedStorage<Node>[capacity_];
			if constexpr (IsPod_v<T>) {
				::memcpy((void*)newItems, (void*)nodes, count * sizeof(Node));
			}
			else {
				for (int32_t i = 0; i < count; ++i) {
					new (&newItems[i].value) T((T&&)nodes[i].value);
					nodes[i].value.T::~T();
				}
			}
			delete[](MyAlignedStorage<Node>*)nodes;
			nodes = newItems;
			itemsCapacity = capacity_;
		}

		template<typename V>
		XX_INLINE int32_t Add(int32_t rowNumber_, int32_t columnNumber_, V&& v_) {
			assert(buckets);
			assert(rowNumber_ >= 0 && rowNumber_ < numRows);
			assert(columnNumber_ >= 0 && columnNumber_ < numCols);

			// alloc
			int32_t nodeIndex;
			if (freeCount > 0) {
				nodeIndex = freeList;
				freeList = nodes[nodeIndex].next;
				freeCount--;
			}
			else {
				if (count == itemsCapacity) {
					Reserve(count ? count * 2 : 16);
				}
				nodeIndex = count;
				count++;
			}
			auto& o = nodes[nodeIndex];

			// calc
			auto bucketsIndex = ToBucketsIndex(rowNumber_, columnNumber_);

			// link
			o.next = buckets[bucketsIndex];
			if (buckets[bucketsIndex] >= 0) {
				nodes[buckets[bucketsIndex]].prev = nodeIndex;
			}
			buckets[bucketsIndex] = nodeIndex;
			o.prev = -1;
			o.bucketsIndex = bucketsIndex;

			// assign
			new (&o.value) T(std::forward<V>(v_));
			return nodeIndex;
		}

		XX_INLINE void Remove(int32_t nodeIndex_) {
			assert(buckets);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].bucketsIndex != -1);
			auto& o = nodes[nodeIndex_];

			// unlink
			if (o.prev < 0) {
				buckets[o.bucketsIndex] = o.next;
			}
			else {
				nodes[o.prev].next = o.next;
			}
			if (o.next >= 0) {
				nodes[o.next].prev = o.prev;
			}

			// free
			o.next = freeList;
			o.prev = -1;
			o.bucketsIndex = -1;
			freeList = nodeIndex_;
			freeCount++;

			// cleanup
			o.value.~T();
		}

		XX_INLINE void Update(int32_t nodeIndex_, int32_t newRowNumber_, int32_t newColumnNumber_) {
			assert(buckets);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count && nodes[nodeIndex_].bucketsIndex != -1);

			// calc
			auto newBucketsIndex = ToBucketsIndex(newRowNumber_, newColumnNumber_);
			auto& o = nodes[nodeIndex_];

			// no change check
			if (o.bucketsIndex == newBucketsIndex) return;

			// unlink
			if (o.prev < 0) {
				buckets[o.bucketsIndex] = o.next;
			}
			else {
				nodes[o.prev].next = o.next;
			}
			if (o.next >= 0) {
				nodes[o.next].prev = o.prev;
			}

			// link
			o.next = buckets[newBucketsIndex];
			if (buckets[newBucketsIndex] >= 0) {
				nodes[buckets[newBucketsIndex]].prev = nodeIndex_;
			}
			buckets[newBucketsIndex] = nodeIndex_;
			o.prev = -1;
			o.bucketsIndex = newBucketsIndex;
		}

		XX_INLINE Node& NodeAt(int32_t nodeIndex_) const {
			assert(buckets);
			assert(nodeIndex_ >= 0 && nodeIndex_ < count);
			assert(nodes[nodeIndex_].bucketsIndex != -1);
			return (Node&)nodes[nodeIndex_];
		}

		XX_INLINE int32_t Count() const {
			return count - freeCount;
		}

		XX_INLINE bool Empty() const {
			return Count() == 0;
		}

		XX_INLINE int32_t ToBucketsIndex(int32_t rowNumber_, int32_t columnNumber_) const {
			assert(rowNumber_ >= 0 && rowNumber_ < numRows);
			assert(columnNumber_ >= 0 && columnNumber_ < numCols);
			return rowNumber_ * numCols + columnNumber_;
		}


		// foreach target cell + round 8 = 9 buckets
		// .Foreach9All([](decltype(grid)::Node& o)->void {  all  });
		// .Foreach9All([](decltype(grid)::Node& o)->bool {  return false == break  });
		template <bool enableExcept = false, typename F, typename R = std::invoke_result_t<F, Node&>>
		void Foreach9All(int32_t rowNumber_, int32_t columnNumber_, F&& func, T const& except = {}) {
			if (columnNumber_ < 0 || columnNumber_ >= numCols) return;
			if (rowNumber_ < 0 || rowNumber_ >= numRows) return;

			// 5
			auto bi = rowNumber_ * numCols + columnNumber_;
			auto ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (enableExcept) {
					if (n.value == except) {
						ni = nex;
						continue;
					}
				}
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 6
			++columnNumber_;
			if (columnNumber_ >= numCols) return;
			++bi;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 3
			++rowNumber_;
			if (rowNumber_ >= numRows) return;
			bi += numCols;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 2
			--bi;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 1
			columnNumber_ -= 2;
			if (columnNumber_ < 0) return;
			--bi;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 4
			bi -= numCols;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 7
			rowNumber_ -= 2;
			if (rowNumber_ < 0) return;
			bi -= numCols;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 8
			++bi;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}

			// 9
			++bi;
			ni = buckets[bi];
			while (ni != -1) {
				auto& n = nodes[ni];
				auto nex = n.next;
				if constexpr (std::is_void_v<R>) {
					func(n);
				}
				else {
					if (func(n)) return;
				}
				ni = nex;
			}
		}

		// ring diffuse foreach ( usually for update logic or range search )
		// .ForeachByRange([](decltype(grid)::Node& o)->void {  all  });
		// .ForeachByRange([](decltype(grid)::Node& o)->bool {  return false == break  });
		template <typename F, typename R = std::invoke_result_t<F, Node&>>
		void ForeachByRange(xx::SpaceGridRingDiffuseData const& rdd, int32_t rowNumber_, int32_t columnNumber_, int32_t searchRange, F&& func) {
			if (columnNumber_ < 0 || columnNumber_ >= numCols) return;
			if (rowNumber_ < 0 || rowNumber_ >= numRows) return;

			// todo: func add bool parameter: current range( max )

			auto& lens = rdd.lens;
			auto& idxs = rdd.idxs;
			for (int32_t i = 1, e = lens.len; i < e; i++) {
				auto offsets = lens[i - 1].count;
				auto size = lens[i].count - lens[i - 1].count;
				for (int32_t j = 0; j < size; ++j) {
					auto& tmp = idxs[offsets + j];
					auto cIdx = columnNumber_ + tmp.x;
					if (cIdx < 0 || cIdx >= numCols) continue;
					auto rIdx = rowNumber_ + tmp.y;
					if (rIdx < 0 || rIdx >= numRows) continue;
					auto bucketsIndex = rIdx * numCols + cIdx;

					auto ni = buckets[bucketsIndex];
					while (ni != -1) {
						auto& n = nodes[ni];
						auto nex = n.next;
						if constexpr (std::is_void_v<R>) {
							func(n);
						}
						else {
							if (func(n)) return;
						}
						ni = nex;
					}
				}
				if (lens[i].radius > searchRange) break;
			}
		}

		// todo: more search

	};



	struct Grid2dPosRadius {
		XY pos{};
		float radius{};
	};

	template<typename T, bool enableCache = false>
	struct Grid2dCircle : xx::Grid2d<T, std::conditional_t<enableCache, Grid2dPosRadius, void>> {
		using Base = xx::Grid2d<T, std::conditional_t<enableCache, Grid2dPosRadius, void>>;
		using Base::Base;
		XYi cellSize{};
		XY _1_cellSize{};

		void Init(XYi cellSize_, int32_t numRows_, int32_t numCols_, int32_t capacity_ = 0) {
			assert(cellSize_.x > 0 && cellSize_.y > 0);
			Base::Init(numRows_, numCols_, capacity_);
			cellSize = cellSize_;
			_1_cellSize.x = 1.f / cellSize_.x;
			_1_cellSize.y = 1.f / cellSize_.y;
		}

		template<typename V>
		void Add(int32_t& nodeIndex_, V&& e) {
			assert(nodeIndex_ == -1);
			auto cri = PosToCRIndex(e->pos);
			nodeIndex_ = Base::Add(cri.y, cri.x, std::forward<V>(e));
			auto& o = this->nodes[nodeIndex_];
			if constexpr (enableCache) {
				o.cache.pos = e->pos;
				o.cache.radius = e->radius;
			}
		}

		void Update(int32_t nodeIndex_, T const& e) {
			assert(Base::nodes[nodeIndex_].value == e);
			auto& o = Base::nodes[nodeIndex_];
			if constexpr (enableCache) {
				o.cache.pos = e->pos;
				o.cache.radius = e->radius;
			}
			auto cri = PosToCRIndex(e->pos);
			Base::Update(nodeIndex_, cri.y, cri.x);
		}

		void Remove(int32_t nodeIndex_, T const& e) {
			assert(Base::nodes[nodeIndex_].value == e);
			Base::Remove(nodeIndex_);
			nodeIndex_ = -1;
		}

		XX_INLINE XYi PosToCRIndex(XY p) const {
			return p * _1_cellSize;
		}

		XX_INLINE int32_t ToBucketsIndex(XY p) const {
			return Base::ToBucketsIndex(int32_t(p.y * _1_cellSize.y), int32_t(p.x * _1_cellSize.x));
		}

		XX_INLINE int32_t NodeIndexAt(XY p) const {
			return Base::buckets[ToBucketsIndex(p)];
		}
	};

}
