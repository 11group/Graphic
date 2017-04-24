#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define GLUT_ACTIVE_SHIFT 1
#include <stdlib.h>
#include <glut.h>
#include <vector>
#include <math.h>
#include "glaux.h"
#include <gl\gl.h>

#pragma comment(lib,"glaux.lib")
using namespace std;

#pragma region Global constants
GLint Width = 1268, Height = 680;

int n = 0, k = 0;
int mode = 0;
bool modeGrid = 0; // 0 - нет, 1 - включена
bool smoothMode = 0;
int colorMod = 0; // 0 - нет, 1 - and, 2 - not and
int gridSize = 6;					//размер сетки
int smoothSize;				//размер сетки сглаживания


enum keys { Empty, KeyR, KeyG, KeyB, KeyW, KeyA, KeyS, KeyD, KeyU, KeyI, KeyZ, KeyX, KeyN, KeyJ, KeyK, KeyC, Key1, Key2};


/* задание контейнера вершин */
struct type_point
{
	GLint x, y;
	type_point(GLint _x, GLint _y) { x = _x; y = _y; }
	type_point() {}
};

/* задание цвета пикселей */
struct colour
{
	GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
};

/* задание контейнера характеристик точек */
struct Characters
{
	GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
	GLubyte PointSize = 5;
	GLubyte LineSize = 3;
	GLubyte mode = 0;
	type_point O;
	int r = 0;
	int angle = 0;
	float scaleX = 1.;
	float scaleY = 1.;
	int texture = -1;
	bool circle = 0;
	Characters()
	{
		O = type_point(0, 0);
	}

};

/* задание контейнера "шестиугольник" */
struct Hexagon
{
	GLint r, ob, ab;
	int x = 1, y = 1;
	type_point O, A, B;

	Hexagon()
	{
		O = type_point();
		A = type_point();
		B = type_point();
	}
};

Hexagon H;
vector <vector <type_point>> Points;
vector <Characters> Numb;
vector <type_point> TextureCoordinates;
GLuint textures[7];

/* загрузка текстур */
void Textures()
{
	glGenTextures(7, textures);
	AUX_RGBImageRec *image1 = auxDIBImageLoad(L"1.bmp");
	AUX_RGBImageRec *image2 = auxDIBImageLoad(L"2.bmp");
	AUX_RGBImageRec *image3 = auxDIBImageLoad(L"3.bmp");
	AUX_RGBImageRec *image4 = auxDIBImageLoad(L"4.bmp");
	AUX_RGBImageRec *image5 = auxDIBImageLoad(L"5.bmp");
	AUX_RGBImageRec *image6 = auxDIBImageLoad(L"6.bmp");
	AUX_RGBImageRec *image7 = auxDIBImageLoad(L"7.bmp");

	glBindTexture(GL_TEXTURE_2D, textures[0]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
	glBindTexture(GL_TEXTURE_2D, textures[1]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image2->sizeX, image2->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image2->data);
	glBindTexture(GL_TEXTURE_2D, textures[2]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image3->sizeX, image3->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image3->data);
	glBindTexture(GL_TEXTURE_2D, textures[3]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image4->sizeX, image4->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image4->data);
	glBindTexture(GL_TEXTURE_2D, textures[4]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image5->sizeX, image5->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image5->data);
	glBindTexture(GL_TEXTURE_2D, textures[5]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image6->sizeX, image6->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image6->data);
	glBindTexture(GL_TEXTURE_2D, textures[6]); //выбрать текущую текстуру
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image7->sizeX, image7->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image7->data);
}

/* расчет текстурных координат*/
void setTextureCoordinates()
{
	double A = M_PI / 6;
	type_point coord = type_point(1, 0.5);
	TextureCoordinates.push_back(coord);
	coord = type_point(0.75, 0.5 + 0.5*cos(A));
	TextureCoordinates.push_back(coord);
	coord = type_point(0.25, 0.5 + 0.5*cos(A));
	TextureCoordinates.push_back(coord);
	coord = type_point(0, 0.5);
	TextureCoordinates.push_back(coord);
	coord = type_point(0.25, 0.5 - 0.5*cos(A));
	TextureCoordinates.push_back(coord);
	coord = type_point(0.75, 0.5 - 0.5*cos(A));
	TextureCoordinates.push_back(coord);
}

/* вычисление координат шестиугольника */
void Build_Points(type_point O, type_point A)
{
	glPushMatrix();
	H.ob = H.A.x - H.O.x;
	H.ab = H.A.y - H.O.y;
	H.r = sqrt(pow(H.ob, 2) + pow(H.ab, 2));
	Numb[n].O = H.O;
	glTranslated(H.O.x, H.O.y, 0);
	GLint l = 6;
	for (float j = 0; j < l; j++)
	{
		H.B.x = H.r * cos(j * 2 * M_PI / l);
		H.B.y = H.r * sin(j * 2 * M_PI / l);
		Points[n].push_back(H.B);
	}
	for (int i = 0; i < Points[n].size(); i++)
	{
		Points[n][i].x += H.O.x;
		Points[n][i].y += H.O.y;
	}
	glPopMatrix();
}

/* вычисление координат круга */
void Build_Circle(type_point O, type_point A)
{
	glPushMatrix();
	H.ob = H.A.x - H.O.x;
	H.ab = H.A.y - H.O.y;
	H.r = sqrt(pow(H.ob, 2) + pow(H.ab, 2));
	Numb[n].O = H.O;
	glTranslated(H.O.x, H.O.y, 0);
	GLint l = 360;
	for (float j = 0; j < l; j++)
	{
		H.B.x = H.r * cos(j * 2 * M_PI / l);
		H.B.y = H.r * sin(j * 2 * M_PI / l);
		Points[n].push_back(H.B);
	}
	for (int i = 0; i < Points[n].size(); i++)
	{
		Points[n][i].x += H.O.x;
		Points[n][i].y += H.O.y;
	}
	glPopMatrix();
}

/* координаты искомой точки в координатах сетки */
bool PointIngrid(int x0, int y0, int * xi, int * yj)
{
	bool found = false;
	int i, j;
	for (i = 0; i < Width / gridSize && !found; i++)
		for (j = 0; j < Height / gridSize && !found; j++)
			if ((x0 >= i*gridSize && x0 < (i + 1)*gridSize) && (y0 >= j*gridSize && y0 < (j + 1)*gridSize))
				found = true;
	*xi = i;
	*yj = j;
	return (found);
}
#pragma endregion

#pragma region Display work
/* Функция вывода на экран */
void Display(void)
{
	/* разбиваем сетку по иксу и игрику */
	int nx = Width / gridSize;
	int ny = Height / gridSize;


	glClearColor(240, 255, 240, 1); glClear(GL_COLOR_BUFFER_BIT);

	/* задаём цвета сетки, по умолчанию поставим белые */
	vector <vector<colour>> colours(nx + 1);
	for (int i = 0; i < nx + 1; i++)
	{
		colours[i].resize(ny + 1);
		for (int j = 0; j < ny + 1; j++)
		{
			colours[i][j].ColorR = 240;
			colours[i][j].ColorB = 255;
			colours[i][j].ColorG = 240;
		}
	}
	/* если режим растеризации включен */
	if (modeGrid)
	{
		////////////////////////////////////////////////
		////////////////////////////////////////////////
		////	Здесь могла быть ваша растеризация	////
		////////////////////////////////////////////////
		////////////////////////////////////////////////


		/* отрисовываем пиксели сетки */
		glBegin(GL_QUADS);
		for (int i = 0; i < nx + 1; i++)
		{
			for (int j = 0; j < ny + 1; j++)
			{
				glColor3ub(colours[i][j].ColorR, colours[i][j].ColorG, colours[i][j].ColorB);
				glVertex2i((i - 1) * gridSize, (j - 1) * gridSize);
				glVertex2i(i * gridSize, (j - 1) * gridSize);
				glVertex2i(i * gridSize, j * gridSize);
				glVertex2i((i - 1) * gridSize, j * gridSize);
			}
		}
		glEnd();

		/* отрисовываем саму сетку */
		glLineWidth(0.5);
		glColor3ub(70, 70, 70);
		glBegin(GL_LINES);
		for (int i = 0; i < nx + 1; i++)
		{
			glVertex2i(i * gridSize, 0);
			glVertex2i(i * gridSize, Height);
		}
		for (int j = 0; j < ny + 1; j++)
		{
			glVertex2i(0, j * gridSize);
			glVertex2i(Width, j * gridSize);
		}
		glEnd();
	}
	
	if (colorMod)		//если включен режим смешения цветов
	{
		glEnable(GL_COLOR_LOGIC_OP);
		if (colorMod == 1) glLogicOp(GL_AND);
		if (colorMod == 2) glLogicOp(GL_NAND);
	}
	else glDisable(GL_COLOR_LOGIC_OP);

	for (int j = 0; j < Points.size(); j++)
	{
		/* режим ломаной */
		if (Numb[j].mode == 0)
		{
			if (Numb[j].texture == -1)
				glDisable(GL_TEXTURE_2D);
			glColor3ub(Numb[j].ColorR, Numb[j].ColorG, Numb[j].ColorB);
			glLineWidth(Numb[j].LineSize);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < Points[j].size(); i++)
				glVertex2f(Points[j][i].x, Points[j][i].y);
			glEnd();
		}
		/* режим шестиугольника */
		if (Numb[j].mode == 1)
		{
			glPushMatrix();

			if (Numb[j].texture != -1)
			{
				/* выбрать текстуру */
				glBindTexture(GL_TEXTURE_2D, textures[Numb[j].texture]);
				/* тип наложения текстуры - цвет фигуры не учитывается */
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				/*разрешить текстурирование */
				glEnable(GL_TEXTURE_2D);
			}
			if (Numb[j].texture == -1)
				glDisable(GL_TEXTURE_2D);
			
			// участок с поворотом вокруг центра
			glTranslated(Numb[j].O.x, Numb[j].O.y, 0);
			glScalef(Numb[j].scaleX, Numb[j].scaleY, 1);	//масштабирование вдоль осей
			glRotated(Numb[j].angle, 0, 0, 1.0f);
			glTranslated(-Numb[j].O.x, -Numb[j].O.y, 0);

			glColor3ub(0, 0, 0);
			glLineWidth(Numb[j].LineSize);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < Points[j].size(); i++)
				glVertex2f(Points[j][i].x, Points[j][i].y);
			glEnd();

			glColor3ub(Numb[j].ColorR, Numb[j].ColorG, Numb[j].ColorB);
			glLineWidth(Numb[j].LineSize);
			glBegin(GL_POLYGON);
			for (int i = 0; i < Points[j].size(); i++)
			{
				glTexCoord2f(TextureCoordinates[i].x, TextureCoordinates[i].y);
				glVertex2f(Points[j][i].x, Points[j][i].y);
			}
			glEnd();
		
			glPopMatrix();
			}
		/* режим окружности */
		if (Numb[j].mode == 2)
		{
			glPushMatrix();
			if (Numb[j].texture == -1)
				glDisable(GL_TEXTURE_2D);
			
			glColor3ub(Numb[j].ColorR, Numb[j].ColorG, Numb[j].ColorB);
			if (Numb[j].circle == 1)
			{
				glLineWidth(Numb[j].LineSize);
				glBegin(GL_POLYGON);
				for (int i = 0; i < Points[j].size(); i++)
					glVertex2f(Points[j][i].x, Points[j][i].y);
				glEnd();
				glColor3ub(0, 0, 0);
			}
			if (Numb[j].circle == 0 && Numb[j].ColorR == 255 && Numb[j].ColorG == 255 && Numb[j].ColorB == 255)
				glColor3ub(0, 0, 0);
			glLineWidth(Numb[j].LineSize);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < Points[j].size(); i++)
				glVertex2f(Points[j][i].x, Points[j][i].y);
			glEnd();

			glPopMatrix();
		}
	}
	/* отрисовка точек текущего примитива */
	glColor3ub(0, 0, 0);
	glPushMatrix();
	// участок с поворотом вокруг центра
	glTranslated(Numb[n].O.x, Numb[n].O.y, 0);
	if (Numb[n].mode == 1)
		glScalef(Numb[n].scaleX, Numb[n].scaleY, 1);	//масштабирование вдоль осей
	glRotated(Numb[n].angle, 0, 0, 1.0f);
	glTranslated(-Numb[n].O.x, -Numb[n].O.y, 0);

	glPointSize(Numb[n].PointSize);
	glBegin(GL_POINTS);
	for (int i = 0; i < Points[n].size(); i++)
		glVertex2f(Points[n][i].x, Points[n][i].y);
	glEnd();
	glPopMatrix();
	
	glFinish();
}

/* Функция изменения размеров окна */
void Reshape(GLint w, GLint h)
{
	Width = w;    Height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
#pragma endregion

#pragma region Control
/* Функция обработки сообщений от клавиатуры */
void Keyboard(unsigned char key, int x, int y)
{
	int i, m = Points[n].size();

	/* выход по нажатию Esc */
	if (key == 27) exit(0);

	/* Изменение RGB-компонент цвета точек */
	if (key == 'r') Numb[n].ColorR += 5;
	if (key == 'g') Numb[n].ColorG += 5;
	if (key == 'b') Numb[n].ColorB += 5;

	/* Изменение XY-кординат точек */
	if (key == 'w') { for (i = 0; i < m; i++) Points[n][i].y += 5; Numb[n].O.y += 5; }
	if (key == 's') { for (i = 0; i < m; i++) Points[n][i].y -= 5; Numb[n].O.y -= 5; }
	if (key == 'a') { for (i = 0; i < m; i++) Points[n][i].x -= 5; Numb[n].O.x -= 5; }
	if (key == 'd') { for (i = 0; i < m; i++) Points[n][i].x += 5; Numb[n].O.x += 5; }

	/* Изменение размера точек */
	if (key == 'u') Numb[n].PointSize++;
	if (key == 'i') Numb[n].PointSize--;

	/* Изменение размера линии */
	if (key == 'j') Numb[n].LineSize++;
	if (key == 'k') Numb[n].LineSize--;

	/* Изменение группы примитивов*/
	if (key == 'x') 
		if (n + 1 < Points.size())
			n++;
	if (key == 'z') 
		if (n != 0)
			n--;

	/* удаление текущего примитива */
	if (key == 'c')
	{
		if (n != 0)
		{
			for (int j = n; j < Points.size() - 1 ; j++)
			{
				Points[j] = Points[j + 1];
				Numb[j] = Numb[j + 1];
			}
			n--;
			Points.resize(Points.size() - 1);
			Numb.resize(Numb.size() - 1);
		}
		else
		{
			Points.pop_back();
			Points.resize(n + 1);
		}
	}

	/* функция поворота шестиугольника по часовой стрелке */
	if (key == 'q')
		if (Numb[n].mode == 1)
			Numb[n].angle += 1;
	
	/* функция поворота шестиугольника против часовой стрелки */
	if (key == 'e')
		if (Numb[n].mode == 1)
			Numb[n].angle -= 1;
	
	/* функция увеличения размера по х*/
	if (key == '+')
	{
		int key2 = glutGetModifiers();
		if (key2 == GLUT_ACTIVE_SHIFT)
		{
			Numb[n].scaleX += 0.25;
			Numb[n].scaleY += 0.25;
		}
		else Numb[n].scaleX += 0.25;
	}

	if(key == '`')
		if (colorMod != 2)
			colorMod++;
		else colorMod = 0;

	/* функция уменьшения размера по у */
	if (key == '-')
	{
		int key2 = glutGetModifiers();
		if (key2 == GLUT_ACTIVE_SHIFT)
		{
			Numb[n].scaleX *= 0.75;
			Numb[n].scaleY *= 0.75;
		}
		else Numb[n].scaleX *= 0.75;
	}

	/* функция увеличения размера по y */
	if (key == '*')
	{
		int key2 = glutGetModifiers();
		if (key2 == GLUT_ACTIVE_SHIFT)
		{
			Numb[n].scaleX += 0.25;
			Numb[n].scaleY += 0.25;
		}
		else Numb[n].scaleY += 0.25;
	}

	/* функция уменьшения размера по у */
	if (key == '/')
	{
		int key2 = glutGetModifiers();
		if (key2 == GLUT_ACTIVE_SHIFT)
		{
			Numb[n].scaleX *= 0.75;
			Numb[n].scaleY *= 0.75;
		}
		else Numb[n].scaleY *= 0.75;
	}

	/* режим ломаной || режим смешивания цветов */
	if (key == '1')
	{
		int key2 = glutGetModifiers();
		if (key2 == GLUT_ACTIVE_SHIFT)
		{
			if (colorMod != 2) colorMod++;
			else colorMod = 0;
		}
		else
		{
			mode = 0;
			if (Points[n].size() != 0)
			{
				n = Points.size();
				Points.resize(n + 1);
				Numb.resize(n + 1);
			}
		}
	}

	/* режим шестиугольника */
	if (key == '2') mode = 1;

	/* режим окружности */
	if (key == '3') mode = 2;

	/* смена текстуры */
	if (key == 't') 
	{
		if (Numb[n].mode == 1)
		{
			if (Numb[n].texture < 7)
				Numb[n].texture++;
			else Numb[n].texture = -1;
		}
	}


	glutPostRedisplay();

	char v[120]; sprintf(v, "Текущий цвет всех точек: R=%.3d G=%.3d B=%.3d  Текущий номер группы примитивов: %d Всего примитивов: %d", Numb[n].ColorR, Numb[n].ColorG, Numb[n].ColorB, n + 1, Points.size());
	glutSetWindowTitle(v);
}

void KeyboardSpecialKeys(int key, int x, int y)
{
	/* режим сетки */
	if (key == GLUT_KEY_F1)
	{
		if (modeGrid == 0)
			modeGrid = 1;
		else modeGrid = 0;
	}

	/* размер сетки */
	if (key == GLUT_KEY_F2)
	{
		if (gridSize < 16)
			gridSize += 2;
		else gridSize = 6;
	}
	
	/* режим сглаживания */
	if (key == GLUT_KEY_F3)
	{
		if (smoothMode == 0)
			smoothMode = 1;
		else smoothMode = 0;
	}
	
	/* режим круга */
	if (key == GLUT_KEY_F4)
	{
		if (Numb[n].mode == 2)
			Numb[n].circle = 1;
	}

	glutPostRedisplay();
}

/* Функция обработки сообщения от мыши */
void Mouse(int button, int state, int x, int y)
{
	/* клавиша была нажата, но не отпущена */
	if (state != GLUT_DOWN) return;

	/* новая точка по левому клику */
	if (button == GLUT_LEFT_BUTTON)
	{
		if (mode == 0)
		{
			type_point p(x, Height - y);
			Points[n].push_back(p);
		}
		if (mode == 1)
		{
			k++;
			if (k % 2 != 0)
			{
				if (Points[n].size() != 0)
				{
					n = Points.size();
					Points.resize(n + 1);
					Numb.resize(n + 1);
				}
				type_point p(x, Height - y);
				H.O = p;
				Points[n].push_back(p);
				Numb[n].mode = 1;
			}
			else
			{
				Points[n].pop_back();
				type_point p(x, Height - y);
				H.A = p;
				Build_Points(H.O, H.A);
				Numb[n].ColorB = 255;
				Numb[n].ColorR = 255;
				Numb[n].ColorG = 255;
			}
		}
		if (mode == 2)
		{
			k++;
			if (k % 2 != 0)
			{
				if (Points[n].size() != 0)
				{
					n = Points.size();
					Points.resize(n + 1);
					Numb.resize(n + 1);
				}
				type_point p(x, Height - y);
				H.O = p;
				Points[n].push_back(p);
				Numb[n].mode = 2;
			}
			else
			{
				Points[n].pop_back();
				type_point p(x, Height - y);
				H.A = p;
				Build_Circle(H.O, H.A);
				Numb[n].ColorB = 255;
				Numb[n].ColorR = 255;
				Numb[n].ColorG = 255;
			}
		}
		/* удаление последней точки по центральному клику */
		if (button == GLUT_MIDDLE_BUTTON)
		{
			if (Points[n].size() != 0)
				Points[n].pop_back();
		}

		glutPostRedisplay();
	}
}
#pragma endregion

#pragma region Menu&Main
void Menu(int pos)
{
	int key = (keys)pos;

	switch (key)
	{
	case KeyR: Keyboard('r', 0, 0); break;
	case KeyG: Keyboard('g', 0, 0); break;
	case KeyB: Keyboard('b', 0, 0); break;
	case KeyW: Keyboard('w', 0, 0); break;
	case KeyS: Keyboard('s', 0, 0); break;
	case KeyA: Keyboard('a', 0, 0); break;
	case KeyD: Keyboard('d', 0, 0); break;
	case KeyU: Keyboard('u', 0, 0); break;
	case KeyI: Keyboard('i', 0, 0); break;
	case KeyX: Keyboard('x', 0, 0); break;
	case KeyZ: Keyboard('z', 0, 0); break;
	//case KeyN: Keyboard('n', 0, 0); break;
	case KeyK: Keyboard('k', 0, 0); break;
	case KeyJ: Keyboard('j', 0, 0); break;
	case KeyC: Keyboard('c', 0, 0); break;
	case Key1: Keyboard('1', 0, 0); break;
	case Key2: Keyboard('2', 0, 0); break;

	default:
		int menu_color = glutCreateMenu(Menu);
		glutAddMenuEntry("Компонента R", KeyR);
		glutAddMenuEntry("Компонента G", KeyG);
		glutAddMenuEntry("Компонента B", KeyB);

		int menu_move = glutCreateMenu(Menu);
		glutAddMenuEntry("Вверх", KeyW);
		glutAddMenuEntry("Вниз", KeyS);
		glutAddMenuEntry("Bлево", KeyA);
		glutAddMenuEntry("Вправо", KeyD);

		int menu_size = glutCreateMenu(Menu);
		glutAddMenuEntry("Увеличить", KeyU);
		glutAddMenuEntry("Уменьшить", KeyI);

		int menu_numb_primitiv = glutCreateMenu(Menu);
		glutAddMenuEntry("Следующая группа примитивов", KeyX);
		glutAddMenuEntry("Предыдущий группа примитивов", KeyZ);

		int menu_mode = glutCreateMenu(Menu);
		glutAddMenuEntry("Режим замкнутой ломаной", Key1);
		glutAddMenuEntry("Режим шестиугольника", Key2);

		int menu_info = glutCreateMenu(Menu);
		glutAddMenuEntry("Клавиша R - изменить красную компоненту цвета", KeyR);
		glutAddMenuEntry("Клавиша G - изменить зеленую компоненту цвета", KeyG);
		glutAddMenuEntry("Клавиша B - изменить синюю компоненту цвета", KeyB);
		glutAddMenuEntry("Клавиша W - переместить вверх", KeyW);
		glutAddMenuEntry("Клавиша S - переместить вниз", KeyS);
		glutAddMenuEntry("Клавиша A - переместить влево", KeyA);
		glutAddMenuEntry("Клавиша D - переместить вправо", KeyD);
		glutAddMenuEntry("Клавиша U - увеличить размер точки", KeyU);
		glutAddMenuEntry("Клавиша I - уменьшить размер точки", KeyI);
		glutAddMenuEntry("Клавиша J - увеличить толщину линии", KeyJ);
		glutAddMenuEntry("Клавиша K - уменьшить толщину линии", KeyK);
		glutAddMenuEntry("Клавиша X - выбрать следующий примитив", KeyX);
		glutAddMenuEntry("Клавиша Z - выбрать предыдущий примитив", KeyZ);
		//glutAddMenuEntry("Клавиша N - создать новый примитив", KeyN);
		glutAddMenuEntry("Клавиша C - удалить текущий примитив", KeyC);
		glutAddMenuEntry("Клавиша 1 - режим замкнутой ломаной", Key1);
		glutAddMenuEntry("Клавиша 2 - режим шестиугольника", Key2);

		int menu = glutCreateMenu(Menu);
		glutAddSubMenu("Смена цвета", menu_color);
		glutAddSubMenu("Перемещение", menu_move);
		glutAddSubMenu("Изменение размера точки", menu_size);
		glutAddSubMenu("Изменениегруппы примитивов", menu_numb_primitiv);
		glutAddSubMenu("Режим построения", menu_mode);
		glutAddSubMenu("Горячие клавиши", menu_info);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
		Keyboard(Empty, 0, 0);
	}
}

/* Головная программа */
void main(int argc, char *argv[])
{
	Points.resize(n + 1);
	Numb.resize(n + 1);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Текущий цвет всех точек:");
	Menu(Empty);
	Textures();
	setTextureCoordinates();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(KeyboardSpecialKeys);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop();
}
#pragma endregion
