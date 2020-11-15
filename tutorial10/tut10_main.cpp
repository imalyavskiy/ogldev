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

    Tutorial 10 - Indexed draws
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <assert.h>

#include "math_3d.h"

#define TORUS

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
const GLfloat r1 = 0.6f;
const GLfloat r2 = 0.2f;
const GLuint SEGMENTS(16);
const GLuint SIDES(25);

const GLfloat Pi = 3.14159265358979323846f;
const GLfloat twoPi = 2 * Pi;

struct VertexParams {
    float phi;
    float psi;
};

VertexParams* pParams = nullptr;

Vector3f* pVertices = nullptr;
Vector3f* pNormals = nullptr;
Vector3ui* pTriangles = nullptr;

GLuint uiVertices = 0;
GLuint& uiNormals = uiVertices;
GLuint uiTriangles;
#ifdef _DEBUG
#define MEMSET(_DEST, _VAL, _SIZE) memset(_DEST, _VAL, _SIZE)
#else
#define MEMSET(_DEST, _VAL, _SIZE)
#endif



static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);                                   \n\
    //Color = vec4(1.0, 1.0, 1.0, 1.0);                                             \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 Color;                                                                      \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}";

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    
    Scale += 0.01f;
    
    Matrix4f World;
    
    World.m[0][0] = cosf(Scale); World.m[0][1] = 0.0f; World.m[0][2] = -sinf(Scale); World.m[0][3] = 0.0f;
    World.m[1][0] = 0.0;         World.m[1][1] = 1.0f; World.m[1][2] = 0.0f        ; World.m[1][3] = 0.0f;
    World.m[2][0] = sinf(Scale); World.m[2][1] = 0.0f; World.m[2][2] = cosf(Scale) ; World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f; World.m[3][2] = 0.0f        ; World.m[3][3] = 1.0f;
    
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (pTriangles) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, sizeof(Vector3ui) * uiTriangles, GL_UNSIGNED_INT, nullptr);
    }
    else {
        glDrawArrays(GL_POINTS, 0, uiVertices);
    }

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
    uiVertices = SEGMENTS * SIDES;

    if (pParams) {
        delete[] pParams;
        pParams = nullptr;
    }
    pParams = new VertexParams[uiVertices];
    MEMSET(pParams, 0, sizeof(VertexParams) * uiVertices);

    if (pVertices) {
        delete[] pVertices;
        pVertices = nullptr;
    }
    pVertices = new Vector3f[uiVertices];
    MEMSET(pVertices, 0, sizeof(Vector3f) * uiVertices);

    // x = (R + r * cos(𝜓)) * cos(𝜑);
    // y = (R + r * cos(𝜓)) * sin(𝜑);
    // z = r * sin(𝜓);
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

            const float radperdeg = twoPi / 360.0;

            VertexParams& p = pParams[index];
            p.psi = psi / radperdeg;
            p.phi = phi / radperdeg;
        }
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, uiVertices * sizeof(Vector3f), pVertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    if (pTriangles) {
        delete[] pTriangles;
        pTriangles = nullptr;
    }
    assert(uiVertices != 0);
    uiTriangles = uiVertices * 2;
    pTriangles = new Vector3ui[uiTriangles];
    MEMSET(pTriangles, 0, sizeof(Vector3ui) * uiTriangles);


    for (GLuint segment = 0; segment < SEGMENTS; ++segment) {
        for (GLuint side = 0; side < SIDES; ++side) {
            const GLuint index0 = SIDES * segment + side;
            const GLuint index1 = SIDES * segment + (((side + 1) < SIDES) ? side + 1 : 0);
            const GLuint index2 = SIDES * ((segment + 1) < SEGMENTS ? segment + 1 : 0) + (((side + 1) < SIDES) ? side + 1 : 0);
            const GLuint index3 = SIDES * ((segment + 1) < SEGMENTS ? segment + 1 : 0) + side;

            Vector3ui* pTriangle1 = &pTriangles[index0 * 2];
            pTriangle1->x = index2;
            pTriangle1->y = index1;
            pTriangle1->z = index0;

            Vector3ui* pTriangle2 = &pTriangles[index0 * 2 + 1];
            pTriangle2->x = index3;
            pTriangle2->y = index2;
            pTriangle2->z = index0;
        }
    }

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3ui) * uiTriangles, pTriangles, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 10");

    InitializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    CreateVertexBuffer();
    CreateIndexBuffer();
    
    CompileShaders();

    glutMainLoop();

    return 0;
}
