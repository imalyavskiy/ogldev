/*

	Copyright 2011 Etay Meiri

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

#ifndef CALLBACKS_H
#define	CALLBACKS_H

namespace t31 {
  class ICallbacks
  {
  public:
    virtual ~ICallbacks() = default;

    virtual void SpecialKeyboardCB(int key, int x, int y) = 0;

    virtual void KeyboardCB(unsigned char key, int x, int y) = 0;

    virtual void PassiveMouseCB(int x, int y) = 0;

    virtual void RenderSceneCB() = 0;

    virtual void IdleCB() = 0;

    virtual void MouseCB(int button, int state, int x, int y) = 0;
  };
}

#endif	/* CALLBACKS_H */

