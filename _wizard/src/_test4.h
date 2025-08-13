#pragma once


namespace xx {

	//template <typename T>
	struct Phys2dFixedCircle {
		static constexpr float cVelocityDamping{ 100.f };
		static constexpr float cGravity{ 100.f };
		static constexpr float cMargin{ 2.f };
		static constexpr int32_t cSubSteps{ 1 };
		static constexpr float cSubDelta{ Cfg::frameDelay / 4 / cSubSteps };
		static constexpr float cResponseCoef{ 0.5f };
		static constexpr float cMaxSpeed{ 0.05f };

		struct Node {
			int32_t next;
			int32_t indexAtDatas;
			void* ud;
		};

		struct Data {
			int32_t indexAtNodes;
			xx::RGBA8 color;
			XY pos, lpos, acc;
		};

		struct Bucket {
			int32_t len;
			std::array<int32_t, 3> indexAtDatass;
		};

		int32_t rowsLen{}, colsLen{}, bucketsLen{};					// for buckets
		int32_t freeHead{ -1 }, freeCount{}, count{}, capacity{};	// for nodes
		int32_t datasLen{};											// for datas

		XY pixelSize{};

		std::unique_ptr<Node[]> nodes;
		std::unique_ptr<Data[]> datas;
		std::unique_ptr<Bucket[]> buckets;

		void Init(int32_t rowsLen_, int32_t colsLen_, int32_t capacity_ = 0) {
			assert(!nodes && !buckets && rowsLen_ > 0 && colsLen_ > 0 && capacity_ >= 0);
			rowsLen = rowsLen_;
			colsLen = colsLen_;
			bucketsLen = rowsLen_ * colsLen_;
			capacity = capacity_;
			freeHead = -1;
			freeCount = count = 0;
			pixelSize = { colsLen_, rowsLen_ };
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

		int32_t Add(void* ud_, XY pos_, XY lpos_, XY acc_, xx::RGBA8 color_ = xx::RGBA8_White) {
			assert(buckets);
			assert((int32_t)pos_.y >= 0 && (int32_t)pos_.y < rowsLen);
			assert((int32_t)pos_.x >= 0 && (int32_t)pos_.x < colsLen);

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
			d.lpos = lpos_;
			d.acc = acc_;

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

		XX_INLINE void ClearBuckets() {
			assert(buckets);
			for (int32_t i = 0; i < bucketsLen; ++i) {
				buckets[i].len = 0;
			}
		}

		XX_INLINE void FillBuckets() {
			for (int32_t di = 0; di < datasLen; ++di) {
				auto p = datas[di].pos.As<int32_t>();
				assert(p.x >= 0 && p.x < colsLen && p.y >= 0 && p.y < rowsLen);
				auto& b = buckets[p.x * rowsLen + p.y];
				if (b.len < b.indexAtDatass.size()) {	// ignore
					b.indexAtDatass[b.len++] = di;
				}
			}
		}

		XX_INLINE void CalcDatas() {
			for (int32_t bi = 0; bi < bucketsLen; ++bi) {
				auto& b = buckets[bi];
				if (!b.len) continue;
				Calc(b, buckets[bi - 1]);
				Calc(b, buckets[bi]);
				Calc(b, buckets[bi + 1]);
				Calc(b, buckets[bi + rowsLen - 1]);
				Calc(b, buckets[bi + rowsLen]);
				Calc(b, buckets[bi + rowsLen + 1]);
				Calc(b, buckets[bi - rowsLen - 1]);
				Calc(b, buckets[bi - rowsLen]);
				Calc(b, buckets[bi - rowsLen + 1]);
			}
		}

		XX_INLINE void Calc(Bucket& b1_, Bucket& b2_) {
			for (int32_t di1 = 0; di1 < b1_.len; ++di1) {
				for (int32_t di2 = 0; di2 < b2_.len; ++di2) {
					Calc(datas[b1_.indexAtDatass[di1]], datas[b2_.indexAtDatass[di2]]);
				}
			}
		}

		XX_INLINE void Calc(Data& d1_, Data& d2_) {
			static constexpr float eps{ 0.0001f };
			auto v = d1_.pos - d2_.pos;
			auto m2 = v.x * v.x + v.y * v.y;
			if (m2 >= 1.f || m2 <= eps) return;
			auto m = std::sqrtf(m2);
			auto a = cResponseCoef * (1.f - m);
			auto spd = v / m * a;
			if (spd.x > cMaxSpeed) spd.x = cMaxSpeed;
			else if (spd.x < -cMaxSpeed) spd.x = -cMaxSpeed;
			if (spd.y > cMaxSpeed) spd.y = cMaxSpeed;
			else if (spd.y < -cMaxSpeed) spd.y = -cMaxSpeed;
			d1_.pos += spd;
			d2_.pos -= spd;
		}

		XX_INLINE void UpdateDatas() {
			for (int32_t i = 0; i < datasLen; ++i) {
				auto& d = datas[i];
				d.acc += XY{ 0, cGravity };

				auto spd = d.pos - d.lpos;
				d.lpos = d.pos;
				d.pos = d.pos + spd + (d.acc - spd * cVelocityDamping) * (cSubDelta * cSubDelta);
				d.acc = {};

				if (d.pos.x > pixelSize.x - cMargin) {
					d.pos.x = pixelSize.x - cMargin;
				}
				else if (d.pos.x < cMargin) {
					d.pos.x = cMargin;
				}
				if (d.pos.y > pixelSize.y - cMargin) {
					d.pos.y = pixelSize.y - cMargin;
				}
				else if (d.pos.y < cMargin) {
					d.pos.y = cMargin;
				}
			}
		}

		void Update() {
			for (int32_t i = 0; i < cSubSteps; ++i) {
				ClearBuckets();
				FillBuckets();
				CalcDatas();
				UpdateDatas();
			}
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
		xx::Phys2dFixedCircle pfc;

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
