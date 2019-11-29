#include <GL/glut.h>

void numero(int player, int num)
{
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
        if(player == 1)
            glTranslated(-130.0,180.0,0.0);
        if(player == 2)
            glTranslated(130.0,180.0,0.0);
    
    switch (num)
    {
    case 0:
        glPushMatrix();
        glTranslated(0.0, 25.0, 0.0);
        glScaled(5.0, 1.0, 1.0);
        glutSolidCube(5.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslated(0.0, -25.0, 0.0);
        glScaled(5.0, 1.0, 1.0);
        glutSolidCube(5.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslated(10.0, 0.0, 0.0);
        glScaled(1.0, 10.0, 1.0);
        glutSolidCube(5.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslated(-10.0, 0.0, 0.0);
        glScaled(1.0, 10.0, 1.0);
        glutSolidCube(5.0f);
        glPopMatrix();
        break;
    case 1:
        break;

    default:
        break;
    }

    glPopMatrix();
}

void barras(GLdouble alturaCampo){
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslated(0.0,alturaCampo,0.0);
    glScaled(100.0,1.0,1.0);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0,-alturaCampo,0.0);
    glScaled(100.0,1.0,1.0);
    glutSolidCube(1.0f);
    glPopMatrix();
}
void player(int num, GLdouble Py, GLdouble larguraCampo){
    glPushMatrix();

    if(num == 1){
        glColor3f(0.0f, 0.0f, 1.0f);
        glTranslated(-larguraCampo,Py,0.0);
    }
        
    
    if(num == 2){
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslated(larguraCampo,Py,0.0);
    }
        

    //glScaled(1.0,2.8,1.0);
    //glutSolidCube(20.0f);
    glScaled(1.0,2.8,1.0);
    glutSolidCube(5.0f);

    glPopMatrix();
}

void Potaro(GLdouble Px,GLdouble Py,GLdouble rotate)
{
    glPushMatrix();
    glTranslated(Px,Py,0.0);
    glRotatef(rotate,0.0,0.0,1.0);
    //glScaled(4.0,4.0,4.0);
    glScaled(1.0,1.0,1.0);
    
        //Corpo + cabeça
        glPushMatrix();
        glColor3f(0.0, 0.7, 0.8);
        glScaled(1.4, 1.6, 1.3);
        glutSolidSphere(3, 100, 100);
        glPopMatrix();

        //bracos direito
        glPushMatrix();
        glColor3f(0.0, 0.7, 0.8);
        glRotated(90, -0.1, 1.0, 0.0);
        glutSolidCone(3.0, 6, 100, 100);
        glPopMatrix();

        //bracos esquerdo
        glPushMatrix();
        glColor3f(0.0, 0.7, 0.8);
        glRotated(-90, 0.1, 1.0, 0.0);
        glutSolidCone(3.0, 6, 100, 100);
        glPopMatrix();

        //Barriga
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslated(0.0, -1.3, 0.7);
        glutSolidSphere(3.5, 100, 100);
        glPopMatrix();

        //Umbiguinho
        glPushMatrix();
        glColor3f(0.0, 0.7, 0.8);
        glTranslated(0.0, -2.6, 3.8);
        glutSolidSphere(0.3, 100, 100);
        glPopMatrix();

        //Olhos direito
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glRotated(45, -1.0, 0.0, 1.5);
        glTranslated(1.8, -0.5, 3.8);
        glScaled(1.0, 1.0, 0.2);
        glutSolidCube(1);
        glPopMatrix();

        //Olhos esquerdo
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glRotated(45, -1.0, 0.0, 1.5);
        glTranslated(-0.1, 1.0, 4.0);
        glScaled(1.0, 1.0, 0.2);
        glutSolidCube(1);
        glPopMatrix();

        //Parte preta dos olhos direito
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(1.2, 2.2, 3.8);
        glutSolidSphere(0.3, 100, 100);
        glPopMatrix();

        //Parte preta dos olhos direito
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(-1.2, 2.2, 3.8);
        glutSolidSphere(0.3, 100, 100);
        glPopMatrix();

        //Boca direita
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glRotated(45, 0.0, 0.0, 1.0);
        glTranslated(0.77, 0.35, 3.9);
        glScaled(0.2, 0.7, 0.1);
        glutSolidCube(1);
        glPopMatrix();

        //boca esquerda
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glRotated(-45, 0.0, 0.0, 1.0);
        glTranslated(-0.6, 0.51, 3.9);
        glScaled(0.2, 0.7, 0.1);
        glutSolidCube(1);
        glPopMatrix();

        //tracinhos rosto direita
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(2.6, 2.4, 3.9);
        glScaled(0.7, 0.2, 0.1);
        glutSolidCube(1);
        glPopMatrix();

        //baixo
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(2.6, 2.0, 3.9);
        glScaled(0.7, 0.2, 0.1);
        glutSolidCube(1);
        glPopMatrix();

        //tracinhos rosto esquerda
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(-2.6, 2.4, 3.9);
        glScaled(0.7, 0.2, 0.1);
        glutSolidCube(1);
        glPopMatrix();

        //baixo
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslated(-2.6, 2.0, 3.9);
        glScaled(0.7, 0.2, 0.1);
        glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();
}