#pragma once
#include "xx2d.h"
#include "xx2d_fps_viewer.h"
#include "menu_item.h"

struct GameLooper;
extern GameLooper* looper;

struct SceneBase {
	virtual void Init() = 0;
	virtual int Update() = 0;
	virtual ~SceneBase() {};
};

struct GameLooper : xx::GameLooperBase {
	xx::BMFont fnt;
	xx::FpsViewer fpsViewer;

	xx::Shared<SceneBase> scene;
	template<typename LT>
	void DelaySwitchTo() {
		xx::engine.DelayExecute([this] {
			scene = xx::Make<LT>();
			scene->Init();
		});
	}

	void Init() override;
	int Update() override;
};
