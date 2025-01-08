#include <string>
#include <fstream>

#include <windows.h>

#include "t36_util.h"

bool ReadFile(const char* pFileName, std::string& outFile)
{
  std::ifstream f(pFileName);

  bool ret = false;

  if (f.is_open()) {
    std::string line;
    while (std::getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }

    f.close();

    ret = true;
  }
  else {
    REPORT_FILE_ERROR(pFileName);
  }

  return ret;
}

void OgldevError(const char* pFileName, uint32_t line, const char* pError)
{
#ifdef WIN32
  char msg[1000];
  _snprintf_s(msg, sizeof(msg), "%s:%d: %s", pFileName, line, pError);
  MessageBoxA(NULL, msg, NULL, 0);
#else
  fprintf(stderr, "%s:%d: %s\n", pFileName, line, pError);
#endif    
}


void OgldevFileError(const char* pFileName, uint32_t line, const char* pFileError)
{
#ifdef WIN32
  char msg[1000];
  _snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError);
  MessageBoxA(NULL, msg, NULL, 0);
#else
  fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
#endif    
}
