#pragma once

namespace Game {

	struct Camera {
		float logicScale{ 1 }, baseScale{ 1 };
		float scale{ 1 }, _1_scale{ 1 };
		XY original{};	// logic center pos

		XX_INLINE Camera& SetOriginal(XY original_) {
			original = original_;
			return *this;
		}

		XX_INLINE Camera& SetBaseScale(float baseScale_) {
			baseScale = baseScale_;
			scale = logicScale * baseScale;
			_1_scale = 1.f / scale;
			return *this;
		}

		XX_INLINE Camera& SetScale(float logicScale_, float baseScale_ = 1.f) {
			logicScale = logicScale_;
			baseScale = baseScale_;
			scale = logicScale * baseScale;
			_1_scale = 1.f / scale;
			return *this;
		}

		XX_INLINE XY ToGLPos_Logic(XY logicPos) const {
			return (logicPos - original - offset).FlipY() * logicScale;
		}

		XX_INLINE XY ToGLPos(XY logicPos) const {
			return (logicPos - original - offset).FlipY() * scale;
		}

		XX_INLINE XY ToLogicPos(XY glPos) const {
			return { glPos.x * _1_scale + original.x + offset.x, -glPos.y * _1_scale + original.y + offset.y };
		}

		/*******************************************************************************************************/
		// shake
		xx::Rnd rnd;
		float cRadius{}, cSpeed{};	// speed: increase val per frame
		int32_t cEndTime{};
		int32_t _n{}, _i{};
		XY offset{}, tarOffset{};

		// example: 5, 300.f * Cfg::frameDelay, int32_t(0.5f * Cfg::fps), time
		XX_INLINE void Shake(float radius, float speed, int32_t duration, int32_t time) {
			cRadius = radius;
			cSpeed = speed;
			cEndTime = time + duration;
		}

		XX_INLINE XY GetRndPosDoughnut(float maxRadius, float safeRadius) {
			auto len = maxRadius - safeRadius;
			auto len_radius = len / maxRadius;
			auto safeRadius_radius = safeRadius / maxRadius;
			auto radius = std::sqrtf(rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius;
			auto radians = rnd.Next<float>(-M_PI, M_PI);
			return { std::cosf(radians) * radius, std::sinf(radians) * radius };
		}

		XX_INLINE void Update(int32_t time) {
			XX_BEGIN(_n);
		LabWait:
			while (time >= cEndTime) { XX_YIELD(_n); }	// sleepEx
		LabSetTarget:
			tarOffset = GetRndPosDoughnut(cRadius, 0);
		LabMove:
			if (time >= cEndTime) {
				offset = {};
				goto LabWait;
			}
			XX_YIELD(_n);
			auto d = tarOffset - offset;
			auto dd = d.x * d.x + d.y * d.y;
			if (dd <= cSpeed * cSpeed) {
				offset = tarOffset;
				goto LabSetTarget;
			}
			else {
				offset += d / std::sqrtf(dd) * cSpeed;
				goto LabMove;
			}
			XX_END(_n);
		}

	};
}
