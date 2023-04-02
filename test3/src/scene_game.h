#pragma once
#include "main.h"
#include "xx2d_audio.h"
#include "sobj_word.h"
#include "virtual_keyboard.h"
// ...

struct Scene_Game : SceneBase {
	void Init() override;
	int Update() override;

	void Hit(char32_t const& c);
	xx::Coro Logic();

	xx::List<xx::Shared<Sobj_Word>, int> words;

	xx::Weak<Sobj_Word> target;

	VirtualKeyboard vk;

	xx::Audio audio;

	xx::Coros coros;

	xx::Rnd rnd;

	std::vector<std::u32string> dict;

	float timePool{};
};
