#pragma once
#include "xx2d.h"

struct VirtualKeyboardItem;
using VirtualKeyboardItemMouseEventListener = xx::MouseEventListener<VirtualKeyboardItem*>;

struct VirtualKeyboard;
struct VirtualKeyboardItem {
	VirtualKeyboard* owner{};
	xx::KbdKeys key{};
	std::string t1, t2;
	xx::XY pos{}, wh{}, leftBottom{}, rightTop{};	// bounding box

	xx::SimpleLabel lb1, lb2;
	xx::LineStrip border;

	void Init(VirtualKeyboard* const& owner_, xx::KbdKeys const& k, std::string_view const& t1
		, std::string_view const& t2, xx::XY const& pos_, xx::XY const& wh_);
	bool Inside(xx::XY const& p);
	void DrawTxt();
	void DrawBorder();

	bool HandleMouseDown(VirtualKeyboardItemMouseEventListener& L);
	int HandleMouseMove(VirtualKeyboardItemMouseEventListener& L);
	void HandleMouseUp(VirtualKeyboardItemMouseEventListener& L);
};

struct VirtualKeyboard {
	std::vector<VirtualKeyboardItem> items;
	float margin{ 16 }, width{ 80 }, height{ 80 };	// every key display config
	float width_backspace{ 160 };
	float width_tab{ 130 }, width_backslash{ 110 };
	float width_capslock{ 150 }, width_enter{ 186 };
	float width_left_shift{ 200 }, width_right_shift{ 232 };
	float width_space{ (width + margin) * 6 };
	xx::XY pos{};	// current root pos
	// todo: shift, capslock check
	VirtualKeyboardItemMouseEventListener meListener;
	xx::KbdKeys mouseClickedKey{};

	void Init();
	void Update();
	void Draw();

	bool Pressed(xx::KbdKeys const& key);
	// show? hide? key press callback? blink effect tips?
};
