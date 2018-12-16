#ifndef utils_hpp
#define utils_hpp

#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#include "SOIL2.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <sstream>

using namespace std;

//-----------

typedef struct {
    GLfloat verticeXYZ[3];
    GLfloat colorRGB[3];
} Vertice;

typedef struct {
    float x,y;
    int izda, dcha;
} EstadoRaton;

//-----------

extern bool* keyStates;
extern bool animacion;
extern vector<GLuint> posxl;
extern vector<GLuint> posyl;
extern vector<GLuint> poszl;
extern Vertice posx[4];
extern Vertice posy[4];
extern Vertice posz[4];
extern bool movingUp;
extern Vertice selector;
extern EstadoRaton raton;
extern int ventana[2], vertice_sel;
extern GLfloat formato_global;

//-----------

void sleep(unsigned int mseconds);
string convertInt(int number);
void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);

void ReProyectar(int w, int h);
float ObtenerPosPlanoX(float x, int ancho_ventana, int alto_ventana, float pos_x_min, float pos_x_max, float format);
float ObtenerPosPlanoY(float y, int ancho_ventana, int alto_ventana, float pos_y_min, float pos_y_max, float format);
void ControlRaton(int button, int state, int x, int y);
void MovimRaton(int x, int y);
void MoverVertice(int value);
void IniciarGLUT();



#endif
