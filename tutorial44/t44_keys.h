/*

  Copyright 2014 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OGLDEV_KEYS_H__
#define OGLDEV_KEYS_H__

enum KEYBOARD_KEY
{
  KB_KEY_SPACE = 32,
  KB_KEY_APOSTROPHE = 39,
  KB_KEY_PLUS = 43,
  KB_KEY_COMMA = 44,
  KB_KEY_MINUS = 45,
  KB_KEY_PERIOD = 46,
  KB_KEY_SLASH = 47,
  KB_KEY_NUM0 = 48,
  KB_KEY_NUM1 = 49,
  KB_KEY_NUM2 = 50,
  KB_KEY_NUM3 = 51,
  KB_KEY_NUM4 = 52,
  KB_KEY_NUM5 = 53,
  KB_KEY_NUM6 = 54,
  KB_KEY_NUM7 = 55,
  KB_KEY_NUM8 = 56,
  KB_KEY_NUM9 = 57,
  KB_KEY_SEMICOLON = 58,
  KB_KEY_EQUAL = 61,
  KB_KEY_A = 65,
  KB_KEY_B = 66,
  KB_KEY_C = 67,
  KB_KEY_D = 68,
  KB_KEY_E = 69,
  KB_KEY_F = 70,
  KB_KEY_G = 71,
  KB_KEY_H = 72,
  KB_KEY_I = 73,
  KB_KEY_J = 74,
  KB_KEY_K = 75,
  KB_KEY_L = 76,
  KB_KEY_M = 77,
  KB_KEY_N = 78,
  KB_KEY_O = 79,
  KB_KEY_P = 80,
  KB_KEY_Q = 81,
  KB_KEY_R = 82,
  KB_KEY_S = 83,
  KB_KEY_T = 84,
  KB_KEY_U = 85,
  KB_KEY_V = 86,
  KB_KEY_W = 87,
  KB_KEY_X = 88,
  KB_KEY_Y = 89,
  KB_KEY_Z = 90,
  KB_KEY_LEFT_BRACKET = 91,
  KB_KEY_BACKSLASH = 92,
  KB_KEY_RIGHT_BRACKET = 93,
  KB_KEY_a = 97,
  KB_KEY_b = 98,
  KB_KEY_c = 99,
  KB_KEY_d = 100,
  KB_KEY_e = 101,
  KB_KEY_f = 102,
  KB_KEY_g = 103,
  KB_KEY_h = 104,
  KB_KEY_i = 105,
  KB_KEY_j = 106,
  KB_KEY_k = 107,
  KB_KEY_l = 108,
  KB_KEY_m = 109,
  KB_KEY_n = 110,
  KB_KEY_o = 111,
  KB_KEY_p = 112,
  KB_KEY_q = 113,
  KB_KEY_r = 114,
  KB_KEY_s = 115,
  KB_KEY_t = 116,
  KB_KEY_u = 117,
  KB_KEY_v = 118,
  KB_KEY_w = 119,
  KB_KEY_x = 120,
  KB_KEY_y = 121,
  KB_KEY_z = 122,
  KB_KEY_ESCAPE,
  KB_KEY_ENTER,
  KB_KEY_TAB,
  KB_KEY_BACKSPACE,
  KB_KEY_INSERT,
  KB_KEY_DELETE,
  KB_KEY_RIGHT,
  KB_KEY_LEFT,
  KB_KEY_DOWN,
  KB_KEY_UP,
  KB_KEY_PAGE_UP,
  KB_KEY_PAGE_DOWN,
  KB_KEY_HOME,
  KB_KEY_END,
  KB_KEY_F1,
  KB_KEY_F2,
  KB_KEY_F3,
  KB_KEY_F4,
  KB_KEY_F5,
  KB_KEY_F6,
  KB_KEY_F7,
  KB_KEY_F8,
  KB_KEY_F9,
  KB_KEY_F10,
  KB_KEY_F11,
  KB_KEY_F12,
  KB_KEY_UNDEFINED = 999,
};

enum MOUSE_BUTTON {
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_RIGHT,
  MOUSE_UNDEFINED = 999
};

enum KEYBOARD_KEY_STATE {
  KB_KEY_STATE_PRESS,
  KB_KEY_STATE_RELEASE
};

#endif