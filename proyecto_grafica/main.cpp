#include "utils.hpp"

string pathImage = "/Users/camilonunezfernandez/proyecto_grafica/proyecto_grafica/textures/";

int tiempo = 0;
int cantImages=26;
int sleepTime = 3500000; // 3.5 segundos cada foto

GLuint loadTexture(const char * filename,char type){
    GLuint textureID;
    if(type=='x'){
        textureID=SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
    }else if(type=='y'){
        textureID=SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        
    }else{
        textureID=SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS );
    }
    
    
    if(textureID == NULL){
        printf("[Texture loader] %s failed to load!\n",filename);
    }
    
    return textureID;
}

void cargar_texturas(){
    for(int i=0; i<cantImages; i++){
            string filenameposx = pathImage+"skybox/posx"+convertInt(i)+".jpg";
            string filenameposy = pathImage+"skybox/posy"+convertInt(i)+".jpg";
            string filenameposz = pathImage+"skybox/posz"+convertInt(i)+".jpg";
            posxl.push_back(loadTexture(filenameposx.c_str(),'x'));
            posyl.push_back(loadTexture(filenameposy.c_str(),'y'));
            poszl.push_back(loadTexture(filenameposz.c_str(),'z'));
    }
}

void animar(){
    glMatrixMode(GL_MODELVIEW);
    glClearColor( 0, 0, 0, 1 );
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    
    int i;
    tiempo++;
    glColor3f(1.0f,1.0f,1.0f);
    
    //Cara Roja - X
    glBindTexture(GL_TEXTURE_2D, posxl[tiempo%cantImages]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBegin(GL_QUADS);
    for (i=0; i<4; i++) {
        if(i==0)
            glTexCoord2f(1.0,1.0);
        else if(i==1)
            glTexCoord2f(1.0,0.0);
        else if(i==2)
            glTexCoord2f(0.0,0.0);
        else if(i==3)
            glTexCoord2f(0.0,1.0);
        
        glVertex3fv(posx[i].verticeXYZ);
    }
    glEnd();
   
    //Cara Verde - Y
    glBindTexture(GL_TEXTURE_2D, posyl[tiempo%cantImages]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBegin(GL_QUADS);
    for (i=0; i<4; i++) {
        if(i==0){
            glTexCoord2f(1.0,1.0);
            
        }
        else if(i==1){
            glTexCoord2f(1.0,0.0);
            
        }
        else if(i==2){
            glTexCoord2f(0.0,0.0);
            
        }
        else if(i==3){
            glTexCoord2f(0.0,1.0);
        }
        
        glVertex3fv(posy[i].verticeXYZ);
        glRotatef(30, 0.0f, 0.0f, 1.0f);
    }
    
    glEnd();
    
    //Cara Azul - Z
    glBindTexture(GL_TEXTURE_2D, poszl[tiempo%cantImages]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBegin(GL_QUADS);
    for (i=0; i<4; i++) {
        if(i==0)
            glTexCoord2f(1.0,1.0);
        else if(i==1)
            glTexCoord2f(1.0,0.0);
        else if(i==2)
            glTexCoord2f(0.0,0.0);
        else if(i==3)
            glTexCoord2f(0.0,1.0);
        
        glVertex3fv(posz[i].verticeXYZ);
    }
    glEnd();
    
    glPointSize(7);
    glBegin(GL_POINTS);
    glColor3fv(selector.colorRGB);
    glVertex3fv(selector.verticeXYZ);
    glEnd();
}

void defecto() {
    
    tiempo=0;
    
    glMatrixMode(GL_MODELVIEW);
    glClearColor( 0, 0, 0, 1 );
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLoadIdentity();
    
    int i;
    
    glBegin(GL_QUADS);
    for (i=0; i<4; i++) {
        glColor3fv(posx[i].colorRGB);
        glVertex3fv(posx[i].verticeXYZ);
    }
    glEnd();
    
    glBegin(GL_QUADS);
    for (i=0; i<4; i++) {
        glColor3fv(posy[i].colorRGB);
        glVertex3fv(posy[i].verticeXYZ);
    }
    glEnd();
    
    glBegin(GL_QUADS);
    for (i=0; i<4; i++) {
        glColor3fv(posz[i].colorRGB);
        glVertex3fv(posz[i].verticeXYZ);
    }
    glEnd();
    
    glPointSize(7);
    glBegin(GL_POINTS);
    glColor3fv(selector.colorRGB);
    glVertex3fv(selector.verticeXYZ);
    glEnd();
    
    
}

void Render(){
    if(animacion){
        animar();
        sleep(sleepTime);
    }else{
        defecto();
    }
    //glFlush();
    glutSwapBuffers();
}


int main(int argc, char **argv) {
    glutInit(&argc,argv); //Solo necesario en Linux
    
    IniciarGLUT();
    cargar_texturas();
    
    glutReshapeFunc(ReProyectar);
    glutDisplayFunc(Render);
    glutIdleFunc(Render);
    glutMouseFunc(ControlRaton);
    glutTimerFunc(33,MoverVertice,1);
    glutMotionFunc(MovimRaton);
    glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events
    
    
    glutMainLoop();
    
    return 0;
}
