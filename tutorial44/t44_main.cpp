/*

  Copyright 2013 Etay Meiri

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

    Tutorial 44 - GLFW
*/

#include "t44_mainapp.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE "Tutorial 44"

int main(int argc, char** argv)
{
  t44::BackendInit(t44::BACKEND_TYPE::GLFW, argc, argv, true, false);

  if (!t44::BackendMakeWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FULLSCREEN, WINDOW_TITLE)) {
    t44::BackendTerminate();
    return 1;
  }

  std::srand(/*WINAPI->*/GetCurrentProcessId());

  const auto app = std::make_unique<t44::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!app->Init())
  {
    t44::BackendTerminate();
    return 1;
  }

  app->Run();

  t44::BackendTerminate();

  return 0;
}