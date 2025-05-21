#pragma once

namespace Game {

	inline void Scene::UpdateScale() {
		auto ws = gLooper.windowSize;
		lastWindowSize = ws;							// cleanup flag
		if (ws.x > ws.y) {
			scale = ws.y / Cfg::height;
			size.y = Cfg::height;
			size.x = ws.x / scale;
		}
		else {
			scale = ws.x / Cfg::width;
			size.x = Cfg::width;
			size.y = ws.y / scale;
		}
		size_2 = size * 0.5f;
		pos7 = { -size_2.x, +size_2.y }; pos8 = { 0, +size_2.y }; pos9 = { +size_2.x, +size_2.y };
		pos4 = { -size_2.x, 0 }; pos5 = { 0, 0 }; pos6 = { +size_2.x, 0 };
		pos1 = { -size_2.x, -size_2.y }; pos2 = { 0, -size_2.y }; pos3 = { +size_2.x, -size_2.y };
	}

	inline void Scene::BeforeUpdate() {
		if (lastWindowSize != gLooper.windowSize) {
			UpdateScale();
			OnWindowSizeChanged();
		}
	}

}
