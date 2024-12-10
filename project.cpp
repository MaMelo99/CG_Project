#include <GL/glut.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

float yCam = 5.0;
float xCam = 0.0;
float zCam = 20.0;
float rotationCam = 0.0;

GLuint idsTextura[3];
float rotacaoX = 0.0;
float rotacaoY = 0.0;
float rotacaoZ = 0.0;

float baseAngle = 0.0f;
float armAngle = 45.0f;
float headAngle = 0.0f;

float posLampX = 0.0f;
float posLampY = 0.0f;
float posLampZ = 0.0f;
float rotationLamp = 0.0;

GLfloat corHaste[] = {0.8, 0.8, 0.8};
GLfloat corConec[] = {0.5, 0.5, 0.5};
GLfloat corLamp[] = {1.0f, 0.9f, 0.6f};

float anguloLuzX = 0.0;
float anguloLuzY = 0.0;
float anguloLuzZ = 0.0;

int moveMode = 1;


void desenhaCilindro(float baseRadius, float topRadius, float height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, baseRadius, topRadius, height, slices, slices);

    if (baseRadius > 0.0f){
        glPushMatrix();
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        gluDisk(quad, 0.0f, baseRadius, slices, 1);
        glPopMatrix();
    }

    /*if (topRadius > 0.0f){
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, height);
        gluDisk(quad, 0.0f, topRadius, slices, 1);
        glPopMatrix();
    }*/

    gluDeleteQuadric(quad);
}

// Draw a sphere (used for joints and lamp head)
void desenhaEsfera(float raio, int fatias, int stacks) {
    glutSolidSphere(raio, fatias, stacks);
}

void desenhaBaseLuminaria(float tamanhoBase, float altura) {
    glPushMatrix();
        glColor3f(corConec[0], corConec[1], corConec[2]);
        glRotatef(90, 1, 0, 0);
        desenhaCilindro(tamanhoBase, tamanhoBase, altura, 100);
    glPopMatrix();
}


void desenhaPrimeiroBraco() {
    glPushMatrix();
        glColor3f(corHaste[0], corHaste[1], corHaste[2]);
        glTranslatef(0.0f, 1.3f, -1.2f); // Offset above the base
        glRotatef(armAngle, 1.0f, 0.0f, 0.0f);
        desenhaCilindro(0.2f, 0.2f, 2.0f, 20); // Arm cylinder
        glTranslatef(0.0f, 0.0f, 0.0f); // Move to the top of the arm
        glColor3f(corConec[0], corConec[1], corConec[2]);
        desenhaEsfera(0.3f, 20, 20); // Joint sphere
    glPopMatrix();
}

void desenhaBulbo(){
    glPushMatrix();
        glColor3f(corLamp[0], corLamp[1], corLamp[2]);
        glTranslatef(0.0, 0.0, 1.0);
        desenhaEsfera(0.475, 20, 20);
    glPopMatrix();
}

void desenhaSegundoBraco() {
    glPushMatrix();
        glPushMatrix();
            glColor3f(corHaste[0], corHaste[1], corHaste[2]);
            glTranslatef(0.0f, 1.5f, -1.1f); // Offset above the first arm
            glRotatef(-armAngle, 1.0f, 0.0f, 0.0f);
            desenhaCilindro(0.2f, 0.2f, 1.5f, 20); // Second arm cylinder
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 2.65f, 0.1f); // Move to the top of the second arm
            glColor3f(corConec[0], corConec[1], corConec[2]);
            desenhaEsfera(0.3f, 20, 20); // Joint sphere
        glPopMatrix();
        glPushMatrix();
            glColor3f(corHaste[0], corHaste[1], corHaste[2]);
            glTranslatef(0.0f, 3.3f, -0.3f);
            desenhaCilindro(0.4f, 0.7f, 1.5f, 20); // Second arm cylinder
            desenhaBulbo();
        glPopMatrix();
    glPopMatrix();
}


void carregarTextura(const char* nomeArquivo, int indice)
{
    int largura, altura, canais;

    unsigned char *dados = stbi_load(nomeArquivo, &largura, &altura, &canais, 0);

    if (!dados)
    exit(1);

    // gerar textura
    glGenTextures(1, &idsTextura[indice]);
    glBindTexture(GL_TEXTURE_2D, idsTextura[indice]);

    // configurar parametros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, (canais == 4) ? GL_RGBA : GL_RGB, 
                largura, altura, 0, (canais == 4) ? GL_RGBA : GL_RGB,
                GL_UNSIGNED_BYTE, dados);

    // liberar a memoria da imagem
    stbi_image_free(dados);              
}

void inicializa(){
    glClearColor(0.0, 0.0, 0.2, 1.0);
    
    glEnable(GL_DEPTH_TEST);

    GLfloat luzAmbiente[] = {0.7f, 0.7f, 0.7f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    //Bulbo de luz
    /*GLfloat luzPosicao[] = {posLampX, posLampY + 1.0f, posLampZ, 1.0f};
    GLfloat luzDifusa[] = {1.0f, 1.0f, 0.8f, 1.0f};
    GLfloat luzEspecular[] = {1.0f, 1.0f, 0.8f, 1.0f};*/

    //glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_POSITION);
    carregarTextura("texturas/madeira_mesa.jpg", 0);
    carregarTextura("texturas/porcelain.jpeg", 1);
    carregarTextura("texturas/porcelain_2.jpeg", 2);

    glEnable(GL_LIGHTING);
}

void desenhaBase(float tamanho){
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, -tamanho);
        glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho, -tamanho);
        glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, -tamanho, tamanho);
        glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho, tamanho);
    glEnd();
}
void reshape(int largura, int altura) {
 glViewport(0, 0, largura, altura);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 gluPerspective(65.0, (float)largura / (float)altura, 1.0, 50.0);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void desenhaLampada(){
    glTranslatef(posLampX, posLampY, posLampZ);
    glRotatef(rotationLamp, 0, 1, 0);
    desenhaBaseLuminaria(1.0, 0.5);
    desenhaPrimeiroBraco();
    desenhaSegundoBraco();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //Posicionamento da Câmera
    gluLookAt(xCam, yCam, zCam,
              rotationCam, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glPushMatrix();

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glColor3f(0.3, 0.3, 0.3);
            glScalef(20.0, 0.2,20.0);
            desenhaBase(1.0);
            //Desabilitando texturas para evitar interferência nas cores
            glDisable(GL_TEXTURE_2D);
            
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0, 0.3, 0.0);          
            desenhaLampada();
        glPopMatrix();

        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, idsTextura[2]);
            glEnable(GL_TEXTURE_2D);
            glColor3f(1.0, 1.0, 1.0);
            glTranslatef(-5, 2, 0);
            glutSolidTeapot(3.0);
            glDisable(GL_TEXTURE_2D);
            
        glPopMatrix();
        
    glPopMatrix();


    glutSwapBuffers();
}

void tecladoEspecial(int tecla, int x, int y){
    switch(tecla){
        case GLUT_KEY_UP:
            if (yCam <= 20.0){
                yCam += 2.0;
            }
            break;
        
        case GLUT_KEY_DOWN:
            if (yCam >= 2.0){
                yCam -= 2.0;
            }
            break;

        case GLUT_KEY_LEFT:
            if (rotationCam >= -20.0)
                rotationCam -= 1.0;
            break;
        
        case GLUT_KEY_RIGHT:
            if (rotationCam <= 20.0)
                rotationCam += 1.0;
            break;
    }
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y){
    switch(tecla){
        case 'w':
            if(moveMode == 0){
                zCam -= 1.0;
            }
            else{
                posLampZ -= 1.0;
            }
            break;
        
        case 'a':
            if(moveMode == 0){
                xCam -= 1.0;
            }
            else{
                posLampX -= 1.0;
            }
            break;
        
        case 'd':
            if(moveMode == 0){
                    xCam += 1.0;
                }
                else{
                    posLampX += 1.0; 
                }
            break;
        
        case 's':
            if(moveMode == 0){
                    zCam += 1.0;
                }
                else{
                    posLampZ += 1.0; 
                }
            break;
        
        case 'e':
            rotationLamp -= 30;
            if (rotationLamp >=360)
                rotationLamp = 0;
            break;

        case 'q':
            rotationLamp += 30;
            if (rotationLamp <= -360)
                rotationLamp = 0;
            break;
        
        case 'm':
            moveMode = moveMode ^ 1;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Projeto");

    inicializa();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(tecladoEspecial);
    glutKeyboardFunc(teclado);

    glutMainLoop();
    return 0;
}