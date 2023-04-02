#include "main.h"
#include "scene_menu.h"
#include "scene_game.h"

void Scene_Menu::Init() {
	menu.Init();
	menu.items.emplace_back().Init(this, {0, 150}, "play", 128, [this]() {
		looper->DelaySwitchTo<Scene_Game>();
	});
	menu.items.emplace_back().Init(this, {0, -150}, "exit", 128, [this]() {
		looper->scene.Reset();
	});
}

int Scene_Menu::Update() {
	menu.Update();

	menu.Draw();
	return 0;
}
