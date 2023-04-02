#pragma once
#include "main.h"

struct Scene_Game;
struct Sobj_Word {
	Scene_Game* scene{};
	int indexAtContainer{-1};
	int typedLen{};
	float speed;
	xx::XY pos{};
	std::u32string txt;
	xx::SimpleLabel body;

	void Init(Scene_Game* const& scene_, int const& idx, xx::XY const& pos_, float const& speed_, std::u32string_view const& txt_);
	bool Update();
	void Draw();

	void ClearTyped();
	int Hit(char32_t const& c);	// 1: match, has left   0: match, finish   -1: no match
};
