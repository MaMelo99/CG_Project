#include <GL/glut.h>

float anguloSol = 0.0;
float anguloMercurio = 0.0;
float anguloVenus = 0.0;
float anguloTerra = 0.0;
float anguloMarte = 0.0;

float anguloMercurioRotacao = 0.0;
float anguloVenusRotacao = 0.0;
float anguloTerraRotacao = 0.0;
float anguloMarteRotacao = 0.0;

float cameraPosZ = -20.0;
float velocidadeOrbita = 1.0;
float velocidadeRotacao = 1.0;

void inicializa() {
 glClearColor(0.0, 0.0, 0.0, 1.0);
 glEnable(GL_DEPTH_TEST);

 // ativar a iluminacao
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);

 // configurar a luz do sol
 GLfloat luzAmbiente[] = {0.1, 0.1, 0.1, 1.0};   // luz suave
 GLfloat luzDifusa[] = {1.0, 1.0, 1.0, 1.0};     // luz branca
 GLfloat luzPosicao[] = {0.0, 0.0, 0.0, 1.0};    // luz pontual

 glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
 glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
}

void desenhaEsfera(GLfloat raio)
{
 glutSolidSphere(raio, 50, 50);   
}

void display() {
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();

 glTranslatef(0.0, 0.0, cameraPosZ);

 // sol
 glPushMatrix();
   glRotatef(anguloSol, 0.0, 1.0, 0.0);

   GLfloat solDifusa[] = {1.0, 1.0, 0.0, 1.0};
   GLfloat solAmbiente[] = {0.2, 0.2, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, solDifusa);
   glMaterialfv(GL_FRONT, GL_AMBIENT, solAmbiente);

   desenhaEsfera(3.0);
 glPopMatrix();

 // mercurio
 glPushMatrix();

   GLfloat mercurioDifusa[] = {0.7, 0.7, 0.7, 1.0};
   GLfloat mercurioAmbiente[] = {0.2, 0.2, 0.2, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mercurioDifusa);
   glMaterialfv(GL_FRONT, GL_AMBIENT, mercurioAmbiente);

   glRotatef(anguloMercurio * velocidadeOrbita, 0.0, 1.0, 0.0);
   glTranslatef(6.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloMercurioRotacao * velocidadeRotacao, 0.0, 1.0, 0.0);
     desenhaEsfera(0.3);
   glPopMatrix();  
 glPopMatrix();

 // venus
 glPushMatrix();

   GLfloat venusDifusa[] = {0.8, 0.5, 0.0, 1.0};
   GLfloat venusAmbiente[] = {0.2, 0.1, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, venusDifusa);
   glMaterialfv(GL_FRONT, GL_AMBIENT, venusAmbiente);

   glRotatef(anguloVenus * velocidadeOrbita, 0.0, 1.0, 0.0);
   glTranslatef(9.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloVenusRotacao * velocidadeRotacao, 0.0, 1.0, 0.0);
     desenhaEsfera(0.7);
   glPopMatrix();  
 glPopMatrix();

 // terra
 glPushMatrix();

   GLfloat terraDifusa[] = {0.0, 0.0, 1.0, 1.0};
   GLfloat terraAmbiente[] = {0.0, 0.0, 0.1, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, terraDifusa);
   glMaterialfv(GL_FRONT, GL_AMBIENT, terraAmbiente);

   glRotatef(anguloTerra * velocidadeOrbita, 0.0, 1.0, 0.0);
   glTranslatef(12.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloTerraRotacao * velocidadeRotacao, 0.0, 1.0, 0.0);
     desenhaEsfera(1.0);
   glPopMatrix();  
 glPopMatrix();

 // marte
 glPushMatrix();

   GLfloat marteDifusa[] = {1.0, 0.0, 0.0, 1.0};
   GLfloat marteAmbiente[] = {0.1, 0.0, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, marteDifusa);
   glMaterialfv(GL_FRONT, GL_AMBIENT, marteAmbiente);

   glRotatef(anguloMarte * velocidadeOrbita, 0.0, 1.0, 0.0);
   glTranslatef(15.0, 0.0, 0.0);
   glPushMatrix();
     glRotatef(anguloMarteRotacao * velocidadeRotacao, 0.0, 1.0, 0.0);
     desenhaEsfera(0.8);
   glPopMatrix();  
 glPopMatrix();

 glutSwapBuffers();
}

void reshape(int largura, int altura) {
 glViewport(0, 0, largura, altura);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 gluPerspective(65.0, (float)largura / (float)altura, 1.0, 50.0);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void atualiza(int valor)
{
 anguloSol += 0.2;

 anguloMercurio += 1.5 * velocidadeOrbita;
 anguloMercurioRotacao += 5.0 * velocidadeRotacao;

 anguloVenus += 1.2 * velocidadeOrbita;
 anguloVenusRotacao += 2.0 * velocidadeRotacao;

 anguloTerra += 0.8 * velocidadeOrbita;
 anguloTerraRotacao += 1.0 * velocidadeRotacao;

 anguloMarte += 0.6 * velocidadeOrbita;
 anguloMarte += 0.8 * velocidadeRotacao;

 glutPostRedisplay();
 glutTimerFunc(16, atualiza, 0);   
}

void teclado(unsigned char tecla, int x, int y)
{
 switch (tecla)
 {
  case 27:
    exit(0);
    break;
  case 'w':
    cameraPosZ += 1;
    glutPostRedisplay();
    break;
  case 's':
    cameraPosZ -= 1;
    glutPostRedisplay();
    break;
  case 'a':
    velocidadeOrbita += 0.1;
    glutPostRedisplay();
    break;
  case 'd':
    if (velocidadeOrbita >= 0.2)
     velocidadeOrbita -= 0.1;
    glutPostRedisplay();
    break;
  case 'q':
    velocidadeRotacao += 0.1;
    glutPostRedisplay();
    break;
  case 'e':
    if (velocidadeRotacao >= 0.2)
     velocidadeRotacao -= 0.1;
    glutPostRedisplay();
    break;

 }   
}

void mouse(int botao, int estado, int x, int y)
{ 
 if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN)
 {
   cameraPosZ = -20;
   glutPostRedisplay();
 }
}
 
int main(int argc, char** argv) {
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(800, 600);
 glutCreateWindow("Sistema Solar");

 inicializa();

 glutDisplayFunc(display);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(teclado);
 glutMouseFunc(mouse);

 glutTimerFunc(1000, atualiza, 0);   
  
 glutMainLoop();

 return 0;
}