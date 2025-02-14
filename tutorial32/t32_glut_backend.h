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

#include "t32_callbacks.h"
namespace t32 {
  void GLUTBackendInit(int argc, char** argv);

  bool GLUTBackendCreateWindow(unsigned int width, unsigned int height, unsigned int bpp, bool isFullScreen, const char* pTitle);

  void GLUTBackendRun(ICallbacks* pCallbacks);
}
#endif	/* GLUT_BACKEND_H */

