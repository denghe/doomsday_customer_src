#pragma once


namespace xx {

	//template <typename T>
	struct BoidsEnv {
		static constexpr float cPerceptionRadius{ 3 };
		static constexpr float cMaxSpeed{ 4.f * Cfg::frameDelay };
		static constexpr float cMaxForce{ 0.2f * Cfg::frameDelay };

		struct Node {
			int32_t next;
			int32_t indexAtDatas;
			void* ud;
		};

		struct Data {
			int32_t indexAtNodes;
			xx::RGBA8 color;
			//float rotation;
			XY pos, velocity, acc;
		};
		    
		struct Bucket {
			int32_t len;
			std::array<int32_t, 3> indexAtDatass;
		};

		int32_t numRows{}, numCols{}, bucketsLen{};					// for buckets
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};	// for nodes
		int32_t datasLen{};											// for datas

		static constexpr float cellSize{ 1 };
		XY pixelSize{};
		xx::SpaceGridRingDiffuseData* rdd{};

		std::unique_ptr<Node[]> nodes;
		std::unique_ptr<Data[]> datas;
		std::unique_ptr<Bucket[]> buckets;

		void Init(xx::SpaceGridRingDiffuseData* rdd_, int32_t numRows_, int32_t numCols_, int32_t capacity_ = 0) {
			assert(rdd_);
			assert(!nodes && !buckets && numRows_ > 0 && numCols_ > 0 && capacity_ >= 0);
			rdd = rdd_;
			numRows = numRows_;
			numCols = numCols_;
			bucketsLen = numRows_ * numCols_;
			capacity = capacity_;
			freeHead = -1;
			freeCount = count = 0;
			pixelSize = { numCols_, numRows_ };
			if (capacity_) {
				nodes = std::make_unique_for_overwrite<Node[]>(capacity_);
				datas = std::make_unique_for_overwrite<Data[]>(capacity_);
			}
			buckets = std::make_unique_for_overwrite<Bucket[]>(bucketsLen);
			ClearBuckets();
		}

		void Reserve(int32_t capacity_) {
			assert(buckets && capacity_ > 0);
			if (capacity_ <= capacity) return;
			capacity = capacity_;

			auto newNodes = std::make_unique_for_overwrite<Node[]>(capacity_);
			::memcpy((void*)newNodes.get(), (void*)nodes.get(), count * sizeof(Node));
			nodes = std::move(newNodes);

			auto newDatas = std::make_unique_for_overwrite<Data[]>(capacity_);
			::memcpy((void*)newDatas.get(), (void*)datas.get(), datasLen * sizeof(Data));
			datas = std::move(newDatas);
		}

		int32_t Add(void* ud_, XY pos_, XY velocity_, XY acc_/*, float rotation_*/, xx::RGBA8 color_ = xx::RGBA8_White) {
			assert(buckets);
			assert((int32_t)pos_.y >= 0 && (int32_t)pos_.y < numRows);
			assert((int32_t)pos_.x >= 0 && (int32_t)pos_.x < numCols);

			int32_t ni;
			if (freeCount > 0) {
				ni = freeHead;
				freeHead = nodes[ni].next;
				freeCount--;
			}
			else {
				if (count == capacity) {
					Reserve(count ? count * 2 : 16);
				}
				ni = count;
				count++;
			}

			auto& n = nodes[ni];
			n.next = -1;
			n.indexAtDatas = datasLen;
			n.ud = ud_;

			auto& d = datas[datasLen++];
			d.indexAtNodes = ni;
			d.color = color_;
			d.pos = pos_;
			d.velocity = velocity_;
			d.acc = acc_;
			//d.rotation = rotation_;

			return ni;
		}

		void Remove(int32_t indexAtNodes_) {
			assert(buckets);
			assert(indexAtNodes_ >= 0 && indexAtNodes_ < count);
			assert(nodes[indexAtNodes_].ud);

			auto& n = nodes[indexAtNodes_];
			auto& ld = datas[datasLen - 1];

			nodes[ld.indexAtNodes].indexAtDatas = n.indexAtDatas;	// redirect
			::memcpy((void*)&datas[n.indexAtDatas], (void*)&ld, sizeof(Data));
			datasLen--;

			n.next = freeHead;
			n.indexAtDatas = -1;
			n.ud = {};
			freeHead = indexAtNodes_;
			freeCount++;
		}


		// todo: ring diffuse foreach with angle range limit
		template <typename F, typename R = std::invoke_result_t<F, Data&, float>>
		void ForeachByRange(int32_t rowNumber_, int32_t columnNumber_, float searchRange, F&& func) {
			auto scale = cellSize / rdd->cellSize;
			for (int32_t i = 1, e = rdd->lens.len; i < e; i++) {
				auto offsets = rdd->lens[i - 1].count;
				auto size = rdd->lens[i].count - rdd->lens[i - 1].count;
				auto range = rdd->lens[i].radius * scale;
				for (int32_t j = 0; j < size; ++j) {
					auto& tmp = rdd->idxs[offsets + j];
					auto rn = rowNumber_ + tmp.y;
					auto cn = columnNumber_ + tmp.x;
					if (rn < 0 || rn >= numRows || cn < 0 || cn >= numCols) continue;
					auto bi = cn * numRows + rn;
					auto& b = buckets[bi];
					for (int32_t i = 0; i < b.len; ++i) {
						auto di = b.indexAtDatass[i];
						auto& d = datas[di];
						if (func(d, range)) return;
					}
				}
				if (range > searchRange) return;
			}
			assert(false);	// not enough rdd data?
		}


		XX_INLINE void ClearBuckets() {
			assert(buckets);
			for (int32_t i = 0; i < bucketsLen; ++i) {
				buckets[i].len = 0;
			}
		}

		XX_INLINE void FillBuckets() {
			for (int32_t di = 0; di < datasLen; ++di) {
				auto p = datas[di].pos.As<int32_t>();
				assert(p.x >= 0 && p.x < numCols && p.y >= 0 && p.y < numRows);
				auto& b = buckets[p.x * numRows + p.y];
				if (b.len < b.indexAtDatass.size()) {	// ignore
					b.indexAtDatass[b.len++] = di;
				}
			}
		}

		XX_INLINE void AlignCohesionSeparation() {
			for (int32_t i = 0; i < datasLen; ++i) {
				auto& d1 = datas[i];
				// todo: maybe Align & Cohesion & Separation have different range
				XY sumVelocity{}, sumPosition{}, sumSeparation{};
				int32_t c{};
				ForeachByRange((int32_t)d1.pos.y, (int32_t)d1.pos.x, cPerceptionRadius + cellSize
					, [&](Data& d2, float range) {
					if (&d1 == &d2) return false; // todo: if range < ???	// todo: number limit ?
					auto v = d1.pos - d2.pos;
					auto m2 = v.x * v.x + v.y * v.y;
					if (m2 > 0 && m2 < cPerceptionRadius * cPerceptionRadius) {
						sumVelocity += d2.velocity;
						sumPosition += d2.pos;
						sumSeparation += v / std::sqrtf(m2);
						++c;
					}
					return false;
				});
				if (c) {
					auto a = sumVelocity / c;						// avg
					auto mag2 = a.x * a.x + a.y * a.y;
					if (mag2 > 0) {
						a = a / std::sqrtf(mag2) * cMaxSpeed;		// set mag
						a -= d1.velocity;							// sub
						mag2 = a.x * a.x + a.y * a.y;
						if (mag2 > cMaxForce * cMaxForce) {
							a = a / std::sqrtf(mag2) * cMaxForce;	// limit
						}
					}
					d1.acc += a;

					a = sumPosition / c;							// avg
					a -= d1.pos;									// sub
					mag2 = a.x * a.x + a.y * a.y;
					if (mag2 > 0) {
						a = a / std::sqrtf(mag2) * cMaxSpeed;		// set mag
						a -= d1.velocity;							// sub
						mag2 = a.x * a.x + a.y * a.y;
						if (mag2 > cMaxForce * cMaxForce) {
							a = a / std::sqrtf(mag2) * cMaxForce;	// limit
						}
					}
					d1.acc += a;

					a = sumSeparation / c;							// avg
					mag2 = a.x * a.x + a.y * a.y;
					if (mag2 > 0) {
						a = a / std::sqrtf(mag2) * cMaxSpeed;		// set mag
						a -= d1.velocity;							// sub
						mag2 = a.x * a.x + a.y * a.y;
						if (mag2 > cMaxForce * cMaxForce) {
							a = a / std::sqrtf(mag2) * cMaxForce;	// limit
						}
					}
					d1.acc += a;
				}
			}
		}

		XX_INLINE void UpdateCore() {
			for (int32_t i = 0; i < datasLen; ++i) {
				auto& d1 = datas[i];
				//d1.rotation = std::atan2f(d1.velocity.y, d1.velocity.x);
				d1.pos += d1.velocity;
				d1.velocity += d1.acc;
				d1.acc = 0;

				if (d1.pos.x >= numCols) {
					d1.pos.x -= numCols;
				}
				else if (d1.pos.x < 0) {
					d1.pos.x = numCols + d1.pos.x;
				}

				if (d1.pos.y >= numRows) {
					d1.pos.y -= numRows;
				}
				else if (d1.pos.y < 0) {
					d1.pos.y = numRows + d1.pos.y;
				}
			}
		}

		void Update() {
			ClearBuckets();
			FillBuckets();
			AlignCohesionSeparation();
			UpdateCore();
			// todo
		}

	};

}


namespace Game {

	struct Test5;
	struct alignas(8) Test5Item {
		Test5* scene{};
		xx::XY pos{};
		float radius{};
		float radians{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Test5Item() {};
	};

	// boids test

	struct Test5 : Scene {
		xx::Shared<xx::Node> ui;
		xx::BoidsEnv be;

		float colorIndex{};
		xx::RGBA8 GetRainbow();

		void MakeUI();

		void Init() override;
		void Update_();
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
