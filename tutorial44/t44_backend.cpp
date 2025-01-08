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

#include "t44_util.h"
#include "t44_backend.h"
#include "t44_glut_backend.h"
#include "t44_glfw_backend.h"


namespace t44 {
  static BACKEND_TYPE sBackendType = BACKEND_TYPE::GLUT;

  void BackendInit(BACKEND_TYPE backendType, int32_t argc, char** argv, bool withDepth, bool withStencil)
  {
    sBackendType = backendType;

    switch (backendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackend::Init(argc, argv, withDepth, withStencil);
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackend::Init(argc, argv, withDepth, withStencil);
      return;
    }
    assert(0);
  }


  void BackendTerminate()
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      // nothing to do here...
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackend::Terminate();
      return;
    }
    assert(0);
  }


  bool BackendMakeWindow(uint32_t width, uint32_t height, bool isFullScreen, const char* pTitle)
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      return GLUTBackend::MakeWindow(width, height, isFullScreen, pTitle);
    case BACKEND_TYPE::GLFW:
      return GLFWBackend::MakeWindow(width, height, isFullScreen, pTitle);
    }

    assert(0);
    return false;
  }


  void BackendRun(ICallbacks* pCallbacks)
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackend::Run(pCallbacks);
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackend::Run(pCallbacks);
      return;
    }
    assert(0);
  }


  void BackendSwapBuffers()
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackend::SwapBuffers();
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackend::SwapBuffers();
      return;
    }
    assert(0);
  }


  void BackendLeaveMainLoop()
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      GLUTBackend::LeaveMainLoop();
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackend::LeaveMainLoop();
      return;
    }
    assert(0);
  }


  void BackendSetMousePos(uint32_t x, uint32_t y)
  {
    switch (sBackendType) {
    case BACKEND_TYPE::GLUT:
      //GLUTBackendLeave::MainLoop();
      return;
    case BACKEND_TYPE::GLFW:
      GLFWBackend::SetMousePos(x, y);
      return;
    }
    assert(0);
  }
}