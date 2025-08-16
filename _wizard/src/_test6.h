#pragma once


namespace xx {

	struct Noise2d {
		// ref: https://gist.github.com/digitalshadow/134a3a02b67cecd72181

		static constexpr float STRETCH_2D = -0.211324865405187; //(1/Math.sqrt(2+1)-1)/2;
		static constexpr float SQUISH_2D = 0.366025403784439; //(Math.sqrt(2+1)-1)/2;
		static constexpr float NORM_2D = 1.0 / 47.0;
		struct Contribution2 {
			float dx{}, dy{};
			int xsb{}, ysb{};
			Contribution2* Next{};
			Contribution2(float multiplier, int xsb, int ysb) {
				dx = -xsb - multiplier * SQUISH_2D;
				dy = -ysb - multiplier * SQUISH_2D;
				this->xsb = xsb;
				this->ysb = ysb;
			}
		};
		inline static std::unique_ptr<Contribution2* []> lookup2D;	// known issue: not delete( memory leak )
		inline static void StaticInit() {
			assert(!lookup2D);
			static constexpr std::array<std::array<int, 9>, 2> base2D { std::array<int, 9>
				{ 1, 1, 0, 1, 0, 1, 0, 0, 0 },
				{ 1, 1, 0, 1, 0, 1, 2, 1, 1 }
			};
			static constexpr std::array<int, 24> p2D
			{ 0, 0, 1, -1, 0, 0, -1, 1, 0, 2, 1, 1, 1, 2, 2, 0, 1, 2, 0, 2, 1, 0, 0, 0 };
			static constexpr std::array<int, 24> lookupPairs2D
			{ 0, 1, 1, 0, 4, 1, 17, 0, 20, 2, 21, 2, 22, 5, 23, 5, 26, 4, 39, 3, 42, 4, 43, 3 };

			std::array<Contribution2*, p2D.size() / 4> contributions2D;
			for (int i = 0; i < p2D.size(); i += 4) {
				auto& baseSet = base2D[p2D[i]];
				Contribution2* previous{}, *current{};
				for (int k = 0; k < baseSet.size(); k += 3) {
					current = new Contribution2((float)baseSet[k], baseSet[k + 1], baseSet[k + 2]);
					if (!previous) {
						contributions2D[i / 4] = current;
					}
					else {
						previous->Next = current;
					}
					previous = current;
				}
				current->Next = new Contribution2((float)p2D[i + 1], p2D[i + 2], p2D[i + 3]);
			}

			lookup2D = std::make_unique_for_overwrite<Contribution2*[]>(64);
			for (auto i = 0; i < lookupPairs2D.size(); i += 2) {
				lookup2D[lookupPairs2D[i]] = contributions2D[lookupPairs2D[i + 1]];
			}
		}

		std::unique_ptr<uint8_t[]> perm, perm2D;
		int32_t lastSeed{};

		void Init(int64_t seed) {
			assert(seed);
			if (lastSeed == seed) return;
			perm = std::make_unique_for_overwrite<uint8_t[]>(256);
			perm2D = std::make_unique_for_overwrite<uint8_t[]>(256);
			uint8_t source[256];
			for (int i = 0; i < 256; i++) {
				source[i] = (uint8_t)i;
			}
			seed = seed * 6364136223846793005L + 1442695040888963407L;
			seed = seed * 6364136223846793005L + 1442695040888963407L;
			seed = seed * 6364136223846793005L + 1442695040888963407L;
			for (int i = 255; i >= 0; i--) {
				seed = seed * 6364136223846793005L + 1442695040888963407L;
				int r = (int)((seed + 31) % (i + 1));
				if (r < 0) {
					r += (i + 1);
				}
				perm[i] = source[r];
				perm2D[i] = (uint8_t)(perm[i] & 0x0E);
				source[r] = source[i];
			}
		}

		float Evaluate(float x, float y) {
			auto stretchOffset = (x + y) * STRETCH_2D;
			auto xs = x + stretchOffset;
			auto ys = y + stretchOffset;

			auto xsb = FastFloor(xs);
			auto ysb = FastFloor(ys);

			auto squishOffset = (xsb + ysb) * SQUISH_2D;
			auto dx0 = x - (xsb + squishOffset);
			auto dy0 = y - (ysb + squishOffset);

			auto xins = xs - xsb;
			auto yins = ys - ysb;

			auto inSum = xins + yins;

			auto hash =
				(int)(xins - yins + 1) |
				(int)(inSum) << 1 |
				(int)(inSum + yins) << 2 |
				(int)(inSum + xins) << 4;

			auto c = lookup2D[hash];

			static constexpr float gradients2D[]{ 5, 2, 2, 5, -5, 2, -2, 5, 5, -2, 2, -5, -5, -2, -2, -5 };
			auto value = 0.0f;
			while (c) {
				auto dx = dx0 + c->dx;
				auto dy = dy0 + c->dy;
				auto attn = 2 - dx * dx - dy * dy;
				if (attn > 0) {
					auto px = xsb + c->xsb;
					auto py = ysb + c->ysb;

					auto i = perm2D[(perm[px & 0xFF] + py) & 0xFF];
					auto valuePart = gradients2D[i] * dx + gradients2D[i + 1] * dy;

					attn *= attn;
					value += attn * attn * valuePart;
				}
				c = c->Next;
			}
			return value * NORM_2D;
		}

		XX_INLINE static int FastFloor(float x) {
			auto xi = (int)x;
			return x < xi ? xi - 1 : xi;
		}

		struct Settings {
			int64_t seed{};
			int numLayers{ 1 };	//[Range(1, 8)]
			float persistence{ 0.5f }, lacunarity{ 1.8 }, scale{ 5 };
			XY offset{};
		};

		// auto Init
		std::unique_ptr<float[]> GenerateHeightmap(Settings const& noiseSettings, XYi size, bool normalize = true) {
			auto map = std::make_unique_for_overwrite<float[]>(size.x * size.y);
			xx::Rnd prng;
			prng.SetSeed(noiseSettings.seed);
			Init(noiseSettings.seed);

			// Generate random offset for each layer
			auto offsets = std::make_unique_for_overwrite<XY[]>(noiseSettings.numLayers);
			for (int layer = 0; layer < noiseSettings.numLayers; layer++) {
				offsets[layer] = XY{ prng.Next<float>() * 2.f - 1.f, prng.Next<float>() * 2.f - 1.f } * 10000.f;
				// offsets[layer] += noiseSettings.offset;
			}

			auto minHeight = std::numeric_limits<float>::max();
			auto maxHeight = std::numeric_limits<float>::min();

			for (int y = 0; y < size.y; y++) {
				for (int x = 0; x < size.x; x++) {
					float frequency = noiseSettings.scale;
					float amplitude = 1;
					float height = 0;

					// Sum layers of noise
					for (int layer = 0; layer < noiseSettings.numLayers; layer++) {
						float sampleX = (float)x / size.x * frequency + offsets[layer].x + noiseSettings.offset.x;
						float sampleY = (float)y / size.y * frequency + offsets[layer].y + noiseSettings.offset.y;
						height += Evaluate(sampleX, sampleY) * amplitude;
						frequency *= noiseSettings.lacunarity;
						amplitude *= noiseSettings.persistence;
					}
					map[y * size.x + x] = height;
					minHeight = std::min(minHeight, height);
					maxHeight = std::max(maxHeight, height);
				}
			}

			// Normalize values to range 0-1
			if (normalize) {
				for (int y = 0; y < size.y; y++) {
					for (int x = 0; x < size.x; x++) {
						auto mi = y * size.x + x;
						map[mi] = InverseLerp(minHeight, maxHeight, map[mi]);
					}
				}
			}

			return map;
		}

		XX_INLINE static float InverseLerp(float a, float b, float value) {
			if (a != b) return Clamp01((value - a) / (b - a));
			return 0.f;
		}
		XX_INLINE static float Clamp01(float value) {
			if (value < 0.f) return 0.f;
			if (value > 1.f) return 1.f;
			return value;
		}

	};

}


namespace Game {

	struct Test6;
	struct alignas(8) Test6Item {
		Test6* scene{};
		xx::XY pos{};
		float radius{};
		float radians{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Test6Item() {};
	};

	// noise test

	struct Test6 : Scene {
		xx::Shared<xx::Node> ui;
		xx::Noise2d noise;
		xx::Noise2d::Settings noiseSettings;
		XY mapSize{};
		xx::GLTexture mapTex;

		void FillMapTex();

		void MakeUI();

		void Init() override;
		void Update_();
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
