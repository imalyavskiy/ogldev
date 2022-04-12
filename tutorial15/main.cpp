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

    Tutorial 15 - Camera Control - Part 2
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Идентификатор объекта буфера вершин
GLuint VBO;

// Идентификатор объекта буфера индексов
GLuint IBO;

// Идентификатор связываемый с uniform переменной gWVP вершинного шейдера,
// всемирной матрицей преобразований. Всемирная она потому, что всё что мы делаем с объектом, 
// это изменение его позиции в место, которое мы указываем относительно коорди-
// натной системы внутри нашего виртуального 'мира'.
GLuint gWVPLocation;

// Эземпляр камеры
Camera* pGameCamera = nullptr;

// Исходный код вершинного шейдера
static const char* pVS =
// использовать GLSL версиию 3.3
"#version 330                                                                        \n"\
// Вершина содержит атрибут, являющийся вектором из 3 элементов типа float, который будет иметь имя 'Position' внутри шейдера
// "layout (location = 0)" создаёт связь между названием атрибута и атрибутом в буфере
"layout (location = 0) in vec3 Position;                                             \n"\
// Всемирная матрица преобразований в шейдере. Доступна из вне шейдерного программного объекта, т.к. uniform.
//		This means that the value does not change between multiple executions of a shader during 
//		the rendering of a primitive (ie: during a glDraw* call). These values are set by the user 
//		from the OpenGL API. "https://www.khronos.org/opengl/wiki/Uniform_(GLSL)"
"uniform mat4 gWVP;                                                                  \n"\
// Переменная передающая данные следующему этапу графического конвейера.
// Обязательно д.б. глобальной и помечена как out
"out vec4 Color;                                                                     \n"\
// Точка входа в шейдер
"void main()                                                                         \n"\
"{                                                                                   \n"\
// Изменяем координаты вершины и записываем результат в "vec4 gl_Position"
//		"gl_Position" - втроенная переменная, содержащая вершинные координаты (X, Y, Z и W компоненты)
//	Сначала дополняем vec3 Position до vec4 - "vec4(Position, 1.0)", что бы согласовать
//	размерность вектора и матрицы. После чего умножаем ветктор на матрицу слева,
//	т.е. матрицу умножаем на вектор и получаем новые кординаты точки.
"    gl_Position = gWVP * vec4(Position, 1.0);                                       \n"\
// Для преобразования в цвет ограничиваем значения x,y,z позиции отрезком от [0.0, 1.0]
// поскольку цвет меняетя именно в этом интервале значений, а координаты у нас в интервале
// [-1, 1]. То что меньше 0 - без clamp бует чёрным цвет
"    vec3 clamped = clamp(Position, 0.0, 1.0);                                       \n"\
// Дополняем vec3 единицей, собраем в  vec4 и передаём на выход
"    Color = vec4(clamped, 1.0);                                                     \n"\
"}                                                                                   \n";

// Исходный код фрагментного шейдера
static const char* pFS =
// использовать GLSL версию 3.3
"#version 330                                                                        \n"\
// Одноименная переменная принимающая данные от предыдущего этапа конвейера.
// Так же глобальная, но помеченная in.
"in vec4 Color;                                                                      \n"\
// Выходной параметр vec4(R,G,B,A)
"out vec4 FragColor;                                                                 \n"\
// Точка входа в шейдер
"void main()                                                                         \n"\
"{                                                                                   \n"\
// Транслируем цвет полученный от пиксельного шейдера дальше.
"    FragColor = Color;                                                              \n"\
"}                                                                                   \n";

// Функция заполнения буфера кадра
static void RenderSceneCB()
{
	// TODO: comment
    pGameCamera->OnRender();

	// Очищаем буфера
    glClear(
			GL_COLOR_BUFFER_BIT|	// буфер кадра
			GL_DEPTH_BUFFER_BIT		// буфер глубины
	);

	// Включаем тест глубины при отрисовке треугольников
	glEnable(GL_DEPTH_TEST);

	// Аргумент функции(sin) расчёта коэффициента преобразования
    static float Scale = 0.0f;

	// Приращение
    Scale += 0.05f;

	// Задание параметров преобразований
    Pipeline pipeline;
	
	// Задаём параметры поворота
    pipeline.Rotate(0.0f, Scale, 0.0f);

	// Задаём параметры сдвига
    pipeline.WorldPos(0.0f, 0.0f, 3.0f);

	// Установка параметров камеры в матричный конвейер
    pipeline.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());

	// Задаём параметры перспективной проекции
    pipeline.SetPerspectiveProj( 60.0f			// угол обзора
							   , WINDOW_WIDTH	// ширина viewport'a
							   , WINDOW_HEIGHT	// высота viewport'a
							   , 1.0f			// z-координата ближней отсекающей плоскости
							   , 100.0f			// z-координата дальней отсекающей плоскости
							   );

	// Вычисление финальной матрицы преобразований
	const Matrix4f& wvp = pipeline.GetTrans();

	// Установка матрицы преобразований в переменную gWVP вершинного 
	// шейдера через её идентификатор хранящийся в gWVPLocation
    glUniformMatrix4fv( gWVPLocation	// Идентификатор uniform переменной
					  , 1				// Количество элементов, в данном случае матриц 4х4 из float'ов
					  , GL_TRUE			// Копируемая матрица - транспонированная
					  , (const GLfloat*)&wvp	// указатель на начало блока данных матрицы
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
   	
	// Подключаем буфер индексов, как буфер индексов буфера атрибутов(координат) вершин(GL_ELEMENT_ARRAY_BUFFER)
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER	// Наименование точки подключения
				, IBO 						// ID буфера
				);

	// Используем glDrawElements вместо glDrawArrays
    glDrawElements( GL_TRIANGLES		// тип примитива который рисуем
				  , 48 * 3				// какое количество индексов в индексном буфере используем 48 треугольников по 3 вершинных индекса на каждый
				  , GL_UNSIGNED_INT		// тип элемента(говорит о количестве байт на элемент)
				  , 0					// смещение в байтах от начала буфера индексов до до позиции первого индекса для обработки
				  );

	// Отключаем доступ к атирибутам вершины 0
    glDisableVertexAttribArray(0);

	// меняем местами буфера кадров(у нас их два, мы включали двойную буфферизацию).
    glutSwapBuffers();
}

// callback для получения событий клавиатуры
static void SpecialKeyboardCB(int Key, int x, int y)
{
	// Переправляем значение нажатой клавиши в камеру
    pGameCamera->OnKeyboard(Key);
}

// TODO: comment
static void KeyboardCB(unsigned char Key, int x, int y)
{
    switch (Key) {
        case 'q':
            exit(0);
    }
}

// TODO: comment
static void PassiveMouseCB(int x, int y)
{
	// TODO: comment
    pGameCamera->OnMouse(x, y);
}

// Функция регистрации callback'ов
static void InitializeGlutCallbacks()
{
	// Устанавливаем callback функцию заполнения буфера кадра
    glutDisplayFunc(RenderSceneCB);

	// Указываем функцию рендера в качестве функции простоя
	//	- функция простоя вызывается когда нет событий пользовательсткого ввода
    glutIdleFunc(RenderSceneCB);

	// Регистрируем callback для получения событий клавиатуры, 
	// а именно нажатии "специальных" кнопок, это:
	// F1-F12, Page Up/Down, Home, End, Insert и кнопки стрелок.
	// Что бы поймать обычные кнопки нужно использовать glutKeyboardFunc.
    glutSpecialFunc(SpecialKeyboardCB);

	// TODO: comment
    glutPassiveMotionFunc(PassiveMouseCB);

	// TODO: comment
    glutKeyboardFunc(KeyboardCB);
}

// Функция создания буфера вершин
static void CreateVertexBuffer()
{

	// Буфер координат вершин модели
	Vector3f Vertices[] = 
	{
		/* 0*/	{ -0.2500f, -0.2500f, -0.2500f },	/* 1*/	{ -0.0000f, -0.1001f, -0.1001f },	/* 2*/	{  0.2500f, -0.2500f, -0.2500f },
		/* 3*/	{ -0.1001f, -0.0000f, -0.1001f },	/* 4*/	{  0.0000f,  0.0000f, -0.3247f },	/* 5*/	{  0.1001f, -0.0000f, -0.1001f },
		/* 6*/	{ -0.2500f,  0.2500f, -0.2500f },	/* 7*/	{ -0.0000f,  0.1001f, -0.1001f },	/* 8*/	{  0.2500f,  0.2500f, -0.2500f },
		/* 9*/	{ -0.2500f, -0.2500f,  0.2500f },	/*10*/	{ -0.0000f, -0.1001f,  0.1001f },	/*11*/	{  0.2500f, -0.2500f,  0.2500f },
		/*12*/	{ -0.1001f, -0.0000f,  0.1001f },	/*13*/	{  0.0000f,  0.0000f,  0.3248f },	/*14*/	{  0.1001f, -0.0000f,  0.1001f },
		/*15*/	{ -0.2500f,  0.2500f,  0.2500f },	/*16*/	{ -0.0000f,  0.1001f,  0.1001f },	/*17*/	{  0.2500f,  0.2500f,  0.2500f },
		/*18*/	{ -0.1001f, -0.1001f,  0.0000f },	/*19*/	{  0.0000f, -0.3248f, -0.0000f },	/*20*/	{  0.1001f, -0.1001f,  0.0000f },
		/*21*/	{  0.3248f,  0.0000f, -0.0000f },	/*22*/	{  0.1001f,  0.1001f,  0.0000f },	/*23*/	{  0.0000f,  0.3248f, -0.0000f },
		/*24*/	{ -0.1001f,  0.1001f,  0.0000f },	/*25*/	{ -0.3248f,  0.0000f, -0.0000f },	
	};


 	// Запрашиваем у OpenGL создание объекта буфера
 	glGenBuffers( 1					// количество буферов
				, &VBO );			// блок памяти GLuint'ов того же количества

	// Подключаем буфер, как буфер атрибутов(координат) вершин(GL_ARRAY_BUFFER)
	glBindBuffer( GL_ARRAY_BUFFER	// Наименование точки подключения
				, VBO				// ID буфера
				);

	// Копируем данные из RAM буфера вершин в VRAM буфер вершин
	glBufferData( GL_ARRAY_BUFFER	// Тип буфера
				, sizeof(Vertices)  // Размер массива с вершинами
				, Vertices          // адрес массива с вершинами
				, GL_STATIC_DRAW    // данные буфера статичны(неизменны)
				);
}

// Функция создания буфера индексов
static void CreateIndexBuffer()
{
	// Буфер индексов вершин треугольников нашей модели
    unsigned int Indices[] = 
	{
		/* 0*/  0,  3,  1,	/* 1*/  1,  5,  2,	/* 2*/  3,  6,  7,	/* 3*/  7,  8,  5,	/* 4*/ 10, 13, 12,	/* 5*/ 10, 14, 13,	/* 6*/ 12, 13, 16,	/* 7*/ 13, 14, 16,
		/* 8*/  0,  1, 18,	/* 9*/  1,  2, 20,	/*10*/ 18, 10,  9,	/*11*/ 20, 11, 10,	/*12*/  2,  5, 20,	/*13*/  5,  8, 22,	/*14*/ 20, 14, 11,	/*15*/ 22, 17, 14,
		/*16*/  8,  7, 22,	/*17*/  7,  6, 24,	/*18*/ 22, 16, 17,	/*19*/ 24, 15, 16,	/*20*/  3, 25, 24,	/*21*/  3, 18, 25,	/*22*/ 24, 25, 12,	/*23*/ 25, 18, 12,
		/*24*/ 12, 15, 24,	/*25*/ 24,  6,  3,	/*26*/  3,  0, 18,	/*27*/ 18,  9, 12,	/*28*/ 18, 19, 10,	/*29*/ 10, 19, 20,	/*30*/ 20, 19,  1,	/*31*/  1, 19, 18,
		/*32*/ 12,  9, 10,	/*33*/ 10, 11, 14,	/*34*/ 14, 17, 16,	/*35*/ 16, 15, 12,	/*36*/ 20, 21, 14,	/*37*/ 14, 21, 22,	/*38*/ 22, 21,  5,	/*39*/  5, 21, 20,
		/*40*/ 22, 23, 16,	/*41*/ 16, 23, 24,	/*42*/ 24, 23,  7,	/*43*/  7, 23, 22,	/*44*/  1,  4,  5,	/*45*/  5,  4,  7,	/*46*/  7,  4,  3,	/*47*/  3,  4,  1,
	};
    
	// Запрашиваем у OpenGL создание объекта буфера
    glGenBuffers( 1							// количество буферов
				, &IBO);					// блок памяти GLuint'ов того же количества

	// Подключаем буфер, как буфер индексов буфера атрибутов(координат) вершин(GL_ELEMENT_ARRAY_BUFFER)
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER	// Наименование точки подключения
				, IBO 						// ID буфера
				);

	// Копируем данные из RAM буфера индексов в VRAM буфер индексов
    glBufferData( GL_ELEMENT_ARRAY_BUFFER	// Тип буфера
				, sizeof(Indices)			// Размер массива с вершинами
				, Indices					// адрес массива с вершинами
				, GL_STATIC_DRAW			// данные буфера статичны(неизменны)
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

	const GLchar* p[]	= { pShaderText };
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

	// Получаем идентификатор uniform переменной gWVP вершинного шейдера
    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");

	// Ошибка может возникнуть, если неправильно указано имя uniform переменной
	// или она была удалена компилятором во время оптимизации шейдера поскольку
	// не использовалась
    assert(gWVPLocation != 0xFFFFFFFF);
}

// Собственно точка входа
int main(int argc, char** argv)
{
	// Инициализация GLUT
    glutInit(&argc, argv);
	
	// Включаем отрисовки и вывода на экран
    glutInitDisplayMode(
		GLUT_DOUBLE|	// Двойную буферизацию
		GLUT_RGBA|		// Цветовое пространство
		GLUT_DEPTH		// Подключение буфера глубины(необходим для работы теста глубины)
	);	

	// Задаём размер окна
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Задаём позицию верхнего левого угла нашего окна
    glutInitWindowPosition(100, 100);

	// Создаём окно с указанным именем
    glutCreateWindow("Tutorial 15");

	// TODO: comment
    glutGameModeString("1280x1024@32");

	// TODO: comment
    glutEnterGameMode();

	// Регистрируем callback'и
    InitializeGlutCallbacks();

	// TODO: comment
    pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

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

	// Создаём буфер индексов
    CreateIndexBuffer();

	// Задаём шейдеры
    CompileShaders();

	// Передаём управление главному циклу GLUT
    glutMainLoop();

    return 0;
}