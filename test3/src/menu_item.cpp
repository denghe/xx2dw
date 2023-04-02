#include "main.h"
#include "menu_item.h"


bool MenuItem::HandleMouseDown(MenuSobj_MouseEventListener& L) {
	return Inside(L.downPos);
}

int MenuItem::HandleMouseMove(MenuSobj_MouseEventListener& L) {
	return 0;
}

void MenuItem::HandleMouseUp(MenuSobj_MouseEventListener& L) {
	if (Inside(xx::engine.mousePosition)) {
		handler();
	}
}

void MenuItem::Init(SceneBase* const& scene_, xx::XY const& pos, std::string_view const& txt_, float const& fontSize, std::function<void()> handler_) {
	scene = scene_;
	txt = txt_;
	handler = std::move(handler_);

	content.SetText(looper->fnt, txt, fontSize)
		.SetPosition(pos);

	auto siz = content.size + xx::XY{ 20, 4 };
	border.FillBoxPoints({}, siz)
		.SetPosition(pos);

	siz /= 2;
	leftBottom = pos - siz;
	rightTop = pos + siz;
}

bool MenuItem::Inside(xx::XY const& p) {
	return p.x >= leftBottom.x && p.x <= rightTop.x
		&& p.y >= leftBottom.y && p.y <= rightTop.y;
}

/************************************************************************/

void Menu::Init() {
	listener.Init(xx::Mbtns::Left);
}

void Menu::Update() {
	listener.Update();
	if (items.size()) {
		auto&& iter = items.begin();
		while (listener.eventId && iter != items.end()) {
			listener.Dispatch(&*iter++);
		}
	}
}

void Menu::Draw() {
	for (auto&& o : items) {
		o.content.Draw();
	}
	for (auto&& o : items) {
		o.border.Draw();
	}
}
