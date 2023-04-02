#include "main.h"
#include "scene_menu.h"

GameLooper* looper{};
int main() {
	auto g = new GameLooper();
	::looper = g;
	return g->Run("xx2d's game: word destroyer");
}


void GameLooper::Init() {
	fnt = xx::engine.LoadBMFont("res/coderscrux.fnt"sv);
	fpsViewer.Init(fnt);

	scene = xx::Make<Scene_Menu>();
	scene->Init();
}

int GameLooper::Update() {
	if (!scene) return 1;
	if (int r = scene->Update()) return r;

	fpsViewer.Update();
	return 0;
}
