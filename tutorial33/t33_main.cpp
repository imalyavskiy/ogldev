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

  Tutorial 33 - Instanced Rendering
*/

#define WINDOW_WIDTH      1280  
#define WINDOW_HEIGHT     1024
#define WINDOW_BPP        32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE      "Tutorial 33"

#include "t33_mainapp.h"

int main(int argc, char** argv)
{
  t33::GLUTBackendInit(argc, argv);

  if (!t33::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  std::srand(/*WINAPI->*/GetCurrentProcessId());

  const auto pApp = std::make_unique<t33::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  return 0;
}