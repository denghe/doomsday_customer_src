#pragma once

namespace xx {

	template <typename T, typename D>
	struct Grid2d {
		struct Node {
			int32_t next, prev;		// nodes index
			int32_t bucketsIndex;	// used by. -1 mean not use
			D cache;				// pos, radius ... cache
			T value;				// pointer?
		};

		int32_t rowCount{}, columnCount{};
	//private:
		int32_t freeList{ -1 }, freeCount{}, count{}, itemsCapacity{};
		Node* nodes{};
		int32_t* buckets{};	// 2d array space. nodes index

		Grid2d() = default;
		Grid2d(Grid2d const& o) = delete;
		Grid2d& operator=(Grid2d const& o) = delete;

		XX_INLINE void Init(int32_t rowCount_, int32_t columnCount_, int32_t capacity_ = 0) {
			assert(!nodes && !buckets && rowCount_ > 0 && columnCount_ > 0 && capacity_ >= 0);
			rowCount = rowCount_;
			columnCount = columnCount_;
			itemsCapacity = capacity_;
			freeList = -1;
			freeCount = count = 0;
			if (capacity_ > 0) {
				nodes = (Node*)new MyAlignedStorage<Node>[capacity_];
			}
			auto bucketsLen = rowCount_ * columnCount_;
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
			assert(rowNumber_ >= 0 && rowNumber_ < rowCount);
			assert(columnNumber_ >= 0 && columnNumber_ < columnCount);

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
			assert(rowNumber_ >= 0 && rowNumber_ < rowCount);
			assert(columnNumber_ >= 0 && columnNumber_ < columnCount);
			return rowNumber_ * columnCount + columnNumber_;
		}

	};

}

namespace Game {

	struct Test3;
	struct alignas(8) Test3Item {
		Test3* scene{};
		xx::XY pos{};
		float radius{};
		float radians{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Test3Item() {};
	};

	// zuma algorithm snake ( body can hit, when body dead, shrink to tail )

	struct Pathway : xx::MovePathCache {
		// todo: move target funcs?
	};

	enum class SnakeElementTypes : int32_t {
		Unknown, Head, Body, Tail
	};

	struct Snake;
	struct SnakeElement : Test3Item {
		static constexpr float cSpeed{ 1.1f };
		static constexpr float cStickRatio{ 10.f };
		static constexpr float cRadiusAnimStep{ 0.01f };
		static constexpr float cDrawScale{ 1.f };
		static constexpr float cNodeDistanceRatio{ 0.6f };
		static constexpr float cFaceAnimSpeed{ 10.f };
		static constexpr xx::FromTo<float> cRadiusRange{ 16.f, 17.f };

		// ********************************** fill by PreInit() **********************************
		Snake* owner{};
		SnakeElementTypes elementType{};
		int32_t index{ -1 };		// owner->elements[index] == this
		// ********************************** fill by PreInit() **********************************

		int32_t pathwayCursor{ -1 };
		int32_t snakeElementsIndex{ -1 };

		void PreInit(Snake* owner_, SnakeElementTypes elementType_);
		void Init(float radius_);
		void Remove();				// remove from owner and sync all index
		bool HitCheck(XY p);
		SnakeElement* GetPrev();	// -> tail
		SnakeElement* GetNext();	// -> head
		int32_t BaseUpdate();
	};


	struct PosRadius {
		XY pos{};
		float radius{};
	};

	struct Grid2dSnakeElement : xx::Grid2d<SnakeElement*, PosRadius> {
		using Base = xx::Grid2d<SnakeElement*, PosRadius>;
		using Base::Base;
		XYi cellSize{};
		XY _1_cellSize{};

		void Init(XYi cellSize_, int32_t rowCount_, int32_t columnCount_, int32_t capacity_ = 0) {
			assert(cellSize_.x > 0 && cellSize_.y > 0);
			Base::Init(rowCount_, columnCount_, capacity_);
			cellSize = cellSize_;
			_1_cellSize.x = 1.f / cellSize_.x;
			_1_cellSize.y = 1.f / cellSize_.y;
		}

		void Add(SnakeElement* e) {
			assert(e->snakeElementsIndex == -1);
			auto nodeIndex = Base::Add(int32_t(e->pos.y * _1_cellSize.y)
				, int32_t(e->pos.x * _1_cellSize.x), e);
			auto& o = this->nodes[nodeIndex];
			o.cache.pos = e->pos;
			o.cache.radius = e->radius;
			e->snakeElementsIndex = nodeIndex;
		}

		void Update(SnakeElement* e) {
			assert(Base::nodes[e->snakeElementsIndex].value == e);
			auto& o = Base::nodes[e->snakeElementsIndex];
			o.cache.pos = e->pos;
			o.cache.radius = e->radius;
			Base::Update(e->snakeElementsIndex
				, int32_t(e->pos.y * _1_cellSize.y), int32_t(e->pos.x * _1_cellSize.x));
		}

		void Remove(SnakeElement* e) {
			assert(Base::nodes[e->snakeElementsIndex].value == e);
			Base::Remove(e->snakeElementsIndex);
			e->snakeElementsIndex = -1;
		}

		XX_INLINE int32_t ToBucketsIndex(XY p) const {
			return Base::ToBucketsIndex(int32_t(p.y * _1_cellSize.y), int32_t(p.x * _1_cellSize.x));
		}
	};


	struct Snake : Test3Item {
		Test3* scene{};
		Pathway* pathway{};
		xx::Listi32<xx::Shared<SnakeElement>> elements;	// [0].elementType == Tail
		template<typename HeadType, typename BodyType, typename TailType>
		void Init(Test3* scene_, Pathway* pathway_, int32_t bodyLen_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct SnakeHead : SnakeElement {
		Shaker faceShaker;
		void Init();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct SnakeTail : SnakeElement {
		void Init();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct SnakeBody : SnakeElement {
		int32_t U1_n{};
		void U1_RadiusAnim();

		void Init();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct Test3 : Scene {
		xx::Shared<xx::Node> ui;

		Camera camera;
		xx::Listi32<xx::Shared<Pathway>> pathways;
		xx::Listi32<xx::Shared<Snake>> snakes;
		Grid2dSnakeElement grid;

		void MakeUI();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
