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


#ifndef OGLDEV_CALLBACKS_H
#define OGLDEV_CALLBACKS_H

#include "t44_keys.h"

class ICallbacks
{
public:

    virtual void KeyboardCB(KEYBOARD_KEY OgldevKey, KEYBOARD_KEY_STATE OgldevKeyState = KEYBOARD_KEY_STATE::PRESS) {};
    
    virtual void PassiveMouseCB(int x, int y) {};

    virtual void RenderSceneCB() {};

    virtual void MouseCB(MOUSE_BUTTON Button, KEYBOARD_KEY_STATE State, int x, int y) {};
};


#endif