﻿#include "main.h"
#include "s0_main_menu.h"

void GameLooper::Init() {
	fontBase = xx::engine.LoadBMFont("res/font/coderscrux.fnt"sv);
	font3500 = xx::engine.LoadBMFont("res/font/3500+.fnt"sv);
	fpsViewer.Init(fontBase);

	scene = xx::Make<MainMenu::Scene>();
	scene->Init(this);
}

int GameLooper::Update() {
	if (xx::engine.Pressed(xx::KbdKeys::Escape)) {
		if (!dynamic_cast<MainMenu::Scene*>(scene.pointer)) {
			DelaySwitchTo<MainMenu::Scene>();
		}
	}

	int r = scene->Update();

	fpsViewer.Update();
	return r;
}




int main() {
	auto g = new GameLooper();
	return g->Run("xx2d's examples");
}