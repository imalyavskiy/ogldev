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

// Идентификатор буфера вершин
GLuint VBO;

// Функция заполнения буфера кадра
static void RenderSceneCB()
{
	// Очищаем буфер кадра
    glClear(GL_COLOR_BUFFER_BIT);

	// Координаты вершин, используемые в буфере, рассматриваются как атрибут вершины с индексом 0 
	// поэтому разрешаем индексацию атрибутов вершины 0
    glEnableVertexAttribArray(0);
	
	// Подключаем буфер, как буфер вершин(GL_ARRAY_BUFFER)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
	// Говорим конвейеру как воспринимать данные внутри буфера
    glVertexAttribPointer( 0		// Индекс атрибута с которого начать чтение
						 , 3		// Количество компонент в атрибуте
						 , GL_FLOAT // Тип компонент
						 , GL_FALSE // Режим нормализации - выкл.
						 , 0		// Шаг - число байт между экземплярами атрибута
						 , 0		// Смещение первого элемента в буфере в байтах.
						 );

	// Отрисовать данные буфера вершин
    glDrawArrays( GL_TRIANGLES	// Тип отрисовки - треугольники(каждая 3ка вершин - треугольник).
				, 0				// Индекс первого элемента буфера вершин.
				, 3				// Количество элементов для отрисовки.
				);

	// Отключаем доступ к атирибутам вершины 0
    glDisableVertexAttribArray(0);

	// меняем местами буфера кадров(у нас их два, мы включали двойную буфферизацию).
    glutSwapBuffers();
}

// Функция регистрации callback'ов
static void InitializeGlutCallbacks()
{
	// Устанавливаем callback функцию заполнения буфера кадра
    glutDisplayFunc(RenderSceneCB);
}

// Функция создания буферов вершин
static void CreateVertexBuffer()
{
	// Массив из трёх вершины
    Vector3f Vertices[3];

	// Задание координат вершин
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f( 1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f( 0.0f,  1.0f, 0.0f);

	// Запрашиваем у OpenGL буфер
 	glGenBuffers( 1					// количество буферов
				, &VBO );			// блок памяти GLuint'ов того же количества
	
	// Подключаем буфер, как буфер вершин(GL_ARRAY_BUFFER)
	glBindBuffer( GL_ARRAY_BUFFER	// Наименование точки подключения
				, VBO				// ID буфера
				);

	// Копируем данные из массива вершин в буфера вершинами OpenGL
	// (вероятно здесь происходит перегрузка памяти из RAM в VRAM)
	glBufferData( GL_ARRAY_BUFFER	// Тип буфера
				, sizeof(Vertices)  // Размер массива с вершинами
				, Vertices          // адрес массива с вершинами
				, GL_STATIC_DRAW    // данные буфера статичны(неизменны)
	);
}

// Собственно точка входа
int main(int argc, char** argv)
{
	// Инициализация GLUT
    glutInit(&argc, argv);
	
	// Включаем режим двойной буферизации(GLUT_DOUBLE) и режим вывода цветов(GLUT_RGBA, не индексированный режим цветопередачи).
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	
	// Задаём размер окна
    glutInitWindowSize(1024, 768);
	
	// Задаём позицию верхнего левого угла нашего окна
    glutInitWindowPosition(100, 100);

	// Создаём окно с указанным именем
	glutCreateWindow("Tutorial 03");

	// Регистрируем callback'и
	InitializeGlutCallbacks();

    // Инициализируем GLEW.
	// ВНИМАНИЕ: Этот шаг инициализации производится только после инициализации GLUT.
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

	// Задаём цвет заливки буфера кадра
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Создаём буфер вершин
    CreateVertexBuffer();

	// Передаём управление главному циклу GLUT
    glutMainLoop();

    return 0;
}


