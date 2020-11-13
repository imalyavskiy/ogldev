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
*/

#ifndef MATH_3D_H
#define	MATH_3D_H

template<typename T_>
struct Vector2
{
    T_ x;
    T_ y;

    Vector2()
    {
    }

    Vector2(T_ _x, T_ _y)
        : x(_x)
        , y(_y)
    {
    }
};

template<typename T_>
struct Vector3
{
    T_ x;
    T_ y;
    T_ z;

    Vector3()
    {
    }

    Vector3(T_ _x, T_ _y, T_ _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {
    }
};

template<typename T_>
struct Vector4
{
    T_ x;
    T_ y;
    T_ z;
    T_ w;

    Vector4()
    {
    }

    Vector4(T_ _x, T_ _y, T_ _z, T_ _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {
    }
};

template<typename T_, int rows, int cols>
struct Matrix4
{
    T_ m[rows][cols];
};

typedef Vector2<GLfloat> Vector2f;
typedef Vector2<GLdouble> Vector2d;
typedef Vector2<GLint> Vector2i;
typedef Vector2<GLuint> Vector2ui;
typedef Vector3<GLfloat> Vector3f;
typedef Vector3<GLdouble> Vector3d;
typedef Vector3<GLint> Vector3i;
typedef Vector3<GLuint> Vector3ui;
typedef Vector4<GLfloat> Vector4f;
typedef Vector4<GLdouble> Vector4d;
typedef Vector4<GLint> Vector4i;
typedef Vector4<GLuint> Vector4ui;
typedef Matrix4<GLfloat,4,4> Matrix4f;
typedef Matrix4<GLdouble,4,4> Matrix4d;
typedef Matrix4<GLint,4,4> Matrix4i;
typedef Matrix4<GLuint,4,4> Matrix4ui;


#endif	/* MATH_3D_H */

