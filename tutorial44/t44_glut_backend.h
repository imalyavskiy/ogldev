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

#ifndef GLUT_BACKEND_H
#define	GLUT_BACKEND_H

#include "t44_callbacks.h"

namespace t44
{
  class GLUTBackend {
  public:
    static void Init(int argc, char** argv, bool WithDepth, bool WithStencil);

    static bool MakeWindow(uint32_t Width, uint32_t Height, bool isFullScreen, const char* pTitle);

    static void Run(ICallbacks* pCallbacks);

    static void SwapBuffers();

    static void LeaveMainLoop();
  };
}
#endif	/* GLUT_BACKEND_H */

