

// OpenGL Jajko(LAB3).cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <vector>

using namespace std;

typedef float point3[3];
static int N = 100;
static int kind = 1;
static int type = 2;
static float colors_tab[100][100][6][3];

#define M_PI 3.14159265358979323846

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

static GLfloat theta_x = 0.0;   // kąt obrotu obiektu
static GLfloat theta_y = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle2;

static GLint status = 0;       // stan klawiszy myszy
							   // 0 - nie naciśnięto żadnego klawisza
							   // 1 - naciśnięty zostać lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
							   // i poprzednią kursora myszy
static int y_pos_old = 0;

static int delta_y = 0;

static int zoom_old;

static int delta_zoom;

const int font = (int)GLUT_BITMAP_9_BY_15;

void renderBitmapString(float x, float y, void *font, const char *string)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
							  // jako pozycji poprzedniej

		y_pos_old = y;

		status = 1;          // wcięnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		zoom_old = y;

		status = 2;          // wciêniêty zosta³ prawy klawisz myszy
	}
	else
	{
		status = 0;          // nie został wcięnięty żaden klawisz
	}

}

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy

	x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie

	delta_y = y - y_pos_old;

	y_pos_old = y;

	delta_zoom = y - zoom_old;

	zoom_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

//funkcja zwracaj¹ca liczbê z przedzia³u [0, 0.9]
float draw_random_float()
{
	float number;
	double const red_random = rand() % 10;

	number = red_random / 10;

	return number;
}

// Funkcja rysujaca osie ukladu wspólrzednych
void Axes(void)
{

	point3  x_min = { -0.5*viewer[2], 0.0, 0.0 };
	point3  x_max = { 0.5*viewer[2], 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x
	point3  y_min = { 0.0, -0.5*viewer[2], 0.0 };
	point3  y_max = { 0.0, 0.5*viewer[2], 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -0.5*viewer[2] };
	point3  z_max = { 0.0, 0.0, 0.5*viewer[2] };
	//  pocz?tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x

	glVertex3fv(x_min);
	glVertex3fv(x_max);

	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);

	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);

	glEnd();

}



void Egg() {
	float tab[100][100][3];

	float a = 0.0;
	float b = 0.0;

	//zamiana dziedziny parametrycznej na postaæ 3-D wed³ug okrelonego wzoru
	for (int i = 0; i<N; i++)
		for (int j = 0; j<N; j++)
		{
			a = (float)i / (N - 1);
			b = (float)j / (N - 1);
			tab[i][j][0] = (-90 * pow(a, 5) + 225 * pow(a, 4) - 270 * pow(a, 3) + 180 *
				pow(a, 2) - 45 * a)*cos(M_PI * b);
			tab[i][j][1] = 160 * pow(a, 4) - 320 * pow(a, 3) + 160 * pow(a, 2) - 5;
			tab[i][j][2] = (-90 * pow(a, 5) + 225 * pow(a, 4) - 270 * pow(a, 3) + 180 *
				pow(a, 2) - 45 * a)*sin(M_PI * b);
		}



	glColor3f(1.0f, 1.0f, 0.0f);  //zólty


								  //rysowanie punktów
	if (type == 0)
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < N - 1; ++i) {
			for (int j = 0; j < N - 1; ++j)
			{
				glVertex3fv(tab[i][j]);//x y 3 wierzcholki trójkata
				glVertex3fv(tab[i][j + 1]);
				glVertex3fv(tab[i][j]);
			}
		}
		glEnd();
	}

	//rysowanie linii
	if (type == 1)
	{
		glBegin(GL_LINES);
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++)
			{
				glVertex3fv(tab[i][j]);//x y 3 wierzcholki trójkata
				glVertex3fv(tab[i][j + 1]);
				glVertex3fv(tab[i][j]);
				glVertex3fv(tab[i + 1][j]);
				glVertex3fv(tab[i + 1][j]);
				glVertex3fv(tab[i][j + 1]);
			}
		}
		glEnd();
	}
	//rysowanie kolorowych trójk¹tów
	if (type == 2)
	{
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++)
			{

				glBegin(GL_TRIANGLES);

				//wybór koloru za pomoc¹ globalnej tablicy z kolorami
				glColor3fv(colors_tab[i][j][0]);
				glVertex3fv(tab[i][j + 1]);

				glColor3fv(colors_tab[i][j][1]);
				glVertex3fv(tab[i + 1][j]);

				glColor3fv(colors_tab[i][j][2]);
				glVertex3fv(tab[i + 1][j + 1]);


				glEnd();


				glBegin(GL_TRIANGLES);

				glColor3fv(colors_tab[i][j][3]);
				glVertex3fv(tab[i + 1][j]);


				glColor3fv(colors_tab[i][j][4]);
				glVertex3fv(tab[i][j + 1]);


				glColor3fv(colors_tab[i][j][5]);
				glVertex3fv(tab[i][j]);

				glEnd();
			}
		}
	}

}


// Funkcja okreslajaca co ma byc rysowane (zawsze wywolywana gdy trzeba
// przerysowac scene)
void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczacym

	glLoadIdentity();
	// Czyszczenie macierzy biezacej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wyzej

	glRotated(20.0, 1.0, 0.0, 0.0);

	if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
	{
		theta_x += delta_x*pix2angle;

		theta_y += delta_y*pix2angle2;
	}

	if (status == 2)                     // jeœli prawy klawisz myszy wciêniêty
	{

		viewer[2] += delta_zoom;
	}

	glRotatef(theta_x, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
	glRotatef(theta_y, 1.0, 0.0, 0.0);

	if (kind == 0)
	{
		glutWireTeapot(3.0);
	}
	if (kind == 1)
	{
		Egg();
	}

	renderBitmapString(-5, 5, (void *)font, "1 - Dzbanek");
	renderBitmapString(-5, 4.8, (void *)font, "2 - Jajko");
	renderBitmapString(-5, 4.6, (void *)font, "p - punkty");
	renderBitmapString(-5, 4.4, (void *)font, "w - linie");
	renderBitmapString(-5, 4.2, (void *)font, "s - kolorowe trojkaty");
	renderBitmapString(4, 5, (void *)font, "LPM - obrot");
	renderBitmapString(4, 4.8, (void *)font, "PPM - zoom");

	glFlush();
	// Przekazanie polecen rysujacych do wykonania



	glutSwapBuffers();
	//

}

//funkcja zwrotna do wyboru rodzaju modelu, pobrana ze strony zsk
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') type = 0;
	if (key == 'w') type = 1;
	if (key == 's') type = 2;

	if (key == '1') kind = 0;
	if (key == '2') kind = 1;
	RenderScene(); // przerysowanie obrazu sceny
}


// Funkcja ustalajaca stan renderowania
void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcacy (wypelnienia okna) ustawiono na czarny

}


// Funkcja ma za zadanie utrzymanie stalych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokosc i szerokosc okna) sa
// przekazywane do funkcji za kazdym razem gdy zmieni sie rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	pix2angle2 = 360.0 / (float)vertical;

	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bieżącej

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkości okna okna widoku (viewport) w zależności
	// relacji pomiędzy wysokością i szerokością okna

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej 

}

/*************************************************************************************/

// Glówny punkt wejscia programu. Program dziala w trybie konsoli



void main(void)
{
	//srand(time(NULL));


	//Naniesienie wartoci na globaln¹ tablicê z losowymi wspó³czynnikami z przedzia³u [0, 0.9]
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					colors_tab[i][j][k][l] = draw_random_float();
				}

			}

		}
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uklad wspólrzednych 3-D");



	glutDisplayFunc(RenderScene);

	glutKeyboardFunc(keys);
	// Okreslenie, ze funkcja RenderScene bedzie funkcja zwrotna
	// (callback function).  Bedzie ona wywolywana za kazdym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutMouseFunc(Mouse);
	// Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcje zwrotna odpowiedzialna
	// zazmiany rozmiaru okna      

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyzej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystapieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// Wlaczenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT


}

