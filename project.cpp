#include <GL/glut.h>
#include <cmath>
//#include <GL/gl.h> -> For Windows
//#include <GL/glu.h> -> For Windows
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

float yCam = 5.0;
float xCam = 0.0;
float zCam = 20.0;
float rotationCam = 0.0;

int mouseOldX = -1, mouseOldY = -1;
bool isMousePressed = false;  

GLuint idsTextura[3];
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

GLfloat corHaste[] = {0.8, 0.8, 0.8};
GLfloat corConec[] = {0.5, 0.5, 0.5};
GLfloat corLamp[] = {1.0f, 0.9f, 0.6f};

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


int moveMode = 1;

bool checkCollision() {
    // Calculate the distance between the lamp and the teapot in 3D space
    float distanceX = fabs(posLampX - teapotPos[0]);
    float distanceZ = fabs(posLampZ - teapotPos[2]);

    // If the lamp is within the radius of the teapot in the X and Z axis, there's a collision
    if (distanceX <= sizeTeapot && distanceZ <= sizeTeapot && posLampY <= teapotPos[1] + jumpHeight) {
        return true;  // Collision detected
    }

    return false;  // No collision
}

void calculateJumpPosition() {
    if (isJumping) {
        float elapsedTime = (glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000.0f; // Convert ms to seconds
        float normalizedTime = fmod(elapsedTime, jumpDuration) / jumpDuration; // Normalize to 0..1 loop

        // Sine wave for smooth up and down movement
        jumpYPos = jumpHeight * sin(M_PI * normalizedTime); // sine wave for vertical motion

        // Move the lamp based on the direction of movement (WASD keys)
        if (isMovingForward && !checkCollision()) {
            posLampZ -= 0.05f;  // Move forward (decrease Z)
        } else if (isMovingBackward && !checkCollision()) {
            posLampZ += 0.05f;  // Move backward (increase Z)
        } else if (isMovingLeft && !checkCollision()) {
            posLampX -= 0.05f;  // Move left (decrease X)
        } else if (isMovingRight && !checkCollision()) {
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

void configuraIluminacao() {
    GLfloat luzAmbiente[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Soft ambient light
    GLfloat luzDifusa[] = {1.0f, 1.0f, 1.0f, 1.0f};  // White diffuse light
    GLfloat luzPosicao[] = {5.0f, 5.0f, 5.0f, 1.0f};  // Light source position

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
    glEnable(GL_LIGHTING);  // Enable lighting
    glEnable(GL_LIGHT0);    // Enable light source
}

void inicializa(){
    glClearColor(0.0, 0.0, 0.2, 1.0);
    
    glEnable(GL_DEPTH_TEST);

    //Luz global
    configuraIluminacao();

    carregarTextura("texturas/madeira_mesa.jpg", 0);
    carregarTextura("texturas/porcelain.jpeg", 1);
    carregarTextura("texturas/porcelain_2.jpeg", 2);

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
        case GLUT_KEY_DOWN:
            if (headAngleX <= 30){
                headAngleX += 5;
            }
            break;  
            
        case GLUT_KEY_UP:
            if (headAngleX >= -40){
                headAngleX -= 5;
            }
            break;

        case GLUT_KEY_LEFT:
            if (headAngleY >= -45){
                headAngleY -= 5;
            }
            break;
        
        case GLUT_KEY_RIGHT:
            if (headAngleY <= 45){
                headAngleY += 5;
            }
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
        
        case 'm':
            moveMode = moveMode ^ 1;
            break;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (isMousePressed) {
        // Calculate change in mouse position
        int deltaX = x - mouseOldX;
        int deltaY = y - mouseOldY;

        // Update camera rotation based on mouse movement
        rotationCam += deltaX * 0.1f;  // Rotate the camera horizontally
        yCam -= deltaY * 0.05f;        // Move camera vertically (up/down)

        if (rotationCam > 90.0f) rotationCam = 90.0f;
        if (rotationCam < -90.0f) rotationCam = -90.0f;

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
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    glutMainLoop();
    return 0;
}
