#include "main.h"
#include "scene_game.h"
#include "sobj_word.h"

void Sobj_Word::Init(Scene_Game* const& scene_, int const& idx, xx::XY const& pos_, float const& speed_, std::u32string_view const& txt_) {
	scene = scene_;
	indexAtContainer = idx;
	pos = pos_;
	speed = speed_;
	txt = txt_;
	typedLen = {};

	body.SetText(looper->fnt, txt, 64);
}

bool Sobj_Word::Update() {
	pos.y -= speed;
	return pos.y < -xx::engine.hh - 100;
}

void Sobj_Word::Draw() {
	body.SetPosition(pos).Draw();
}

void Sobj_Word::ClearTyped() {
	typedLen = {};
	body.SetColor({ 255,255,255,255 });
}

int Sobj_Word::Hit(char32_t const& c) {
	assert(txt.size());
	auto lastIdx = (ptrdiff_t)txt.size() - 1;
	assert (typedLen <= lastIdx);
	if (txt[typedLen] == c) {
		body.chars[typedLen].color = { 255,0,0,255 };
		++typedLen;
		if (typedLen > lastIdx) {
			scene->audio.Play("res/5.ogg");
			return 0;
		}
		scene->audio.Play("res/4.ogg");
		return 1;
	}
	return -1;
}
