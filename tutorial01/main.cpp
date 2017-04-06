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

    Tutorial 01 - Create a window
*/

#include <GL/freeglut.h>

// Функция заполнения буфера кадра
static void RenderSceneCB()
{
	// Очищаем буфер кадра
    glClear(GL_COLOR_BUFFER_BIT);
	
	/*
	TODO: Производим отрисовку
	*/
	
	// меняем местами буфера кадров(у нас их два, мы включали двойную буфферизацию).
    glutSwapBuffers();
}

// Функция регистрации callback'ов
static void InitializeGlutCallbacks()
{
	// Устанавливаем callback функцию заполнения буфера кадра
    glutDisplayFunc(RenderSceneCB);
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
    glutCreateWindow("Tutorial 01");

	// Регистрируем callback'и
    InitializeGlutCallbacks();

	// Задаём цвет заливки буфера кадра
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Передаём управление главному циклу GLUT
    glutMainLoop();
    
    return 0;
}