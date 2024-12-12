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

    Tutorial 31 - PN Triangles Tessellation
*/

#define WINDOW_WIDTH      1680
#define WINDOW_HEIGHT     1050
#define WINDOW_BPP        32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE      "Tutorial 31"

#include "t31_mainapp.h"

int main(int argc, char** argv)
{
  t31::GLUTBackendInit(argc, argv);

  if (!t31::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto pApp = 
    std::make_shared<t31::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!pApp->Init())
    return 1;

  pApp->Run();

  return 0;
}