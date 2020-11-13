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

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"
#include <math.h>

GLuint VBO;

const GLfloat r1 = 0.6f;
const GLfloat r2 = 0.2f;
const GLuint SEGMENTS(3);
const GLuint SIDES(3);
const GLfloat Pi = 3.14159265358979323846f;
const GLfloat twoPi = 2 * Pi;

Vector3f* pVertices = nullptr;
GLuint uiVerteces = 0;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, uiVerteces);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{   
    uiVerteces = SEGMENTS * SIDES;
    pVertices = new Vector3f[uiVerteces];

    for (GLuint segment = 0; segment < SEGMENTS; ++segment) {
        for (GLuint side = 0; side < SIDES; ++side) {
            const GLfloat psi = (twoPi / SIDES) * side;
            const GLfloat phi = (twoPi / SEGMENTS) * segment;

            const GLfloat cosPsi = cosf(psi);
            const GLfloat sinPsi = sinf(psi);
            const GLfloat cosPhi = cosf(phi);
            const GLfloat sinPhi = sinf(phi);

            const GLuint index = SIDES * segment + side;

            Vector3f& v = pVertices[index];
            v.x = (r1 + r2 * cosPsi) * cosPhi;
            v.y = (r1 + r2 * cosPsi) * sinPhi;
            v.z = r2 * sinPsi;
        }
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, uiVerteces * sizeof(Vector3f), pVertices, GL_STATIC_DRAW);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 03");

    InitializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    CreateVertexBuffer();

    glutMainLoop();

    return 0;
}


