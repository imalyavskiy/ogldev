/*
        Copyright 2015 Etay Meiri

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

#include "t44_atb.h"
#include "t44_lights_common.h"

namespace t44
{
  TwType TW_TYPE_OGLDEV_VECTOR3F;
  TwType TW_TYPE_OGLDEV_ATTENUATION;


  ATB::ATB()
  {

  }


  bool ATB::Init()
  {
    bool ret = false;

    if (TwInit(TW_OPENGL_CORE, nullptr) == 1)
    {
      TwStructMember Vector3fMembers[] = {
          { "x", TW_TYPE_FLOAT, offsetof(Vector3f, x), "" },
          { "y", TW_TYPE_FLOAT, offsetof(Vector3f, y), "" },
          { "z", TW_TYPE_FLOAT, offsetof(Vector3f, z), "" }
      };

      TW_TYPE_OGLDEV_VECTOR3F = TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(Vector3f), nullptr, nullptr);

      TwStructMember AttenuationMembers[] = {
          { "Const", TW_TYPE_FLOAT, offsetof(LightAttenuation, Constant), "" },
          { "Linear", TW_TYPE_FLOAT, offsetof(LightAttenuation, Linear), "" },
          { "Exp", TW_TYPE_FLOAT, offsetof(LightAttenuation, Exp), "" }
      };

      TW_TYPE_OGLDEV_ATTENUATION = TwDefineStruct("Attenuation", AttenuationMembers, 3, sizeof(LightAttenuation), nullptr, nullptr);

      ret = true;
    }

    return ret;
  }


  static int OgldevKeyToATBKey(KEYBOARD_KEY OgldevKey)
  {
    if (OgldevKey >= KB_KEY_SPACE && OgldevKey <= KB_KEY_RIGHT_BRACKET) {
      return OgldevKey;
    }

    switch (OgldevKey) {

    case KB_KEY_BACKSPACE:
      return TW_KEY_BACKSPACE;
    case KB_KEY_TAB:
      return TW_KEY_TAB;
      //            return TW_KEY_CLEAR;
    case KB_KEY_ENTER:
      return TW_KEY_RETURN;

      //return TW_KEY_PAUSE;
    case KB_KEY_ESCAPE:
      return TW_KEY_ESCAPE;
    case KB_KEY_DELETE:
      return TW_KEY_DELETE;
    case KB_KEY_UP:
      return TW_KEY_UP;
    case KB_KEY_DOWN:
      return TW_KEY_DOWN;
    case KB_KEY_RIGHT:
      return TW_KEY_RIGHT;
    case KB_KEY_LEFT:
      return TW_KEY_LEFT;
    case KB_KEY_INSERT:
      return TW_KEY_INSERT;
    case KB_KEY_HOME:
      return TW_KEY_HOME;
    case KB_KEY_END:
      return TW_KEY_END;
    case KB_KEY_PAGE_UP:
      return TW_KEY_PAGE_UP;
    case KB_KEY_PAGE_DOWN:
      return TW_KEY_PAGE_DOWN;
    case KB_KEY_F1:
      return TW_KEY_F1;
    case KB_KEY_F2:
      return TW_KEY_F2;
    case KB_KEY_F3:
      return TW_KEY_F3;
    case KB_KEY_F4:
      return TW_KEY_F4;
    case KB_KEY_F5:
      return TW_KEY_F5;
    case KB_KEY_F6:
      return TW_KEY_F6;
    case KB_KEY_F7:
      return TW_KEY_F7;
    case KB_KEY_F8:
      return TW_KEY_F8;
    case KB_KEY_F9:
      return TW_KEY_F9;
    case KB_KEY_F10:
      return TW_KEY_F10;
    case KB_KEY_F11:
      return TW_KEY_F11;
    case KB_KEY_F12:
      return TW_KEY_F12;
    default:
      REPORT_ERROR("Unimplemented OGLDEV to ATB key");
    }

    return TW_KEY_LAST;
  }

  bool ATB::KeyboardCB(KEYBOARD_KEY OgldevKey)
  {
    int ATBKey = OgldevKeyToATBKey(OgldevKey);

    if (ATBKey == TW_KEY_LAST) {
      return false;
    }

    return (TwKeyPressed(ATBKey, TW_KMOD_NONE) == 1);
  }


  bool ATB::PassiveMouseCB(int x, int y)
  {
    return (TwMouseMotion(x, y) == 1);
  }


  bool ATB::MouseCB(MOUSE_BUTTON Button, KEYBOARD_KEY_STATE State, int x, int y)
  {
    TwMouseButtonID btn = (Button == MOUSE_BUTTON_LEFT) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
    TwMouseAction ma = (State == KB_KEY_STATE_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;

    return (TwMouseButton(ma, btn) == 1);
  }

}