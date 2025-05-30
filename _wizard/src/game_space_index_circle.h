#pragma once
#include "game_space_index.h"

namespace Game {

	// space index with size limited circle items
	// requires:
	/*
		XY pos{};
		float radius{};
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Base *prev{}, *next{};		// if enableDoubleLink == true
	*/
	template<typename T, bool enableDoubleLink = true>
	struct SpaceIndexCircle : SpaceIndex<T, enableDoubleLink> {
		// todo: more search funcs

		// ring diffuse search   nearest edge   best one and return
		template<bool enableExcept = false>
		T* FindNearestByRange(float x, float y, float maxDistance, T* except = {}) {
			int32_t cIdxBase = (int32_t)(x * this->_1_cellSize);
			if (cIdxBase < 0 || cIdxBase >= this->numCols) return nullptr;
			int32_t rIdxBase = (int32_t)(y * this->_1_cellSize);
			if (rIdxBase < 0 || rIdxBase >= this->numRows) return nullptr;
			auto searchRange = maxDistance + this->cellSize;

			T* rtv{};
			float maxV{};

			auto& lens = this->rdd->lens;
			auto& idxs = this->rdd->idxs;
			for (int32_t i = 1, e = lens.len; i < e; i++) {
				auto offsets = lens[i - 1].count;
				auto siz = lens[i].count - lens[i - 1].count;
				for (int32_t j = 0; j < siz; ++j) {
					auto& tmp = idxs[offsets + j];
					auto cIdx = cIdxBase + tmp.x;
					if (cIdx < 0 || cIdx >= this->numCols) continue;
					auto rIdx = rIdxBase + tmp.y;
					if (rIdx < 0 || rIdx >= this->numRows) continue;
					auto cidx = rIdx * this->numCols + cIdx;

					auto c = this->cells[cidx];
					while (c) {
						auto nex = (T*)c->next;
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
			int cIdx = (int)(x * this->_1_cellSize);
			if (cIdx < 0 || cIdx >= this->numCols) return nullptr;
			int rIdx = (int)(y * this->_1_cellSize);
			if (rIdx < 0 || rIdx >= this->numRows) return nullptr;

			// 5
			int idx = rIdx * this->numCols + cIdx;
			auto c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			if (cIdx >= this->numCols) return nullptr;
			++idx;
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			if (rIdx >= this->numRows) return nullptr;
			idx += this->numCols;
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			idx -= this->numCols;
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			idx -= this->numCols;
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
			c = this->cells[idx];
			while (c) {
				auto nex = (T*)c->next;

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
