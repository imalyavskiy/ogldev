/*

  Copyright 2014 Etay Meiri

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

*/

#ifndef _OGLDEV_UTIL_
#define _OGLDEV_UTIL_

#include <iostream>
#include <fstream>
#include <format>
#include <Windows.h>

#include "t37_util.h"

namespace t37 {
  bool ReadFile(const char* pFileName, std::string& outFile)
  {
    std::ifstream f(pFileName);

    bool ret = false;

    if (f.is_open()) {
      std::string line;
      while (getline(f, line)) {
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

  void ReportError(const char* pFileName, uint32_t line, const char* pError)
  {
    const std::string msg = std::format("{}:{}: {}", pFileName, line, pError);
    MessageBoxA(nullptr, msg.c_str(), "Error", MB_ICONERROR | MB_OK);
  }


  void ReportFileError(const char* pFileName, uint32_t line, const char* pFileError)
  {
    const std::string msg = std::format("{}:{}: unable to open file `{}`", pFileName, line, pFileError);
    MessageBoxA(nullptr, msg.c_str(), "File error", MB_ICONERROR | MB_OK);
  }

  long long GetCurrentTimeMillis()
  {
    return GetTickCount();
  }
}
#endif /* _OGLDEV_UTIL_ */
