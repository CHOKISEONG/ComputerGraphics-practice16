/// --------------------
///      toggle_key
/// 
/// only for keyboard toggle
/// --------------------
#pragma once

#include <array>

enum Mouse {
	MLEFT,
	MRIGHT
};

std::array<bool, 10> toggle_number{};
std::array<bool, 2> toggle_mouse{};
void Toggle_key(char ch)
{
	if (ch >= '0' && ch <= '9')
		toggle_number[ch - '0'] = !toggle_number[ch - '0'];
}
void Toggle_mouse(Mouse mouse)
{
	if (mouse == LEFT)
		toggle_mouse[LEFT] = !toggle_mouse[LEFT];
	else if (mouse == RIGHT)
		toggle_mouse[RIGHT] = !toggle_mouse[RIGHT];
}