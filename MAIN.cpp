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

GLint Width = 1920, Height = 1280;
GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
GLubyte PointSize = 5;
GLubyte LineSize = 3;
int n = 0;


enum keys { Empty, KeyR, KeyG, KeyB, KeyW, KeyA, KeyS, KeyD, KeyU, KeyI, KeyZ, KeyX, KeyN};


/* задание контейнера вершин */
struct type_point
{
	GLint x, y;
	type_point(GLint _x, GLint _y) { x = _x; y = _y; }
};
vector <vector <type_point>> Points;

/* Функция вывода на экран */
void Display(void)
{
	glClearColor(240, 255, 240, 1); glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(ColorR, ColorG, ColorB);
	glPointSize(PointSize);
	glBegin(GL_POINTS);
	for (int j = 0; j < Points.size(); j++)
		for (int i = 0; i<Points[j].size(); i++)
			glVertex2i(Points[j][i].x, Points[j][i].y);
	glEnd();

	glColor3ub(ColorR, ColorG, ColorB);
	glLineWidth(LineSize);
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < Points.size(); j++)
		for (int i = 0; i < Points[j].size(); i++)
			glVertex2i(Points[j][i].x, Points[j][i].y);
	glEnd();

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
	int i, m = Points.size();

	/* Изменение RGB-компонент цвета точек */
	if (key == 'r') ColorR += 5;
	if (key == 'g') ColorG += 5;
	if (key == 'b') ColorB += 5;

	/* Изменение XY-кординат точек */
	if (key == 'w') for (i = 0; i<m; i++) Points[n][i].y += 5;
	if (key == 's') for (i = 0; i<m; i++) Points[n][i].y -= 5;
	if (key == 'a') for (i = 0; i<m; i++) Points[n][i].x -= 5;
	if (key == 'd') for (i = 0; i<m; i++) Points[n][i].x += 5;

	/* Изменение размера точек */
	if (key == 'u') PointSize++;
	if (key == 'i') PointSize--;

	/* Изменение размера линии */
	if (key == 'j') LineSize++;
	if (key == 'k') LineSize--;

	/* Изменение группы примитивов*/
	if (key == 'x') if (n + 1 < Points.size()) n++;
	if (key == 'z') if (n != 0) n--;

	/* новая группа примитивов */
	if(key == 'n') 
	{
		vector <type_point> A;
		Points.push_back(A);
		n++;
	}

	glutPostRedisplay();

	char v[50]; sprintf(v, "Текущий цвет всех точек: R=%.3d G=%.3d B=%.3d", ColorR, ColorG, ColorB);
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
		if(Points[n].size()!=0)
			Points[n].pop_back();
		else
		{

			// спросить, как сделать, что при закрытии этого окна программа не падает
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

		default:
			int menu_color = glutCreateMenu(Menu);
			glutAddMenuEntry("Компонента R", KeyR);
			glutAddMenuEntry("Компонента G", KeyG);
			glutAddMenuEntry("Компонента B", KeyB);

			int menu_move = glutCreateMenu(Menu);
			glutAddMenuEntry("Вверх",  KeyW);
			glutAddMenuEntry("Вниз",   KeyS);
			glutAddMenuEntry("Bлево",  KeyA);
			glutAddMenuEntry("Вправо", KeyD);

			int menu_size = glutCreateMenu(Menu);
			glutAddMenuEntry("Увеличить", KeyU);
			glutAddMenuEntry("Уменьшить", KeyI);

			int menu_numb_primitiv = glutCreateMenu(Menu);
			glutAddMenuEntry("Следующий примитив", KeyX);
			glutAddMenuEntry("Предыдущий примитив", KeyZ);

			int menu = glutCreateMenu(Menu);
			glutAddSubMenu("Смена цвета", menu_color);
			glutAddSubMenu("Перемещение", menu_move);
			glutAddSubMenu("Изменение размера точки", menu_size);

			glutAttachMenu(GLUT_RIGHT_BUTTON);
			Keyboard(Empty,0,0);
	}
}


/* Головная программа */
void main(int argc, char *argv[])
{
	Points.resize(n+1);

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
