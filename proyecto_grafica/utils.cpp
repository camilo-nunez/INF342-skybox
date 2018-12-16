#include "utils.hpp"

bool* keyStates = new bool[256];
bool animacion = false;

Vertice posx[4] = { //rojo
    {{8,-4,0},{1,0,0}},
    {{8,-2,0},{1,0,0}},
    {{6,-2,0},{1,0,0}},
    {{6,-4,0},{1,0,0}}
};
Vertice posy[4] = { //verde
    {{1,8,0},{0,1,0}},
    {{1,6,0},{0,1,0}},
    {{-1,6,0},{0,1,0}},
    {{-1,8,0},{0,1,0}}
};
Vertice posz[4] = { //azul
    {{-8,-4,0},{0,0,1}},
    {{-8,-2,0},{0,0,1}},
    {{-6,-2,0},{0,0,1}},
    {{-6,-4,0},{0,0,1}}
};
bool movingUp = false;
Vertice selector = {{0,0,0},{1,1,1}};
EstadoRaton raton;
int ventana[2], vertice_sel;
GLfloat formato_global;

vector<GLuint> posxl;
vector<GLuint> posyl;
vector<GLuint> poszl;

//-------------------

void sleep(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

string convertInt(int number){
    stringstream ss;
    ss << number;
    return ss.str();
}

void keyPressed (unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp (unsigned char key, int x, int y) {
    keyStates[key] = false;
    
    switch ( key ) {
        case 'r':
            glutFullScreen();
            break;
        case 's':
            glutReshapeWindow(800, 600);
            glutPositionWindow(100,100);
            break;
        case 'e':
            exit(1);
        case 'z':
            animacion = true;
            break;
        case 'x':
            animacion = false;
            break;
    }
}

void ReProyectar(int w, int h) {
    GLfloat formato;
    
    ventana[0] = w;
    ventana[1] = h;
    
    if(h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    formato = (GLfloat)w / (GLfloat)h;
    formato_global = formato;
    if (w <= h) glOrtho (-10.0f, 10.0f, -10.0f / formato, 10.0f / formato, -1.0f, 1.0f);
    else glOrtho (-10.0f * formato, 10.0f * formato, -10.0f, 10.0f, -1.0f, 1.0f);
}

float ObtenerPosPlanoX(float x, int ancho_ventana, int alto_ventana, float pos_x_min, float pos_x_max, float format) {
    float pos_x_relativa = ((float)x/ancho_ventana); //La posición relativa de 0 a 1 en X
    float pos_plano;
    
    if (ancho_ventana<=alto_ventana) pos_plano = (pos_x_min+((pos_x_max-pos_x_min)*pos_x_relativa));
    else pos_plano = ((pos_x_min * format)+(((pos_x_max-pos_x_min) * format)*pos_x_relativa));
    
    return pos_plano;
}

float ObtenerPosPlanoY(float y, int ancho_ventana, int alto_ventana, float pos_y_min, float pos_y_max, float format) {
    float pos_y_relativa = ((float)y/alto_ventana); //La posición relativa de 0 a 1 en Y
    float pos_plano;
    
    if (ancho_ventana<=alto_ventana) pos_plano = -((pos_y_min / format)+(((pos_y_max-pos_y_min) / format)*pos_y_relativa));
    else pos_plano = -(pos_y_min+((pos_y_max-pos_y_min)*pos_y_relativa));
    
    return pos_plano;
}

void ControlRaton(int button, int state, int x, int y) {
    if (button==GLUT_LEFT_BUTTON) {
        if (state==GLUT_DOWN){
            raton.izda = 1;
        }
        else raton.izda = 0;
    }
    else if (button==GLUT_RIGHT_BUTTON) {
        if (state==GLUT_DOWN) {
            raton.dcha = 1;
            
            raton.x = ObtenerPosPlanoX(x,ventana[0],ventana[1],-10,10,formato_global);
            raton.y = ObtenerPosPlanoY(y,ventana[0],ventana[1],-10,10,formato_global);
            
            int i;
            for (i=0; i<4; i++) {
                if (raton.x > posy[i].verticeXYZ[0] - 0.5f &&
                    raton.x < posy[i].verticeXYZ[0] + 0.5f &&
                    raton.y > posy[i].verticeXYZ[1] - 0.5f &&
                    raton.y < posy[i].verticeXYZ[1] + 0.5f ) {
                    vertice_sel = i;
                }
            }
            for (i=0; i<4; i++) {
                if (raton.x > posx[i].verticeXYZ[0] - 0.5f &&
                    raton.x < posx[i].verticeXYZ[0] + 0.5f &&
                    raton.y > posx[i].verticeXYZ[1] - 0.5f &&
                    raton.y < posx[i].verticeXYZ[1] + 0.5f ) {
                    vertice_sel = 4 + i;
                }
            }
            for (i=0; i<4; i++) {
                if (raton.x > posz[i].verticeXYZ[0] - 0.5f &&
                    raton.x < posz[i].verticeXYZ[0] + 0.5f &&
                    raton.y > posz[i].verticeXYZ[1] - 0.5f &&
                    raton.y < posz[i].verticeXYZ[1] + 0.5f ) {
                    vertice_sel = 8 + i;
                }
            }
        }
        else raton.dcha = 0;
    }
}

void MovimRaton(int x, int y) {
    raton.x = ObtenerPosPlanoX(x,ventana[0],ventana[1],-10,10,formato_global);
    raton.y = ObtenerPosPlanoY(y,ventana[0],ventana[1],-10,10,formato_global);
}

void MoverVertice(int value) {
    switch (vertice_sel) {
        case 0: case 1: case 2: case 3:
            selector.verticeXYZ[0] = posy[vertice_sel].verticeXYZ[0];
            selector.verticeXYZ[1] = posy[vertice_sel].verticeXYZ[1];
            break;
        case 4: case 5: case 6: case 7:
            selector.verticeXYZ[0] = posx[vertice_sel-4].verticeXYZ[0];
            selector.verticeXYZ[1] = posx[vertice_sel-4].verticeXYZ[1];
            break;
        case 8: case 9: case 10: case 11:
            selector.verticeXYZ[0] = posz[vertice_sel-8].verticeXYZ[0];
            selector.verticeXYZ[1] = posz[vertice_sel-8].verticeXYZ[1];
            break;
    }
    
    if (raton.izda) {
        if (vertice_sel<=3) {
            posy[vertice_sel].verticeXYZ[0] = selector.verticeXYZ[0] = raton.x;
            posy[vertice_sel].verticeXYZ[1] = selector.verticeXYZ[1] = raton.y;
        }
        else if(vertice_sel>3 && vertice_sel<=7){
            posx[vertice_sel-4].verticeXYZ[0] = selector.verticeXYZ[0] = raton.x;
            posx[vertice_sel-4].verticeXYZ[1] = selector.verticeXYZ[1] = raton.y;
        }
        else
        {
            posz[vertice_sel-8].verticeXYZ[0] = selector.verticeXYZ[0] = raton.x;
            posz[vertice_sel-8].verticeXYZ[1] = selector.verticeXYZ[1] = raton.y;
        }
    }
    
    glutTimerFunc(33,MoverVertice,1);
    glutPostRedisplay();
}

void IniciarGLUT(){
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("INF342-Grafica");
}

