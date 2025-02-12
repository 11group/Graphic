/*
Отключено консольное (главное) окно:
Linker ->  Advanced -> Entery Point := "mainCRTStartup"
Linker ->  System -> SubSystem := "Windows (/SUBSYSTEM:WINDOWS)"
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <glut.h>
#include <vector>
using namespace std;

GLint Width = 1268, Height = 680;

int n = 0;

/* задание контейнера характеристик точек */
struct Characters
{
	GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
	GLubyte PointSize = 5;
	GLubyte LineSize = 3;
};

enum keys { Empty, KeyR, KeyG, KeyB, KeyW, KeyA, KeyS, KeyD, KeyU, KeyI, KeyZ, KeyX, KeyN, KeyJ, KeyK, KeyC};


/* задание контейнера вершин */
struct type_point
{
	GLint x, y;
	type_point(GLint _x, GLint _y) { x = _x; y = _y; }
};

vector <vector <type_point>> Points;
vector <Characters> Numb;

/* Функция вывода на экран */
void Display(void)
{
	glClearColor(240, 255, 240, 1); glClear(GL_COLOR_BUFFER_BIT);


	for (int j = 0; j < Points.size(); j++)
	{
		glColor3ub(Numb[j].ColorR, Numb[j].ColorG, Numb[j].ColorB);
		glPointSize(Numb[j].PointSize);
		glBegin(GL_POINTS);
		for (int i = 0; i < Points[j].size(); i++)
			glVertex2i(Points[j][i].x, Points[j][i].y);
		glEnd();
	}

	for (int j = 0; j < Points.size(); j++)
	{
		glColor3ub(Numb[j].ColorR, Numb[j].ColorG, Numb[j].ColorB);
		glLineWidth(Numb[j].LineSize);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < Points[j].size(); i++)
			glVertex2i(Points[j][i].x, Points[j][i].y);
		glEnd();
	}
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

/* Функция обработки сообщений от клавиатуры */
void Keyboard(unsigned char key, int x, int y)
{
	int i, m = Points[n].size();

	/* Изменение RGB-компонент цвета точек */
	if (key == 'r') Numb[n].ColorR += 5;
	if (key == 'g') Numb[n].ColorG += 5;
	if (key == 'b') Numb[n].ColorB += 5;

	/* Изменение XY-кординат точек */
	if (key == 'w') for (i = 0; i<m; i++) Points[n][i].y += 5;
	if (key == 's') for (i = 0; i<m; i++) Points[n][i].y -= 5;
	if (key == 'a') for (i = 0; i<m; i++) Points[n][i].x -= 5;
	if (key == 'd') for (i = 0; i<m; i++) Points[n][i].x += 5;

	/* Изменение размера точек */
	if (key == 'u') Numb[n].PointSize++;
	if (key == 'i') Numb[n].PointSize--;

	/* Изменение размера линии */
	if (key == 'j') Numb[n].LineSize++;
	if (key == 'k') Numb[n].LineSize--;

	/* Изменение группы примитивов*/
	if (key == 'x') 
		if (n + 1 < Points.size())
		{
			Numb[n].LineSize--;
			Numb[n].PointSize--;
			n++;
			Numb[n].LineSize++;
			Numb[n].PointSize++;
		}
	if (key == 'z') 
		if (n != 0)
		{
			Numb[n].LineSize--;
			Numb[n].PointSize--;
			n--;
			Numb[n].LineSize++;
			Numb[n].PointSize++;
		}

	/* новая группа примитивов */
	if (key == 'n')
	{
		n++;
		Points.resize(n + 1);
		Numb.resize(n + 1);
	}

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

	glutPostRedisplay();

	char v[90]; sprintf(v, "Текущий цвет всех точек: R=%.3d G=%.3d B=%.3d  Текущий номер группы примитивов: %d", Numb[n].ColorR, Numb[n].ColorG, Numb[n].ColorB, n + 1);
	glutSetWindowTitle(v);
}

/* Функция обработки сообщения от мыши */
void Mouse(int button, int state, int x, int y)
{
	/* клавиша была нажата, но не отпущена */
	if (state != GLUT_DOWN) return;

	/* новая точка по левому клику */
	if (button == GLUT_LEFT_BUTTON)
	{
		type_point p(x, Height - y);
		Points[n].push_back(p);
	}
	/* удаление последней точки по центральному клику */
	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (Points[n].size() != 0)
			Points[n].pop_back();
		else
		{
			/*vector<>pixels;
			FILE *gendalf = fopen("1.bmp", "r");
			fread(pixels, pixels_count, 1, gendalf);
			glBitmap(Width, Height, 0, 0, dX, dY, bitmap);
			glReadPixels(PosX, PosY, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, pixels);*/


		}
	}

	glutPostRedisplay();
}

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
	case KeyN: Keyboard('n', 0, 0); break;
	case KeyK: Keyboard('k', 0, 0); break;
	case KeyJ: Keyboard('j', 0, 0); break;
	case KeyC: Keyboard('c', 0, 0); break;


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
		glutAddMenuEntry("Клавиша N - создать новый примитив", KeyN);
		glutAddMenuEntry("Клавиша C - удалить текущий примитив", KeyC);

		int menu = glutCreateMenu(Menu);
		glutAddSubMenu("Смена цвета", menu_color);
		glutAddSubMenu("Перемещение", menu_move);
		glutAddSubMenu("Изменение размера точки", menu_size);
		glutAddSubMenu("Изменениегруппы примитивов", menu_numb_primitiv);
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
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop();
}
