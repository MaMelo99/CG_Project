#include <GL/freeglut.h>
#include <stb_image.h>

float anguloLuzX = 0.0;
float anguloLuzY = 0.0;
float anguloLuzZ = 0.0;

void inicializa() {
 glEnable(GL_DEPTH_TEST); // ativa teste de profundidade
 glEnable(GL_LIGHTING); // ativa a iluminacao
 glEnable(GL_LIGHT0); // ativa a luz 0

 // configuracao da luz
 GLfloat luzAmbiente[] = {0.2, 0.2, 0.2, 1.0}; // rgb + opacidade
 GLfloat luzDifusa[] = {0.8, 0.8, 0.8, 1.0};
 GLfloat luzEspecular[] = {1.0, 1.0, 1.0, 1.0};
 GLfloat posicaoLuz[] = {1.0, 1.0, 1.0, 0.0}; // 0: luz direcional 1: pontual

 glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
 glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
 glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

 // configuracao do material
 GLfloat materialAmbiente[] = {0.3, 0.3, 0.3, 1.0};
 GLfloat materialDifuso[] = {0.0, 0.0, 1.0, 1.0};      // azul
 GLfloat materialEspecular[] = {1.0, 1.0, 1.0, 1.0};
 GLfloat materialBrilho[] = {50.0};                    // brilho medio

 glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
 glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
 glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);
 glMaterialfv(GL_FRONT, GL_SHININESS, materialBrilho);

 glClearColor(0.1, 0.1, 0.1, 1.0); // cor de fundo
}

void display() {
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glPushMatrix();

   GLfloat posicaoLuz[] = {1.0, 1.0, 1.0, 0.0}; // 0: luz direcional 1: pontual
   glRotatef(anguloLuzX, 1.0, 0.0, 0.0);
   glRotatef(anguloLuzY, 0.0, 1.0, 0.0);
   glRotatef(anguloLuzZ, 0.0, 0.0, 1.0);
   glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

   glutSolidSphere(1.0, 50, 50);

 glPopMatrix();

 glutSwapBuffers();
}

void reshape(int largura, int altura) {
 glViewport(0, 0, largura, altura);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 gluPerspective(45.0, (float)largura / (float)altura, 1.0, 100.0);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 // camera
 gluLookAt(0.0, 0.0, 5.0,   // posicao da camera
           0.0, 0.0, 0.0,   // para onde olhar
           0.0, 1.0, 0.0);  // vetor "para cima"
}

void tecladoEspecial(int tecla, int x, int y)
{
 if (tecla == GLUT_KEY_UP)
  anguloLuzX -= 5.0;
 if (tecla == GLUT_KEY_DOWN)
  anguloLuzX += 5.0;
 if (tecla == GLUT_KEY_LEFT)
  anguloLuzY -= 5.0;
 if (tecla == GLUT_KEY_RIGHT)
  anguloLuzY += 5.0;
 if (tecla == GLUT_KEY_PAGE_UP)
  anguloLuzZ -= 5.0;
 if (tecla == GLUT_KEY_PAGE_DOWN)
  anguloLuzZ += 5.0;

 glutPostRedisplay();
}
 
int main(int argc, char** argv) {
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(800, 600);
 glutCreateWindow("Iluminacao");

 inicializa();

 glutDisplayFunc(display);
 glutReshapeFunc(reshape);
 glutSpecialFunc(tecladoEspecial);
  
 glutMainLoop();

 return 0;
}