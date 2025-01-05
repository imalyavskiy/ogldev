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

#include <AntTweakBar.h>

#include "t44_util.h"
#include "t44_backend.h"
#include "t44_glut_backend.h"
#include "t44_glfw_backend.h"


namespace t44 {
  static BACKEND_TYPE sBackendType = BACKEND_TYPE::GLUT;

  void OgldevBackendInit(BACKEND_TYPE backendType, int argc, char** argv, bool withDepth, bool withStencil)
  {
    sBackendType = backendType;

    switch (backendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackendInit(argc, argv, withDepth, withStencil);
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackendInit(argc, argv, withDepth, withStencil);
      return;
    }
    assert(0);
  }


  void OgldevBackendTerminate()
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      // nothing to do here...
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackendTerminate();
      return;
    }
    assert(0);
  }


  bool OgldevBackendCreateWindow(uint32_t width, uint32_t height, bool isFullScreen, const char* pTitle)
  {
    TwWindowSize(width, height);

    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      return GLUTBackendCreateWindow(width, height, isFullScreen, pTitle);
    case BACKEND_TYPE::GLFW:
      return GLFWBackendCreateWindow(width, height, isFullScreen, pTitle);
    }

    assert(0);
    return false;
  }


  void OgldevBackendRun(ICallbacks* pCallbacks)
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackendRun(pCallbacks);
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackendRun(pCallbacks);
      return;
    }
    assert(0);
  }


  void OgldevBackendSwapBuffers()
  {
    TwDraw();

    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackendSwapBuffers();
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackendSwapBuffers();
      return;
    }
    assert(0);
  }


  void OgldevBackendLeaveMainLoop()
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackendLeaveMainLoop();
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackendLeaveMainLoop();
      return;
    }
    assert(0);
  }


  void OgldevBackendSetMousePos(uint32_t x, uint32_t y)
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      //GLUTBackendLeaveMainLoop();
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackendSetMousePos(x, y);
      return;
    }
    assert(0);
  }
}