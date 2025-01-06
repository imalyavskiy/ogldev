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

enum class KEYBOARD_KEY : int32_t
{
  SPACE = 32,
  APOSTROPHE = 39,
  PLUS = 43,
  COMMA = 44,
  MINUS = 45,
  PERIOD = 46,
  SLASH = 47,
  NUM0 = 48,
  NUM1 = 49,
  NUM2 = 50,
  NUM3 = 51,
  NUM4 = 52,
  NUM5 = 53,
  NUM6 = 54,
  NUM7 = 55,
  NUM8 = 56,
  NUM9 = 57,
  SEMICOLON = 58,
  EQUAL = 61,
  A = 65,
  B = 66,
  C = 67,
  D = 68,
  E = 69,
  F = 70,
  G = 71,
  H = 72,
  I = 73,
  J = 74,
  K = 75,
  L = 76,
  M = 77,
  N = 78,
  O = 79,
  P = 80,
  Q = 81,
  R = 82,
  S = 83,
  T = 84,
  U = 85,
  V = 86,
  W = 87,
  X = 88,
  Y = 89,
  Z = 90,
  L_BRACKET = 91,
  BACKSLASH = 92,
  R_BRACKET = 93,
  a = 97,
  b = 98,
  c = 99,
  d = 100,
  e = 101,
  f = 102,
  g = 103,
  h = 104,
  i = 105,
  j = 106,
  k = 107,
  l = 108,
  m = 109,
  n = 110,
  o = 111,
  p = 112,
  q = 113,
  r = 114,
  s = 115,
  t = 116,
  u = 117,
  v = 118,
  w = 119,
  x = 120,
  y = 121,
  z = 122,
  ESCAPE,
  ENTER,
  TAB,
  BACKSPACE,
  INSERT,
  DEL,
  RIGHT,
  LEFT,
  DOWN,
  UP,
  PAGE_UP,
  PAGE_DN,
  HOME,
  END,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  UNDEF = 999,
};

enum class MOUSE_BUTTON : int32_t
{
  LEFT,
  MIDDLE,
  RIGHT,
  UNDEF = 999,
};

enum class KEYBOARD_KEY_STATE : int32_t
{
  PRESS,
  RELEASE
};

#endif