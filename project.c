#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "objetos.h"

GLfloat angle, fAspect;
//Pontuacao
int P1 = 0;
int P2 = 0;
GLdouble obsX=0, obsY=-150, obsZ=400;
//Posicao Players
GLdouble P1y=0;
GLdouble P2y=0;

//Configuracao da altura do campo
GLdouble alturaCampo = 130;
GLdouble larguraCampo = 180;

//Posicao do Potaro (Bola)
static GLdouble ball_pos_x = 0, ball_pos_y = 0, ball_radius = 15;
static GLfloat ball_velocity_x = 0, ball_velocity_y = 0, speed_increment = 0.1;
static GLfloat rotate = 0;



void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    
    //NUMERO PLAYER 1
    numero(1,P1);


    //NUMERO PLAYER 2
    numero(2,P2);


    //BARRAS
    barras(alturaCampo);


    //PLAYERS
    player(1,P1y,larguraCampo);

    player(2,P2y,larguraCampo);

    //BOLA
    Potaro(ball_pos_x,ball_pos_y,rotate); //nome da bola

    //glutSwapBuffers();
    glFlush();
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


/*void GerenciaMouse(int button, int state, int x, int y)
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
}*/

void startGame() {

    // Movendo o Pentaro
    ball_pos_x += ball_velocity_x;
    ball_pos_y += ball_velocity_y;
    rotate++;

    // Pentaro colidiu em cima ou em baixo
    if (ball_pos_y + ball_radius > alturaCampo || ball_pos_y - ball_radius < -alturaCampo)
        ball_velocity_y = -ball_velocity_y;

    // P1 acertou o Pentaro
    if (ball_pos_x - ball_radius - 5 < -larguraCampo && ball_pos_x - ball_radius < -larguraCampo)
        if (ball_pos_y < P1y + 28 && ball_pos_y > P1y - 28) { //28 é a altura do player
            ball_velocity_x = -ball_velocity_x;
            //ball_velocity_x += speed_increment;
            //paddile_velocity += speed_increment;
        }


    // P2 acertou o Pentaro
    if (ball_pos_x + ball_radius + 5 > larguraCampo && ball_pos_x + ball_radius < larguraCampo)
        if (ball_pos_y < P2y + 28 && ball_pos_y > P2y - 28) //28 é a altura do player
            ball_velocity_x = -ball_velocity_x;

    // P1 pontuou
    if (ball_pos_x + ball_radius > larguraCampo) {
        P1++;
        printf("Player 1 = %d \n", P1);
        ball_velocity_x = -ball_velocity_x;
    }

    // P2 pontuou
    if (ball_pos_x - ball_radius < -larguraCampo) {
        P2++;
        printf("Player 2 = %d \n", P2);
        ball_velocity_x = -ball_velocity_x;
    }

    glutPostRedisplay();
}

void GerenciaMouse(int button, int state, int x, int y) {
    switch (button) {
        // Botao Esquerdo - Inicia uma velocidade aleatoria entre (ran(5) - rand(3))
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            ball_velocity_x = (rand() % 2);// -  (rand() % 3);
            ball_velocity_y = (rand() % 2);// -  (rand() % 3);
            /*ball_velocity_x = (rand() % 5) -  (rand() % 3);
            ball_velocity_y = (rand() % 5) -  (rand() % 3);*/

            glutIdleFunc(startGame);
            break;
        // Botao do meio reseta a bola, os jogadores e a pontuacao
        case GLUT_MIDDLE_BUTTON:
            // Reseta
            ball_pos_x = ball_pos_y = 0;
            P1y = P2y = 0;
            P1 = P2 = 0;
            if (state == GLUT_DOWN)
                // Removendo bounce
                glutIdleFunc(NULL);
            break;
        default:
        break;
    }
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
    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
    //O 33 somado e subtraido se deve ao tamanho do player
    switch (key)
    {
    case 'q':
    case 'Q':
        if(P1y  < (alturaCampo - 33))
            P1y = P1y + 5;
        break;
    case 'a':
    case 'A':
        if(P1y  > -(alturaCampo - 33))
            P1y = P1y - 5;
        break;
    case 'o':
    case 'O':
        if(P2y  < (alturaCampo - 33))
            P2y = P2y + 5;
        break;
    case 'l':
    case 'L':
        if(P2y  > -(alturaCampo - 33))
            P2y = P2y - 5;
        break;
    }
    glutPostRedisplay();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PONG");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(Keyboard);
    Inicializa();
    glutMainLoop();
}