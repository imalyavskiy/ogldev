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

#ifndef OGLDEV_KEYS_H
#define OGLDEV_KEYS_H

#include "t44_callbacks.h"
namespace t44 {

  enum class BACKEND_TYPE {
    GLUT,
    GLFW
  };


  void OgldevBackendInit(BACKEND_TYPE backendType, int argc, char** argv, bool withDepth, bool withStencil);

  void OgldevBackendTerminate();

  bool OgldevBackendCreateWindow(uint32_t width, uint32_t height, bool isFullScreen, const char* pTitle);

  void OgldevBackendRun(ICallbacks* pCallbacks);

  void OgldevBackendLeaveMainLoop();

  void OgldevBackendSwapBuffers();

  void OgldevBackendSetMousePos(uint32_t x, uint32_t y);

}
#endif