/*

	Copyright 2010 Etay Meiri

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

    Tutorial 03 - First triangle
*/

#include <cstdio>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "t03_math_3d.h"

GLuint VBO;

static void RenderSceneCB()
{
  glClear(GL_COLOR_BUFFER_BIT);

  constexpr GLuint vertex_attribute_index = 0;

  glEnableVertexAttribArray(vertex_attribute_index);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  constexpr GLenum draw_mode = GL_TRIANGLES; // Vertices 0, 1, and 2 form a triangle.Vertices 3, 4, and 5 form a triangle.And so on.
  constexpr GLuint starting_index = 0;       // starting from index 0
  constexpr GLuint num_points = 3;           // just single vertex
  glDrawArrays(draw_mode, starting_index, num_points);

  glDisableVertexAttribArray(vertex_attribute_index);

  glutSwapBuffers();
}

void InitializeGlutCallbacks()
{
  glutDisplayFunc(RenderSceneCB);
}

void CreateVertexBuffer()
{
  const Vector3f vertices[] {
    { -1.0f, -1.0f,  0.0f },
    {  1.0f, -1.0f,  0.0f },
    {  0.0f,  1.0f,  0.0f }
  };

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

  glutInitWindowSize(1024, 768);

  glutInitWindowPosition(100, 100);

  glutCreateWindow("Tutorial 03");

  InitializeGlutCallbacks();

  const GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  CreateVertexBuffer();

  glutMainLoop();

  return 0;
}


