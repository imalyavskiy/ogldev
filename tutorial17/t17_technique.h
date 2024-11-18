#ifndef TEXHNIQUE_H
#define TEXHNIQUE_H

#include <GL/glew.h>
#include <list>
namespace t17
{
  class Technique
  {
  public:
    Technique();
    virtual ~Technique();

    virtual bool Init();
    void Enable();

  protected:
    bool AddShader(GLenum shaderType, const char* pShaderText);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

  private:
    GLuint m_shaderProg;
    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
  };
}
#endif /* TEXHNIQUE_H */
