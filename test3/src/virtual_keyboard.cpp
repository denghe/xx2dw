#include "main.h"
#include "virtual_keyboard.h"

bool VirtualKeyboardItem::Inside(xx::XY const& p) {
	// todo: calc owner's pos
	return p.x >= leftBottom.x && p.x <= rightTop.x
		&& p.y >= leftBottom.y && p.y <= rightTop.y;
}

void VirtualKeyboardItem::Init(VirtualKeyboard* const& owner_, xx::KbdKeys const& key_
	, std::string_view const& t1_, std::string_view const& t2_, xx::XY const& pos_, xx::XY const& wh_) {
	owner = owner_;
	key = key_;
	t1 = t1_;
	t2 = t2_;
	pos = pos_;
	wh = wh_;

	lb1.SetText(looper->fnt, t1);
	lb2.SetText(looper->fnt, t2);
	border.FillBoxPoints({}, wh);
	leftBottom = pos - wh / 2;
	rightTop = pos + wh / 2;
}

void VirtualKeyboardItem::DrawTxt() {
	if (t2.size()) {
		lb1.SetPosition(owner->pos + pos.MakeAdd(0, 17)).Draw();
		lb2.SetPosition(owner->pos + pos.MakeAdd(0, -18)).Draw();
	} else {
		lb1.SetPosition(owner->pos + pos).Draw();
	}
}

void VirtualKeyboardItem::DrawBorder() {
	border.SetColor(owner->Pressed(key) ? xx::RGBA8{255,0,0,255} : xx::RGBA8{255,255,255,255});
	border.SetPosition(owner->pos + pos).Draw();
}

bool VirtualKeyboardItem::HandleMouseDown(VirtualKeyboardItemMouseEventListener& L) {
	assert(owner->mouseClickedKey == xx::KbdKeys::Null);
	if (Inside(L.downPos - owner->pos)) {
		owner->mouseClickedKey = key;
		return true;
	}
	return false;
}

int VirtualKeyboardItem::HandleMouseMove(VirtualKeyboardItemMouseEventListener& L) {
	if (!Inside(xx::engine.mousePosition - owner->pos)) {
		owner->mouseClickedKey = {};
		return 4;
	}
	return 0;
}

void VirtualKeyboardItem::HandleMouseUp(VirtualKeyboardItemMouseEventListener& L) {
	owner->mouseClickedKey = {};
}


/***********************************************************************************************/

void VirtualKeyboard::Init() {

	meListener.Init(xx::Mbtns::Left);

	using namespace xx;
	std::array<KbdKeys, 54> keys;
	std::array<std::string_view, 54> txts1;
	std::array<std::string_view, 54> txts2;

	keys = {
		// ` 1 2 3 4 5 6 7 8 9 0 - = Backspace
		KbdKeys::Grave, KbdKeys::One, KbdKeys::Two, KbdKeys::Three, KbdKeys::Four,
		KbdKeys::Five, KbdKeys::Six, KbdKeys::Seven, KbdKeys::Eight, KbdKeys::Nine,
		KbdKeys::Zero, KbdKeys::Minus, KbdKeys::Equal, KbdKeys::Backspace,
		// Tab q w e r t y u i o p [ ] |
		KbdKeys::Tab, KbdKeys::Q, KbdKeys::W, KbdKeys::E, KbdKeys::R, KbdKeys::T,
		KbdKeys::Y, KbdKeys::U, KbdKeys::I, KbdKeys::O, KbdKeys::P,
		KbdKeys::LeftBracket, KbdKeys::RightBracket, KbdKeys::Backslash,
		// CapsLock a s d f g h j k l ; ' Enter
		KbdKeys::CapsLock, KbdKeys::A, KbdKeys::S, KbdKeys::D, KbdKeys::F,
		KbdKeys::G, KbdKeys::H, KbdKeys::J, KbdKeys::K, KbdKeys::L,
		KbdKeys::Semicolon, KbdKeys::Apostrophe, KbdKeys::Enter,
		// LeftShift z x c v b n m , . / RightShift
		KbdKeys::LeftShift, KbdKeys::Z, KbdKeys::X, KbdKeys::C, KbdKeys::V, KbdKeys::B,
		KbdKeys::N, KbdKeys::M,	KbdKeys::Comma, KbdKeys::Period, KbdKeys::Slash, KbdKeys::RightShift,
		// Space
		KbdKeys::Space
	};
	txts1 = {
		"~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "Backspace",
		"Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "|",
		"CapsLock", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Enter",
		"Shift", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "Shift",
		"Space"
	};
	txts2 = {
		"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "",
		"", "", "", "", "", "", "", "", "", "", "", "[", "]", "\\",
		"", "", "", "", "", "", "", "", "", "", ";", "'", "",
		"", "", "", "", "", "", "", "", ",", ".", "/", "",
		""
	};

	items.reserve(keys.size());

	auto totalWidth1 = (width + margin) * 13 + width_backspace;
	auto totalWidth2 = width_tab + margin + (width + margin) * 12 + width_backslash;
	auto totalWidth3 = width_capslock + margin + (width + margin) * 11 + width_enter;
	auto totalWidth4 = width_left_shift + margin + (width + margin) * 10 + width_right_shift;
	assert(totalWidth1 == totalWidth2);
	assert(totalWidth1 == totalWidth3);
	assert(totalWidth1 == totalWidth4);
	auto totalHeight = (height + margin) * 5;

	xx::XY original{ -totalWidth1 / 2, totalHeight };

	size_t i{};
	auto xy = original;

	// ` 1 2 3 4 5 6 7 8 9 0 - = ........
	xy.y -= height / 2;
	for (int j = 0; j < 13; j++) {
		xy.x += width / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width, height });
		xy.x += width / 2 + margin;
		++i;
	}
	// ......................... Backspace
	{
		xy.x += width_backspace / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_backspace, height });
		++i;
	}

	xy.x = original.x;
	xy.y -= height / 2 + margin + height / 2;
	// Tab ...............................
	{
		xy.x += width_tab / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_tab, height });
		xy.x += width_tab / 2 + margin;
		++i;
	}
	// .... q w e r t y u i o p [ ]
	for (int j = 0; j < 12; j++) {
		xy.x += width / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width, height });
		xy.x += width / 2 + margin;
		++i;
	}
	// .............................. |
	{
		xy.x += width_backslash / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_backslash, height });
		++i;
	}

	xy.x = original.x;
	xy.y -= height / 2 + margin + height / 2;
	// CapsLock ..........................
	{
		xy.x += width_capslock / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_capslock, height });
		xy.x += width_capslock / 2 + margin;
		++i;
	}
	// .... a s d f g h j k l ; ' ....
	for (int j = 0; j < 11; j++) {
		xy.x += width / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width, height });
		xy.x += width / 2 + margin;
		++i;
	}
	// .............................. Enter
	{
		xy.x += width_enter / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_enter, height });
		++i;
	}

	xy.x = original.x;
	xy.y -= height / 2 + margin + height / 2;
	// LeftShift ..........................
	{
		xy.x += width_left_shift / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_left_shift, height });
		xy.x += width_left_shift / 2 + margin;
		++i;
	}
	// .... z x c v b n m , . / ....
	for (int j = 0; j < 10; j++) {
		xy.x += width / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width, height });
		xy.x += width / 2 + margin;
		++i;
	}
	// .............................. RightShift
	{
		xy.x += width_right_shift / 2;
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_right_shift, height });
		++i;
	}

	xy.x = 0;
	xy.y -= height / 2 + margin + height / 2;
	// Space
	{
		items.emplace_back().Init(this, keys[i], txts1[i], txts2[i], xy, { width_space, height });
	}
}

void VirtualKeyboard::Update() {
	pos = xx::engine.ninePoints[2];
	
	meListener.Update();
	auto&& iter = items.begin();
	while (meListener.eventId && iter != items.end()) {
		meListener.Dispatch(&*iter++);
	}

	// todo
}

void VirtualKeyboard::Draw() {
	for (auto&& o : items) {
		o.DrawTxt();
	}
	for (auto&& o : items) {
		o.DrawBorder();
	}
}

bool VirtualKeyboard::Pressed(xx::KbdKeys const& key) {
	return xx::engine.Pressed(key) || key == mouseClickedKey;
}

// todo
