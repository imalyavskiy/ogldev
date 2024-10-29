#include <cstdio>
#include <cstring>

#include "t20_technique.h"
namespace t20
{
  Technique::Technique(){
    m_shaderProgram = 0;
  }

  Technique::~Technique(){
    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++){
      glDeleteShader(*it);
    }

    if (m_shaderProgram != 0){
      glDeleteProgram(m_shaderProgram);
      m_shaderProgram = 0;
    }
  }

  bool Technique::Init(){
    m_shaderProgram = glCreateProgram();

    if (m_shaderProgram == 0){
      fprintf(stderr, "Error creating shader program\n");
      return false;
    }

    return true;
  }

  bool Technique::AddShader(GLenum ShaderType, const char* pShaderText){
    const GLuint shaderObj = glCreateShader(ShaderType);

    if (shaderObj == 0){
      fprintf(stderr, "Error creating shader type %d\n", ShaderType);
      return false;
    }

    m_shaderObjList.push_back(shaderObj);

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(shaderObj, 1, p, Lengths);

    glCompileShader(shaderObj);

    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if (!success){
      GLchar InfoLog[1024];
      glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
      fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
      return false;
    }

    glAttachShader(m_shaderProgram, shaderObj);

    return true;
  }

  bool Technique::Finalize(){
    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(m_shaderProgram);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0){
      glGetProgramInfoLog(m_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
      fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
      return false;
    }

    glValidateProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (Success == 0){
      glGetProgramInfoLog(m_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
      fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
      return false;
    }

    for (auto it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++){
      glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return true;
  }

  void Technique::Enable(){
    glUseProgram(m_shaderProgram);
  }

  GLint Technique::GetUniformLocation(const char* pUniformName){
    const GLint location = glGetUniformLocation(m_shaderProgram, pUniformName);

    if (location == 0xFFFFFFFF){
      fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return location;
  }
}
