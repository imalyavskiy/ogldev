#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t16_pipeline.h"
#include "t16_camera.h"
#include "t16_texture.h"


#define VERTICAL_FOV 60.f
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define Z_NEAR_CLIPPING_PLANE 1.f
#define Z_FAR_CLIPPING_PLANE 100.f

namespace t16
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() = default;

    Vertex(Vector3f pos, Vector2f tex)
    {
      m_pos = pos;
      m_tex = tex;
    }
  };
}


GLuint VBO;
GLuint IBO;
GLuint gWVPLocation;
GLuint gSampler;
t16::Texture* pTexture = nullptr;
t16::Camera* pGameCamera = nullptr;

static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec2 TexCoord;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec2 TexCoord0;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    TexCoord0 = TexCoord;                                                           \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec2 TexCoord0;                                                                  \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
uniform sampler2D gSampler;                                                         \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = texture2D(gSampler, TexCoord0.xy);                                  \n\
}";

static void RenderSceneCB()
{
  pGameCamera->OnRender();

  glClear(GL_COLOR_BUFFER_BIT);

  static float Scale = 0.0f;

  Scale += 0.1f;

  t16::Pipeline pipeline;
  pipeline.Rotate(0.0f, Scale, 0.0f);
  pipeline.WorldPos(0.0f, 0.0f, 3.0f);
  pipeline.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
  pipeline.SetPerspectiveProj(VERTICAL_FOV, WINDOW_WIDTH, WINDOW_HEIGHT, Z_NEAR_CLIPPING_PLANE, Z_FAR_CLIPPING_PLANE);

  glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(pipeline.GetTrans()));

  glEnableVertexAttribArray(0); // These magic 0 and 1 are locations(aka attributes of the vertex taken from vertex shader)
  glEnableVertexAttribArray(1); // Please look at "in vec3 Position" and "in vec 2 TexCoord" variable definitions in the shader.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t16::Vertex), nullptr);
                                // Here the actual data plugged to shader's input:
                                //    attr - 0,
                                //    3 - items,
                                //    of float,
                                //    not normalized,
                                //    stride width of 20 bytes(float - 4bytes * 5)
                                //    first byte addr from the attached buffer beginning

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(t16::Vertex), reinterpret_cast<const GLvoid*>(12));
                                // Here the actual data plugged to shader's input:
                                //    attr - 1,
                                //    2 - items,
                                //    of float,
                                //    not normalized,
                                //    stride width of 20 bytes(float - 4bytes * 5)
                                //    12th(0 based) byte addr from the attached buffer beginning

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  pTexture->Bind(GL_TEXTURE0);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

  glDisableVertexAttribArray(0); // These magic 0 and 1 are locations(aka attributes of the vertex taken from vertex shader)
  glDisableVertexAttribArray(1); // Please look at "in vec3 Position" and "in vec 2 TexCoord" variable definitions in the shader.

  glutSwapBuffers();
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
  pGameCamera->OnKeyboard(Key);
}


static void KeyboardCB(unsigned char Key, int x, int y)
{
  switch (Key) {
  case 0x1b: // Esc
    glutLeaveMainLoop();
  }
}


static void PassiveMouseCB(int x, int y)
{
  pGameCamera->OnMouse(x, y);
}


static void InitializeGlutCallbacks()
{
  glutDisplayFunc(RenderSceneCB);
  glutIdleFunc(RenderSceneCB);
  glutSpecialFunc(SpecialKeyboardCB);
  glutPassiveMotionFunc(PassiveMouseCB);
  glutKeyboardFunc(KeyboardCB);
}


static void CreateVertexBuffer()
{
  t16::Vertex Vertices[4] = {
      t16::Vertex(t16::Vector3f( -1.0f, -1.0f,  0.57730f ), t16::Vector2f(0.0f, 0.0f)),
      t16::Vertex(t16::Vector3f(  0.0f, -1.0f, -1.15475f ), t16::Vector2f(0.5f, 0.0f)),
      t16::Vertex(t16::Vector3f(  1.0f, -1.0f,  0.57730f ), t16::Vector2f(1.0f, 0.0f)),
      t16::Vertex(t16::Vector3f(  0.0f,  1.0f,  0.00000f ), t16::Vector2f(0.5f, 1.0f))
  };

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


static void CreateIndexBuffer()
{
  unsigned int Indices[] = { 0, 3, 1,
                             1, 3, 2,
                             2, 3, 0,
                             1, 2, 0 };

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
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
  Lengths[0] = strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, nullptr, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    exit(1);
  }

  glAttachShader(ShaderProgram, ShaderObj);
}


static void CompileShaders()
{
  const GLuint ShaderProgram = glCreateProgram();

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
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glUseProgram(ShaderProgram);

  gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
  assert(gWVPLocation != 0xFFFFFFFF);
  gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
  assert(gSampler != 0xFFFFFFFF);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Tutorial 16");
  glutGameModeString("1280x1024@32");
  glutEnterGameMode();

  InitializeGlutCallbacks();

  pGameCamera = new t16::Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

  // Must be done after glut is initialized!
  const GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  CreateVertexBuffer();
  CreateIndexBuffer();

  CompileShaders();

  glUniform1i(gSampler, 0);

  pTexture = new t16::Texture(GL_TEXTURE_2D, "../Content/test.png");

  if (!pTexture->Load()) {
    return 1;
  }

  glutMainLoop();

  return 0;
}
