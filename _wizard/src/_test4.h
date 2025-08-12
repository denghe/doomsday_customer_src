#pragma once


namespace xx {

	//template <typename T>
	struct Phys2dFixedCircle {

		struct Node {
			void* ud;				// ref to user struct
			int32_t next;			// for memory manage
			float radius;			//
			XY pos, inc, acc;		// for phys
		};

		struct Bucket {
			int32_t len;
			std::array<int32_t, 4> nodeIndexs;
		};

		int32_t numRows{}, numCols{}, numCells{};
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};
		float cellDiamiter{};
		std::unique_ptr<Node[]> nodes;
		std::unique_ptr<Bucket[]> buckets;

		void Init(int32_t numRows_, int32_t numCols_, float cellDiamiter_, int32_t capacity_ = 0) {
			assert(!nodes && !buckets && numRows_ > 0 && numCols_ > 0 && capacity_ >= 0);
			numRows = numRows_;
			numCols = numCols_;
			numCells = numRows_ * numCols_;
			capacity = capacity_;
			freeHead = -1;
			freeCount = count = 0;
			cellDiamiter = cellDiamiter_;
			if (capacity_) {
				nodes = std::make_unique_for_overwrite<Node[]>(capacity_);
			}
			buckets = std::make_unique_for_overwrite<Bucket[]>(numCells);
			ClearBuckets();
		}

		void Reserve(int32_t capacity_) {
			assert(buckets && capacity_ > 0);
			if (capacity_ <= capacity) return;
			auto newNodes = std::make_unique_for_overwrite<Node[]>(capacity_);
			::memcpy((void*)newNodes.get(), (void*)nodes.get(), count * sizeof(Node));
			nodes = std::move(newNodes);
			capacity = capacity_;
		}

		XX_INLINE Node& Alloc() {
			int32_t nodeIndex;
			if (freeCount > 0) {
				nodeIndex = freeHead;
				freeHead = nodes[nodeIndex].next;
				freeCount--;
			}
			else {
				if (count == capacity) {
					Reserve(count ? count * 2 : 16);
				}
				nodeIndex = count;
				count++;
			}
			return nodes[nodeIndex];
		}
		// todo: Free

		XX_INLINE int32_t Add(void* ud_, float radius_, XY pos_, XY inc_, XY acc_) {
			assert(buckets);
			assert((int32_t)pos_.y >= 0 && (int32_t)pos_.y < numRows);
			assert((int32_t)pos_.x >= 0 && (int32_t)pos_.x < numCols);
			auto& o = Alloc();
			o.ud = ud_;
			o.next = -1;
			o.radius = radius_;
			o.pos = pos_;
			o.inc = inc_;
			o.acc = acc_;
		}

		XX_INLINE void ClearBuckets() {
			assert(buckets);
			for (int32_t i = 0; i < numCells; ++i) {
				buckets[i].len = 0;
			}
		}

		XX_INLINE void FillBuckets() {
			for (int32_t i = 0; i < count; ++i) {
				if (auto& o = nodes[i]; o.ud) {
					auto p = o.pos.As<int32_t>();
					assert(p.x >= 0 && p.x < numCols && p.y >= 0 && p.y < numRows);
					auto bi = p.x * numRows + p.y;
					auto& b = buckets[bi];
					b.nodeIndexs[b.len++] = i;
					assert(b.len <= 4);
				}
			}
		}

		XX_INLINE void Calc(Node& n1, Node& n2) {
			static constexpr float response_coef{ 1.0f };
			static constexpr float eps{ 0.0001f };
			auto d = n1.pos - n2.pos;
			auto m2 = d.x * d.x + d.y * d.y;
			if (m2 > 1.f || m2 < eps) return;	// todo: use radius
			auto m = std::sqrtf(m2);
			auto a = response_coef * 0.5f * (1.0f - m);
			auto v = d / m * a;
			n1.pos += v;
			n2.pos -= v;
		}

#if 0
		XX_INLINE void Calc(int32_t ni1, Bucket& b) {
			for (int32_t j = 0; j < b.len; ++j) {
				auto ni2 = b.nodeIndexs[j];
				Calc(nodes[ni1], nodes[ni2]);
			}
		}

		XX_INLINE void Calc() {
			for (int32_t bi = 0; bi < numCells; ++bi) {
				auto& b = buckets[bi];
				for (int32_t j = 0; j < b.len; ++j) {
					auto ni = b.nodeIndexs[j];
					Calc(ni, buckets[bi - 1]);
					Calc(ni, buckets[bi]);
					Calc(ni, buckets[bi + 1]);
					Calc(ni, buckets[bi + numRows - 1]);
					Calc(ni, buckets[bi + numRows]);
					Calc(ni, buckets[bi + numRows + 1]);
					Calc(ni, buckets[bi - numRows - 1]);
					Calc(ni, buckets[bi - numRows]);
					Calc(ni, buckets[bi - numRows + 1]);
				}
			}
		}
#else
		XX_INLINE void Calc(Bucket& b1, Bucket& b2) {
			for (int32_t ni1 = 0; ni1 < b1.len; ++ni1) {
				for (int32_t ni2 = 0; ni2 < b2.len; ++ni2) {
					Calc(nodes[b1.nodeIndexs[ni1]], nodes[b2.nodeIndexs[ni2]]);
				}
			}
		}

		XX_INLINE void Calc() {
			for (int32_t bi = 0; bi < numCells; ++bi) {
				auto& b = buckets[bi];
				if (!b.len) continue;
				Calc(b, buckets[bi - 1]);
				Calc(b, buckets[bi]);
				Calc(b, buckets[bi + 1]);
				Calc(b, buckets[bi + numRows - 1]);
				Calc(b, buckets[bi + numRows]);
				Calc(b, buckets[bi + numRows + 1]);
				Calc(b, buckets[bi - numRows - 1]);
				Calc(b, buckets[bi - numRows]);
				Calc(b, buckets[bi - numRows + 1]);
			}
		}

		XX_INLINE void UpdateCore() {
			// todo

			//for (uint32_t i{ start }; i < end; ++i) {
			//	PhysicObject& obj = objects.data[i];
			//	// Add gravity
			//	obj.acceleration += gravity;
			//	// Apply Verlet integration
			// 
			/*
			const Vec2 last_update_move = position - last_position;

			const float VELOCITY_DAMPING = 40.0f; // arbitrary, approximating air friction

			const Vec2 new_position = position + last_update_move + (acceleration - last_update_move * VELOCITY_DAMPING) * (dt * dt);
			last_position = position;
			position = new_position;
			acceleration = { 0.0f, 0.0f };
			*/
			// 
			//	// Apply map borders collisions
			//	const float margin = 2.0f;
			//	if (obj.position.x > world_size.x - margin) {
			//		obj.position.x = world_size.x - margin;
			//	}
			//	else if (obj.position.x < margin) {
			//		obj.position.x = margin;
			//	}
			//	if (obj.position.y > world_size.y - margin) {
			//		obj.position.y = world_size.y - margin;
			//	}
			//	else if (obj.position.y < margin) {
			//		obj.position.y = margin;
			//	}
			//}
		}
#endif

		void Update() {
			ClearBuckets();
			FillBuckets();
			Calc();
			UpdateCore();
		}

	};

}


namespace Game {

	struct Test4;
	struct alignas(8) Test4Item {
		Test4* scene{};
		xx::XY pos{};
		float radius{};
		float radians{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Test4Item() {};
	};

	// phys ( fixed circles ) simulate


	struct Test4 : Scene {
		xx::Shared<xx::Node> ui;


		void MakeUI();

		void Init() override;
		void Update_();
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
