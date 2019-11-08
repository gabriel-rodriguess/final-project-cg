#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

GLfloat angle, fAspect;
GLdouble obsX=0, obsY=-150, obsZ=400;


void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    
    //NUMERO PLAYER 1
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(-130.0,180.0,0.0);

    glPushMatrix();
    glTranslated(0.0,25.0,0.0);
    glScaled(5.0,1.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0,-25.0,0.0);
    glScaled(5.0,1.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(10.0,0.0,0.0);
    glScaled(1.0,10.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-10.0,0.0,0.0);
    glScaled(1.0,10.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPopMatrix();

    //NUMERO PLAYER 2
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(130.0,180.0,0.0);

    glPushMatrix();
    glTranslated(0.0,25.0,0.0);
    glScaled(5.0,1.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0,-25.0,0.0);
    glScaled(5.0,1.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(10.0,0.0,0.0);
    glScaled(1.0,10.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-10.0,0.0,0.0);
    glScaled(1.0,10.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glPopMatrix();

    //BARRAS
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(0.0,130.0,0.0);
    glScaled(500.0,1.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(0.0,-130.0,0.0);
    glScaled(500.0,1.0,1.0);
    glutSolidCube(5.0f);
    glPopMatrix();
    
    glColor3f(0.0f, 0.0f, 1.0f);

    //PLAYERS
    glPushMatrix();
    glTranslated(180.0,0.0,0.0);
    glScaled(1.0,3.0,1.0);
    glutSolidCube(25.0f);
    glPopMatrix();


    glColor3f(1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslated(-180.0,0.0,0.0);
    glScaled(1.0,3.0,1.0);
    glutSolidCube(25.0f);
    glPopMatrix();

    //BOLA
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(15.0,50.0,50.0);

    glutSwapBuffers();
}


void Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    angle = 45;
}


void PosicionaObservador(void)
{

    glMatrixMode(GL_MODELVIEW);
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
}


void EspecificaParametrosVisualizacao(void)
{

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();


    gluPerspective(angle, fAspect, 0.5, 500);

    PosicionaObservador();
}


void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{

    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);


    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}


void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN)
        { // Zoom-in
            if (angle >= 10)
                angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN)
        { // Zoom-out
            if (angle <= 130)
                angle += 5;
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        obsX -= 10;
        break;
    case GLUT_KEY_RIGHT:
        obsX += 10;
        break;
    case GLUT_KEY_UP:
        obsY += 10;
        break;
    case GLUT_KEY_DOWN:
        obsY -= 10;
        break;

    case GLUT_KEY_HOME:
        obsZ += 10;
        break;
    case GLUT_KEY_END:
        obsZ -= 10;
        break;
    }
    glLoadIdentity();
    gluLookAt(obsX, obsY, obsZ, 0, 0, 0, 0, 1, 0);
    //gluLookAt(obsX, obsY, obsZ, 0, 0, 0, 0, 1, 0);
    glutPostRedisplay();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PONG");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(SpecialKeys);
    Inicializa();
    glutMainLoop();
}