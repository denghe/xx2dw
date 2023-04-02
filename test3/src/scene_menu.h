#pragma once
#include "main.h"

struct Scene_Menu : SceneBase {
	Menu menu;
	void Init() override;
	int Update() override;
};
