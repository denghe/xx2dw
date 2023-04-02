#include "main.h"
#include "scene_game.h"
// ...

void Scene_Game::Init() {
	vk.Init();

	// init words dict
	auto s = " began only need were animal like add as both place write group last fire their about to run run took some food read oil change these my story leave good men seem she or number with well got took took could last to began hear line has list me which number add no man he run had by are eye why while be good say only old for if talk put saw miss tell a through its until earth show word book well an ask example word began one small"sv;
	std::u32string* cs{};
	for (auto& c : s) {
		if (c == ' ') {
			cs = &dict.emplace_back();
		} else {
			cs->push_back((char32_t)c);
		}
	}

	// init logic
	coros.Add(Logic());
	words.Reserve(1024);

	// cleanup keyboard queue
	xx::engine.kbdInputs.clear();
}

int Scene_Game::Update() {
	if (xx::engine.Pressed(xx::KbdKeys::Escape)) {
		if (auto word = target.Lock()) {
			word->ClearTyped();
			target.Reset();
		}
		xx::engine.kbdInputs.clear();
	}

	timePool += xx::engine.delta;
	while (timePool >= 1.f / 120) {
		timePool -= 1.f / 120;

		// logic update
		coros();

		// keyboard handle
		auto& ks = xx::engine.kbdInputs;
		if (ks.size()) {
			for (auto& c : ks) {
				Hit(c);
			}
			ks.clear();
		}

		// virtual keyboard event handle
		vk.Update();

		// move words
		for (auto i = words.len - 1; i >= 0; --i) {
			if (words[i]->Update()) {
				words.RemoveAt(i);
			}
		}
	}

	// draw words
	for (auto& word : words) {
		word->Draw();
	}

	// draw virtual keyboard
	vk.Draw();

	return 0;
}

void Scene_Game::Hit(char32_t const& c) {
	if (auto word = target.Lock()) {
		if (word->Hit(c) == 0) {
			auto i = word->indexAtContainer;
			words.RemoveAt(i);
			target.Reset();
		}
		return;
	}

	for (auto e = words.len, i = 0; i < e; ++i) {
		auto r = words[i]->Hit(c);
		switch (r) {
		case -1:
			continue;
		case 0:
			words.RemoveAt(i);
			return;
		case 1:
			target = words[i];
			target->indexAtContainer = i;
			return;
		}
	}
}

xx::Coro Scene_Game::Logic() {
	while (true) {

		// todo: more random check  avoid first litter same ?

		auto idx = rnd.Next<int>(0, (int)dict.size() - 1);
		auto w = xx::engine.hw - 100;
		auto x = rnd.Next<int>(-w, w);
		auto siz = words.len;
		words.Emplace().Emplace()->Init(this, siz, xx::engine.ninePoints[8].MakeAdd(x, 32), 0.1f, dict[idx]);
		for (int i = 0, e = words.len; i < e; i++) {
			CoSleep(2s);
		}
	}
}
