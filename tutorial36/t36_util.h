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
*/

#ifndef UTIL_H
#define	UTIL_H

#include <string>

bool ReadFile(const char* fileName, std::string& outFile);

void OgldevError(const char* pFileName, uint32_t line, const char* pError);
void OgldevFileError(const char* pFileName, uint32_t line, const char* pFileError);

#define REPORT_ERROR(Error) OgldevError(__FILE__, __LINE__, Error);
#define REPORT_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifdef WIN32
#define SNPRINTF _snprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#else
#define SNPRINTF snprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF
#define INVALID_OGL_VALUE 0xFFFFFFFF

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError()                                                          \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

#endif	/* UTIL_H */

