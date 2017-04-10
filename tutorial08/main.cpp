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

    Tutorial 08 - Scaling Transformation
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"

// Идентификатор объекта буфера вершин
GLuint VBO;

// Идентификатор связываемый с uniform переменной gWorld вершинного шейдера,
// всемирной матрицей. Всемирная она потому, что всё что мы делаем с объектом, 
// это изменение его позиции в место, которое мы указываем относительно коорди-
// натной системы внутри нашего виртуального 'мира'.
GLuint gWorldLocation;


// Исходный код вершинного шейдера
static const char* pVS =
// использовать GLSL версиию 3.3
"#version 330                                                                        \n"\
// Вершина содержит атрибут, являющийся вектором из 3 элементов типа float, который будет иметь имя 'Position' внутри шейдера
// "layout (location = 0)" создаёт связь между названием атрибута и атрибутом в буфере
"layout (location = 0) in vec3 Position;                                             \n"\
// Всемирная матрица в шейдере. Доступна из вне шейдерного программного объекта, т.к. uniform.
//		This means that the value does not change between multiple executions of a shader during 
//		the rendering of a primitive (ie: during a glDraw* call). These values are set by the user 
//		from the OpenGL API. "https://www.khronos.org/opengl/wiki/Uniform_(GLSL)"
"uniform mat4 gWorld;                                                                \n"\
// Точка входа в шейдер
"void main()                                                                         \n"\
"{                                                                                   \n"\
// Изменяем координаты вершины и записываем результат в "vec4 gl_Position"
//		"gl_Position" - втроенная переменная, содержащая вершинные координаты (X, Y, Z и W компоненты)
//	Сначала дополняем vec3 Position до vec4 - "vec4(Position, 1.0)", что бы согласовать
//	размерность вектора и матрицы. После чего умножаем ветктор на матрицу слева,
//	т.е. матрицу умножаем на вектор и получаем новые корлинаты токи.
"    gl_Position = gWorld * vec4(Position, 1.0);                                     \n"\
"}                                                                                   \n";

// Исходный код фрагментного шейдера
static const char* pFS =
// использовать GLSL версию 3.3
"#version 330                                                                        \n"\
// Выходной параметр vec4(R,G,B,A)
"out vec4 FragColor;                                                                 \n"\
// Точка входа в шейдер
"void main()                                                                         \n"\
"{                                                                                   \n"\
// Задаём цвет, в данном случае красный
// 0.0 <= R,G,B,A <= 1.0
"    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                           \n"\
"}                                                                                   \n";

// Функция заполнения буфера кадра
static void RenderSceneCB()
{
	// Очищаем буфер кадра
    glClear(GL_COLOR_BUFFER_BIT);

	// Аргумент функции(sin) расчёта коэффициента преобразования
    static float Scale = 0.0f;

	// Приращение
    Scale += 0.005f;

	// Матрица float'ов 4х4
	// Наша всемирная матрица
    Matrix4f World;

	// Каноническая матрица масштабирования меет вид:
	// A 0 0 0
	// 0 B 0 0
	// 0 0 C 0
	// 0 0 0 1

	// Задаём матирцу масштабирования. Поскольку коэффициенты стоят только на главной лиагонали, то
	// транспонированная матрица и обычная - тождественны
	World.m[0][0] = sinf(Scale) ; World.m[0][1] = 0.0f       ; World.m[0][2] = 0.0f;        World.m[0][3] = 0.0f;
    World.m[1][0] = 0.0f        ; World.m[1][1] = cosf(Scale); World.m[1][2] = 0.0f;        World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f;       ; World.m[2][1] = 0.0f;      ; World.m[2][2] = sinf(Scale); World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;       ; World.m[3][1] = 0.0f;      ; World.m[3][2] = 0.0f;        World.m[3][3] = 1.0f;

	// Установка матрицы сдвига в переменную gWorld вершинного 
	// шейдера через её идентификатор хранящийся в gWorldLocation
    glUniformMatrix4fv( gWorldLocation	// Идентификатор uniform переменной
					  , 1				// Количество элементов, в данном случае матриц 4х4 из float'ов
					  , GL_TRUE			// Копируемая матрица - транспонированная
					  , &World.m[0][0]	// указатель на начало длока данных матрицы
					  );

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

	// Указываем функцию рендера в качестве функции простоя
	//	- функция простоя вызывается когда нет событий пользовательсткого ввода
    glutIdleFunc(RenderSceneCB);
}

// Функция создания буферов вершин
static void CreateVertexBuffer()
{
	// Массив из трёх вершины
    Vector3f Vertices[3];

	// Задание координат вершин
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	// Запрашиваем у OpenGL создание объекта буфера
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

// Функция добавления шейдера в шейдерную программу
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// Запросим у OpenGL создать объект шейдера
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
		// Что-то пошло не так
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

	const GLchar* p[] = { pShaderText };
    GLint Lengths[]   = { (GLint)strlen(pShaderText) };
    
	// Заполним объект шейдера ресурсами
    glShaderSource( ShaderObj // Идентификатор
				  , 1		  // Кол-во пар код/длина в нижеследующих массивах
				  , p		  // Код шейдеров
				  , Lengths	  // Размер кода шедеров
				  ); 
	
	// Запустим процесс компиляции
    glCompileShader(ShaderObj);
    
	GLint success = 0;

	// Запросим у объекта шейдера параметр - результат компиляции
    glGetShaderiv( ShaderObj			// Идентификатор объекта шейдера
				 , GL_COMPILE_STATUS	// Идентификатор запрашиваемого параметра
				 , &success				// Куда положить значение параметра
				 );

    if (!success) {
		// Если были ошибки коде в шейдера
        GLchar InfoLog[1024];

		// Запросим и выведем лог
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

	// Всё прошло отлично. 
	// Добавим скомпилированный шейдер в объект программы
    glAttachShader(ShaderProgram, ShaderObj);
}

// Функция задания шейдеров
static void CompileShaders()
{
	// Запросим у OpenGL создать объект шейдероной программы
    GLuint ShaderProgram = glCreateProgram();

	// Проверяем что всё идёт по плану
    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

	// Добавляем в программу исходный код вершинного шейдера 
    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);

	// Добавляем в программу исходный код фрагментного шейдера 
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

	// Линкуем программу
    glLinkProgram(ShaderProgram);

	// Запрашиваем результат и лог
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		// Что-то пошло не так
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

	// Проверяем сможет ли программа запуститься с текущим состоянием конвейера.
    glValidateProgram(ShaderProgram);

	// Запрашиваем результат и лог
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
		// Что-то пошло не так
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

	// Подключаем программу к графическому конвейеру
    glUseProgram(ShaderProgram);

	// Получаем идентификатор uniform переменной gWorld вершинного шейдера
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");

	// Ошибка может возникнуть, если неправильно указано имя uniform переменной
	// или она была удалена компилятором во время оптимизации шейдера поскольку
	// не использовалась
    assert(gWorldLocation != 0xFFFFFFFF);
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
    glutCreateWindow("Tutorial 08");

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

	// Задаём шейдеры
    CompileShaders();

	// Передаём управление главному циклу GLUT
    glutMainLoop();

    return 0;
}