#pragma once
#ifndef ESSENTIALS_UI_SOFT_KEYBOARD_SOFT_KEYBOARD_DEFS
#define ESSENTIALS_UI_SOFT_KEYBOARD_SOFT_KEYBOARD_DEFS

#include <string>

namespace ds {
namespace ui {

/** SoftKeyboard
*	General definitions for the Software Keyboard classes
*/
namespace SoftKeyboardDefs {

	typedef enum { kLetter, kNumber, kDelete, kSpace, kEnter, kShift } KeyType;


	void handleKeyPressGeneric(const KeyType& inputKeyType, std::wstring& inOutCurrentKey, std::wstring& inOutFullString);
};
} // namespace ui
} // namespace ds

#endif
