//#include <GL/glut.h> //->For Linux
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

float yCam = 5.0;
float xCam = 0.0;
float zCam = 20.0;
float rotationCamX = 0.0;
float rotationCamY = 0.0;

int mouseOldX = -1, mouseOldY = -1;
bool isMousePressed = false;  

GLuint idsTextura[10];
float rotacaoX = 0.0;
float rotacaoY = 0.0;
float rotacaoZ = 0.0;

float baseAngle = 0.0f;
float armAngle = 45.0f;
float headAngleY = 0.0f;
float headAngleX = 0.0f;
float lampHeight = 5.0f;  // Initial height of the lamp

float posLampX = 0.0f;
float posLampY = 0.0f;
float posLampZ = 0.0f;
float rotationLamp = 0.0;

GLfloat corHaste[] = {0.8, 0.8, 0.8, 1.0f};
GLfloat corConec[] = {0.5, 0.5, 0.5, 1.0f};
GLfloat corLamp[] = {1.0f, 0.9f, 0.6f, 1.0f};

float anguloLuzX = 0.0;
float anguloLuzY = 0.0;
float anguloLuzZ = 0.0;

float startTime = 0.0f;
bool isAnimating = false;
const float animationDuration = 5.0f;

float jumpHeight = 1.0f; // Maximum height of the jump
float jumpDuration = 1.0f; // Duration to complete the jump cycle (up + down)
float jumpTime = 0.0f; // Time passed in the jump cycle
bool isJumping = false;
bool jumpUp = true; // To track whether the object is going up or down
float jumpYPos = 0.0f;

bool isMovingForward = false;
bool isMovingBackward = false;
bool isMovingLeft = false;
bool isMovingRight = false;

float sizeTeapot = 3.0;
float teapotPos[] = {-10.0, 2, 0};

void resetAllGlobals(){
    yCam = 5.0;
    xCam = 0.0;
    zCam = 20.0;
    rotationCamX = 0.0;
    rotationCamY = 0.0;
    mouseOldX = -1;
    mouseOldY = -1;
    rotacaoX = 0.0;
    rotacaoY = 0.0;
    rotacaoZ = 0.0;

    baseAngle = 0.0f;
    armAngle = 45.0f;
    headAngleY = 0.0f;
    headAngleX = 0.0f;
    lampHeight = 5.0f;

    posLampX = 0.0f;
    posLampY = 0.0f;
    posLampZ = 0.0f;
    rotationLamp = 0.0;

    anguloLuzX = 0.0;
    anguloLuzY = 0.0;
    anguloLuzZ = 0.0;

}

void configuraIluminacao() {
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0); 
    glEnable(GL_DEPTH_TEST);

    GLfloat luzAmbiente[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    
}

void configuraMaterialHaste(){
    GLfloat materialDifuso[] = {corHaste[0], corHaste[1], corHaste[2], corHaste[3]};    // vermelho
    GLfloat materialAmbiente[] = {corHaste[0], corHaste[1], corHaste[2], corHaste[3]};  // vermelho escuro
    GLfloat materialEspecular[] = {corHaste[0], corHaste[1], corHaste[2], corHaste[3]}; 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);

    glMaterialf(GL_FRONT, GL_SHININESS, 30); // brilho medio
}

void configuraMaterialConec(){
    GLfloat materialDifuso[] = {corConec[0], corConec[1], corConec[2], corConec[3]};    // vermelho
    GLfloat materialAmbiente[] = {corConec[0], corConec[1], corConec[2], corConec[3]};  // vermelho escuro
    GLfloat materialEspecular[] = {corConec[0], corConec[1], corConec[2], corConec[3]}; 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);

    glMaterialf(GL_FRONT, GL_SHININESS, 30); // brilho medio

}

void configuraMaterialLamp(){
    GLfloat materialDifuso[] = {corLamp[0], corLamp[1], corLamp[2], corLamp[3]};    // vermelho
    GLfloat materialAmbiente[] = {corLamp[0], corLamp[1], corLamp[2], corLamp[3]};  // vermelho escuro
    GLfloat materialEspecular[] = {corLamp[0], corLamp[1], corLamp[2], corLamp[3]}; 

    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);

    glMaterialf(GL_FRONT, GL_SHININESS, 30); // brilho medio

}

void calculateJumpPosition() {
    if (isJumping) {
        float elapsedTime = (glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000.0f; // Convert ms to seconds
        float normalizedTime = fmod(elapsedTime, jumpDuration) / jumpDuration; // Normalize to 0..1 loop

        // Sine wave for smooth up and down movement
        jumpYPos = jumpHeight * sin(M_PI * normalizedTime); // sine wave for vertical motion

        // Move the lamp based on the direction of movement (WASD keys)
        if (isMovingForward) {
            posLampZ -= 0.05f;  // Move forward (decrease Z)
        } else if (isMovingBackward) {
            posLampZ += 0.05f;  // Move backward (increase Z)
        } else if (isMovingLeft) {
            posLampX -= 0.05f;  // Move left (decrease X)
        } else if (isMovingRight) {
            posLampX += 0.05f;  // Move right (increase X)
        }

        // Stop jumping after one full cycle (up + down)
        if (elapsedTime >= jumpDuration) {
            isJumping = false; // Stop jumping
            isMovingForward = false;
            isMovingBackward = false;
            isMovingLeft = false;
            isMovingRight = false;
        }
    }
}



void animateJump(int value) {
    if (isJumping) {
        calculateJumpPosition(); // Update the jump position
        glutPostRedisplay(); // Redraw the scene
        glutTimerFunc(16, animateJump, 0); // Call again for smooth animation
    }
}

// Function to start the jump
void startJump() {
    if (!isJumping) {
        isJumping = true;
        startTime = glutGet(GLUT_ELAPSED_TIME); // Record the start time
        jumpYPos = 0.0f; // Reset vertical position at the start of the jump
        glutTimerFunc(16, animateJump, 0); // Start the jump animation
    }
}


void desenhaCilindro(float baseRadius, float topRadius, float height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, baseRadius, topRadius, height, slices, slices);

    if (baseRadius > 0.0f){
        glPushMatrix();
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        gluDisk(quad, 0.0f, baseRadius, slices, 1);
        glPopMatrix();
    }

    gluDeleteQuadric(quad);
}

// Draw a sphere (used for joints and lamp head)
void desenhaEsfera(float raio, int fatias, int stacks) {
    glutSolidSphere(raio, fatias, stacks);
}

void desenhaBaseLuminaria(float tamanhoBase, float altura) {
    glPushMatrix();
        configuraMaterialConec();
        glRotatef(90, 1, 0, 0);
        desenhaCilindro(tamanhoBase, tamanhoBase, altura, 100);
    glPopMatrix();
}

void desenhaPrimeiroBraco() {
    glPushMatrix();
        configuraMaterialHaste();
        glTranslatef(0.0f, 1.3f, -1.2f); // Offset above the base
        glRotatef(armAngle, 1.0f, 0.0f, 0.0f);
        desenhaCilindro(0.2f, 0.2f, 2.0f, 20); // Arm cylinder
        glTranslatef(0.0f, 0.0f, 0.0f); // Move to the top of the arm
        configuraMaterialConec();
        desenhaEsfera(0.3f, 20, 20); // Joint sphere
    glPopMatrix();
}

void desenhaBulbo(){
    glPushMatrix();
        configuraMaterialLamp();
        glTranslatef(0.0, 0.0, 1.0);
        desenhaEsfera(0.475, 20, 20);
    glPopMatrix();
}

void desenhaSegundoBraco() {
    glPushMatrix();
        glPushMatrix();
            configuraMaterialHaste();
            glTranslatef(0.0f, 1.5f, -1.1f); // Offset above the first arm
            glRotatef(-armAngle, 1.0f, 0.0f, 0.0f);
            desenhaCilindro(0.2f, 0.2f, 1.5f, 20); // Second arm cylinder
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, 2.65f, 0.1f); // Move to the top of the second arm
            configuraMaterialConec();
            desenhaEsfera(0.3f, 20, 20); // Joint sphere
        glPopMatrix();
        glPushMatrix();
            configuraMaterialHaste();
            glTranslatef(0.0f, 3.15f, -0.3f);
            glRotatef(headAngleY, 0.0, 1.0, 0.0);
            glRotatef(headAngleX, 1.0, 0.0, 0.0);
            desenhaCilindro(0.4f, 0.7f, 1.5f, 20); // Second arm cylinder
            desenhaBulbo();
        glPopMatrix();
    glPopMatrix();
}


void carregarTextura(const char* nomeArquivo, int indice)
{
    int largura, altura, canais;

    unsigned char *dados = stbi_load(nomeArquivo, &largura, &altura, &canais, 0);

    if (!dados){
    fprintf(stderr, "Erro ao carregar a textura: %s\n", nomeArquivo);
    exit(1);}

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
    

    //Luz global
    configuraIluminacao();

    carregarTextura("texturas/madeira_mesa.jpg", 0);
    carregarTextura("texturas/madeira.png", 1);
    carregarTextura("texturas/porcelain_2.jpeg", 2);
    carregarTextura("texturas/parede_norte.jpg", 3);
    carregarTextura("texturas/parede_leste.jpg", 4);
    carregarTextura("texturas/parede_oeste.jpg", 5);
    carregarTextura("texturas/chao.jpg", 6);
    carregarTextura("texturas/teto.jpg", 7);

    //glEnable(GL_LIGHTING);
}

// Function to draw a small plate
void drawPlate(float radius, float thickness) {
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0); // White plate color
        glTranslatef(0.0f, -2, 0.0f); // Move plate down to the right position
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idsTextura[2]);             
        
        GLUquadric* quad = gluNewQuadric();
        // Draw a disk with inner radius 0.0 (center) and outer radius as plate radius
        gluDisk(quad, 0.0f, radius, 60, 1);  // Outer radius = plate radius, 60 slices for smoothness
        gluDeleteQuadric(quad);
        glPopMatrix();
        
        // Add thickness to the plate by creating a thin cylinder
        glTranslatef(0.0f, 0.0f, thickness);  // Move up to create thickness
        glColor3f(1.0, 1.0, 1.0); // Plate color
        gluCylinder(quad, radius, radius, thickness, 60, 1);  // Add a cylindrical thickness layer
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


// Function to draw the teacup
void drawTeacup(float radius, float height, float handleRadius) {
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0); // White teacup color
        
        // Body of the teacup (cylinder)
        glPushMatrix();
            glTranslatef(0.0f, height / 2.0f, 0.0f); // Move teacup up to its position
            GLUquadric* quad = gluNewQuadric();
            gluCylinder(quad, radius, radius, height, 30, 30); // Teacup body
            gluDeleteQuadric(quad);
        glPopMatrix();
        
        // Base of the teacup (disk)
        glPushMatrix();
            glTranslatef(0.0f, -height / 2.0f, 0.0f); // Move base to the bottom of the teacup
            GLUquadric* quadBase = gluNewQuadric();
            gluDisk(quadBase, 0.0f, radius, 30, 1); // Teacup base
            gluDeleteQuadric(quadBase);
        glPopMatrix();
        
        // Handle of the teacup (sphere and small cylinders)
        glPushMatrix();
            glTranslatef(radius * 1.3f, 0.0f, 0.0f); // Move the handle to the side
            glRotatef(90, 0.0f, 1.0f, 0.0f); // Rotate to make the handle horizontal
            glColor3f(1.0, 1.0, 1.0); // White color for the handle
            glutSolidTorus(handleRadius / 2.0f, handleRadius, 10, 30); // Draw handle using a torus
        glPopMatrix();
        
    glPopMatrix();
}
void desenhaQuarto(float tamanho){
    glPushMatrix();
        //Parede Norte
        glBindTexture(GL_TEXTURE_2D, idsTextura[3]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0); glVertex3f(-tamanho, tamanho, -tamanho);
            glTexCoord2f(0.0, 0.0); glVertex3f(tamanho, tamanho, -tamanho);
            glTexCoord2f(0.0, 1.0); glVertex3f(tamanho, -tamanho * 0.4, -tamanho);
            glTexCoord2f(1.0, 1.0); glVertex3f(-tamanho, -tamanho * 0.4, -tamanho);
        glEnd();

        //Parede Oeste
        glBindTexture(GL_TEXTURE_2D, idsTextura[5]);
        glBegin(GL_QUADS);
        
            glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, tamanho, -tamanho);
            glTexCoord2f(0.0, 0.0); glVertex3f(tamanho, tamanho, tamanho);
            glTexCoord2f(0.0, 1.0); glVertex3f(tamanho, -tamanho * 0.4, tamanho);
            glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, -tamanho * 0.4, -tamanho);
        glEnd();

        //Parede Leste
        glBindTexture(GL_TEXTURE_2D, idsTextura[4]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0); glVertex3f(-tamanho, tamanho, -tamanho);
            glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, tamanho, tamanho);
            glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho * 0.4, tamanho);
            glTexCoord2f(1.0, 1.0); glVertex3f(-tamanho, -tamanho * 0.4, -tamanho);
        glEnd();

        //chao
        glBindTexture(GL_TEXTURE_2D, idsTextura[6]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0); glVertex3f(-tamanho, -tamanho * 0.4, -tamanho);
            glTexCoord2f(0.0, 0.0); glVertex3f(tamanho, -tamanho * 0.4, -tamanho);
            glTexCoord2f(0.0, 1.0); glVertex3f(tamanho, -tamanho * 0.4, tamanho);
            glTexCoord2f(1.0, 1.0); glVertex3f(-tamanho, -tamanho * 0.4, tamanho);
        glEnd();

        //teto
        glBindTexture(GL_TEXTURE_2D, idsTextura[7]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0); glVertex3f(-tamanho, tamanho, -tamanho);
            glTexCoord2f(0.0, 0.0); glVertex3f(tamanho, tamanho, -tamanho);
            glTexCoord2f(0.0, 1.0); glVertex3f(tamanho, tamanho, tamanho);
            glTexCoord2f(1.0, 1.0); glVertex3f(-tamanho, tamanho, tamanho);
        glEnd();

        //Parede Sul
        glBindTexture(GL_TEXTURE_2D, idsTextura[3]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0); glVertex3f(-tamanho, tamanho, tamanho);
            glTexCoord2f(0.0, 0.0); glVertex3f(tamanho, tamanho, tamanho);
            glTexCoord2f(0.0, 1.0); glVertex3f(tamanho, -tamanho * 0.4, tamanho);
            glTexCoord2f(1.0, 1.0); glVertex3f(-tamanho, -tamanho * 0.4, tamanho);
        glEnd();
    glPopMatrix();
}
void desenhaPerna(float x, float z, float tamanho, float altura, float largura) {
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);

    // Frente
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(x, -tamanho, z);
        glTexCoord2f(1.0, 0.0); glVertex3f(x + largura, -tamanho - 1, z);
        glTexCoord2f(1.0, 1.0); glVertex3f(x + largura, -tamanho - 1 - altura, z);
        glTexCoord2f(0.0, 1.0); glVertex3f(x, -tamanho- 1- altura, z);
    glEnd();

    // Trás
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(x, -tamanho- 1 , z + largura);
        glTexCoord2f(1.0, 0.0); glVertex3f(x + largura, -tamanho- 1, z + largura);
        glTexCoord2f(1.0, 1.0); glVertex3f(x + largura, -tamanho - 1- altura, z + largura);
        glTexCoord2f(0.0, 1.0); glVertex3f(x, -tamanho - 1 - altura, z + largura);
    glEnd();

    // Lado esquerdo
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(x, -tamanho - 1, z);
        glTexCoord2f(1.0, 0.0); glVertex3f(x, -tamanho - 1 , z + largura);
        glTexCoord2f(1.0, 1.0); glVertex3f(x, -tamanho - 1 - altura, z + largura);
        glTexCoord2f(0.0, 1.0); glVertex3f(x, -tamanho - 1 - altura, z);
    glEnd();

    // Lado direito
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(x + largura, -tamanho - 1, z);
        glTexCoord2f(1.0, 0.0); glVertex3f(x + largura, -tamanho - 1, z + largura);
        glTexCoord2f(1.0, 1.0); glVertex3f(x + largura, -tamanho - 1 - altura, z + largura);
        glTexCoord2f(0.0, 1.0); glVertex3f(x + largura, -tamanho  - 1 - altura, z);
    glEnd();

    // Topo
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(x, -tamanho - 1 , z);
        glTexCoord2f(1.0, 0.0); glVertex3f(x + largura, -tamanho - 1, z);
        glTexCoord2f(1.0, 1.0); glVertex3f(x + largura, -tamanho - 1, z + largura);
        glTexCoord2f(0.0, 1.0); glVertex3f(x, -tamanho - 1, z + largura);
    glEnd();

    // Base
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(x, -tamanho - 1 - altura, z);
        glTexCoord2f(1.0, 0.0); glVertex3f(x + largura, -tamanho - 1 - altura, z);
        glTexCoord2f(1.0, 1.0); glVertex3f(x + largura, -tamanho  - 1 - altura, z + largura);
        glTexCoord2f(0.0, 1.0); glVertex3f(x, -tamanho - 1 - altura, z + largura);
    glEnd();
}

void desenhaBase(float tamanho, float espessura){
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    //Cima
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, -tamanho);
        glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho, -tamanho);
        glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, -tamanho, tamanho);
        glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho, tamanho);
    glEnd();

    //Baixo
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho - espessura, -tamanho);
        glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho - espessura, -tamanho);
        glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, -tamanho - espessura, tamanho);
        glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho - espessura, tamanho);
    glEnd();

    //Frente
    glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-tamanho, -tamanho, tamanho);
        glTexCoord2f(1.0, 0.0); glVertex3f(tamanho, -tamanho, tamanho);
        glTexCoord2f(1.0, 1.0); glVertex3f(tamanho, -tamanho - espessura, tamanho);
        glTexCoord2f(0.0, 1.0); glVertex3f(-tamanho, -tamanho - espessura, tamanho);
    glEnd();


}

void desenhaMesaComPernas(float tamanho, float espessura, float alturaPerna, float larguraPerna) {
    desenhaBase(tamanho, espessura);

    // Adiciona as pernas nas extremidades
    desenhaPerna(-tamanho, -tamanho, tamanho, alturaPerna, larguraPerna); // Perna esquerda frontal
    desenhaPerna(tamanho - larguraPerna, -tamanho, tamanho, alturaPerna, larguraPerna); // Perna direita frontal
    desenhaPerna(-tamanho, tamanho - larguraPerna, tamanho, alturaPerna, larguraPerna); // Perna esquerda traseira
    desenhaPerna(tamanho - larguraPerna, tamanho - larguraPerna, tamanho, alturaPerna, larguraPerna); // Perna direita traseira
}
void reshape(int largura, int altura) {
 glViewport(0, 0, largura, altura);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 gluPerspective(65.0, (float)largura / (float)altura, 1.0, 100.0);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void animateLamp(int value) {
    if (isAnimating) {
        float elapsedTime = (glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000.0f; // Convert milliseconds to seconds
        if (elapsedTime < animationDuration) {
            // Perform the animation (e.g., rotating the lamp)
            rotationLamp += 5.0f; // Increment the rotation angle
            if (rotationLamp >= 360.0f) {
                rotationLamp -= 360.0f; // Reset the rotation to avoid overflow
            }

            // Request another timer call
            glutTimerFunc(16, animateLamp, 0); // 16 ms for ~60 FPS
        } else {
            // Stop the animation after the specified duration
            isAnimating = false;
        }
    }
    glutPostRedisplay(); // Redraw the scene
}

// Function to start the animation
void startAnimation() {
    if (!isAnimating) {
        isAnimating = true;
        startTime = glutGet(GLUT_ELAPSED_TIME); // Record the start time
        glutTimerFunc(16, animateLamp, 0); // Start the animation
    }
}

void desenhaLampada(){
    glTranslatef(posLampX, posLampY + jumpYPos, posLampZ);
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
              rotationCamX, rotationCamY, 0.0,
              0.0, 1.0, 0.0);

    glPushMatrix();
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        desenhaQuarto(40);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glColor3f(0.3, 0.3, 0.3);
            glScalef(20.0, 0.2,20.0);
            desenhaMesaComPernas(0.8, 3, 75, 0.2);
            //Desabilitando texturas para evitar interferência nas cores
            glDisable(GL_TEXTURE_2D);
            
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0 + posLampX, 0.3, 0.0 + posLampZ);          
            desenhaLampada();
        glPopMatrix();

        glPushMatrix();
            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, idsTextura[2]);
                glEnable(GL_TEXTURE_2D);
                glColor3f(1.0, 1.0, 1.0);
                glTranslatef(teapotPos[0], teapotPos[1], teapotPos[2]);
                glutSolidTeapot(sizeTeapot);
            glPopMatrix();

            //glPushMatrix();
            //    glTranslatef(-3.5f, 0.3f, 1.0f);
            //    drawPlate(2.0f, 0.5f);
            //glPopMatrix();
            //glTranslatef(3.0f, 1.0f, 0.0f);
            //drawTeacup(0.6f, 1.0f, 0.2f);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();  
       
        
    glPopMatrix();


    glutSwapBuffers();
}

void tecladoEspecial(int tecla, int x, int y){
    switch(tecla){
        case GLUT_KEY_END:
            if (headAngleX <= 30){
                headAngleX += 5;
            }
            break;  
            
        case GLUT_KEY_PAGE_UP:
            if (headAngleX >= -40){
                headAngleX -= 5;
            }
            break;

        case GLUT_KEY_HOME:
            if (headAngleY >= -45){
                headAngleY -= 5;
            }
            break;
        
        case GLUT_KEY_PAGE_DOWN:
            if (headAngleY <= 45){
                headAngleY += 5;
            }
            break;

        case GLUT_KEY_UP:
            if (yCam <= 35)
            yCam += 0.5;
            break;  
            
        case GLUT_KEY_DOWN:
            if (yCam >= -15)
            yCam -= 0.5;
            break;  

        case GLUT_KEY_LEFT:
            if (xCam <= 40)
            xCam -= 0.5;
            break;
        
        case GLUT_KEY_RIGHT:
            if (xCam >= -40)
            xCam += 0.5;
            break;
    }
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y){
    switch(tecla){
            case 'w':
            isMovingForward = true;
            isMovingBackward = false;
            isMovingLeft = false;
            isMovingRight = false;
            startJump();
            break;
        
        case 'a':
            isMovingLeft = true;
            isMovingForward = false;
            isMovingBackward = false;
            isMovingRight = false;
            startJump();
            break;
        
        case 'd':
            isMovingRight = true;
            isMovingForward = false;
            isMovingBackward = false;
            isMovingLeft = false;
            startJump();
            break;
        
        case 's':
            isMovingBackward = true;
            isMovingForward = false;
            isMovingLeft = false;
            isMovingRight = false;
            startJump();
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
        
        case 'p':
            startJump();
            break;
        
        case 'r':
            resetAllGlobals();
            break;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (isMousePressed) {
        // Calcula a variacao da posição de X
        int deltaX = x - mouseOldX;
        int deltaY = y - mouseOldY;

        // Atualiza rotação da camera
        rotationCamX += deltaX * 0.1f;  // Rotate the camera horizontally
        rotationCamY += deltaY * 0.1f; //Rotaciona a camera verticalmente

        if (rotationCamY > 90.0f) rotationCamY = 90.0f;
        if (rotationCamY < -90.0f) rotationCamY = -90.0f;
        

        // Update old mouse position
        mouseOldX = x;
        mouseOldY = y;

        glutPostRedisplay();  // Redraw the scene
    }
}

// Function to handle mouse button clicks (activate/deactivate camera movement)
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMousePressed = true;  // Activate camera movement when left button is pressed
            mouseOldX = x;          // Store the current mouse position
            mouseOldY = y;
        } else if (state == GLUT_UP) {
            isMousePressed = false;  // Deactivate camera movement when left button is released
        }
    }
}
void menu(int opcao){
    switch (opcao){
        case 1:
            jumpHeight = 2.0f;
            break;
        case 2:
            jumpHeight = 1.0f;
            break;
        case 3:
            jumpHeight = 0.5f;
            break;
        case 4:
            jumpDuration = 0.5f;
            break;
        case 5:
            jumpDuration = 1.0f;
            break;
        case 6:
            jumpDuration = 2.0f;
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

    int submenuAltura = glutCreateMenu(menu);
    glutAddMenuEntry("Alto", 1);
    glutAddMenuEntry("Normal", 2);
    glutAddMenuEntry("Baixo", 3);

    int submenuDuracao = glutCreateMenu(menu);
    glutAddMenuEntry("Rapido", 4);
    glutAddMenuEntry("Normal", 5);
    glutAddMenuEntry("Lento", 6);

    glutCreateMenu(menu);
    glutAddSubMenu("Altura Salto", submenuAltura);
    glutAddSubMenu("Duracao Salto", submenuDuracao);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSpecialFunc(tecladoEspecial);
    glutKeyboardFunc(teclado);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    glutMainLoop();
    return 0;
}
