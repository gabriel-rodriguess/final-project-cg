#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "objetos.h"



GLfloat angle, fAspect;
GLfloat angle45 = 45;
//Pontuacao
int P1 = 0;
int P2 = 0;
GLdouble obsX=0, obsY=-150, obsZ=400;
//Posicao Players
GLdouble P1y=0;
GLdouble P2y=0;

int eixo = -1;
int direcao = 0;
int conta = 0;

int winner = 0;



//Configuracao da altura do campo
GLdouble alturaCampo = 130;
GLdouble larguraCampo = 180;

//Posicao do Potaro (Bola)
static GLdouble ball_pos_x = 0, ball_pos_y = 0, ball_radius = 15;
static GLfloat ball_velocity_x = 0, ball_velocity_y = 0, speed_increment = 0.1;
static GLfloat rotate = 0;

typedef struct BMPImagem
{
    int   width;
    int   height;
    char *data;
}BMPImage;

/* Qtd máxima de texturas a serem usadas no programa */
#define MAX_NO_TEXTURES 3

/* vetor com os números das texturas */
GLuint texture_id[MAX_NO_TEXTURES];

char* filenameArray[MAX_NO_TEXTURES] = {
		"porter.bmp",
		"madeon.bmp",
        "campo.bmp"
};

GLUquadricObj *obj;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;


void getBitmapImageData( char *pFileName, BMPImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
		printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);


    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ", pFileName);

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ", pFileName);

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);

    if( nNumPlanes != 1 )
		printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
		printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);

    if( nNumBPP != 24 )
		printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);


    fseek( pFile, 24, SEEK_CUR );

    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);


	char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
	{
		charTemp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = charTemp;
    }
}



void CarregaTexturas()
{
    BMPImage textura;


    glGenTextures(MAX_NO_TEXTURES, texture_id);
								

    int i;
    for ( i=0; i<MAX_NO_TEXTURES; i++ ) {
        getBitmapImageData( filenameArray[i], &textura);
        glBindTexture(GL_TEXTURE_2D, texture_id[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
}


void initTexture (void)
{


	glEnable ( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    CarregaTexturas();

}



void init(void)
{
    glEnable ( GL_COLOR_MATERIAL );
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    glEnable(GL_DEPTH_TEST);

    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
   
    gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);

    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    

    gluPerspective(45.0, (GLfloat)800 / (GLfloat)600, 0.5, 500);

    glViewport(0, 0, 800, 600);

}

void picture(int imagem){
    float size = 1.0;
    GLfloat v[8][3];

     v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
     v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
     v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
     v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
     v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
     v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    GLfloat n[3] = {0.0, 0.0, 1.0};

    glBindTexture(GL_TEXTURE_2D, texture_id[imagem]);
    glBegin(GL_QUADS);
        glNormal3fv(&n[0]);

        glTexCoord2f(0.0f,0.0f);
        glVertex3fv(&v[5][0]);

        glTexCoord2f(1.0f,0.0f);
        glVertex3fv(&v[6][0]);
        
        glTexCoord2f(1.0f,1.0f);
        glVertex3fv(&v[2][0]);
        
        glTexCoord2f(0.0f,1.0f);
        glVertex3fv(&v[1][0]);
    glEnd();
}

void campo(){
    float size = 1.0;
    GLfloat v[8][3];

     v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
     v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
     v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
     v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
     v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
     v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    GLfloat n[3] = {0.0, 0.0, -2.0};

    glBindTexture(GL_TEXTURE_2D, texture_id[2]);
    glBegin(GL_QUADS);
        glNormal3fv(&n[0]);

        glTexCoord2f(0.0f,0.0f);
        glVertex3fv(&v[5][0]);

        glTexCoord2f(1.0f,0.0f);
        glVertex3fv(&v[6][0]);
        
        glTexCoord2f(1.0f,1.0f);
        glVertex3fv(&v[2][0]);
        
        glTexCoord2f(0.0f,1.0f);
        glVertex3fv(&v[1][0]);
    glEnd();

}

void startGame() {

    // Movendo o Pentaro
    ball_pos_x += ball_velocity_x;
    ball_pos_y += ball_velocity_y;
    rotate++;

    if(eixo == 0){
        angleY = 0.0f;
        if(direcao == 0){
            angleX = angleX + 0.01;
            conta++;
        }else{
            angleX = angleX - 0.01;
            conta--;
        }

        if(direcao == 0 && conta == 300){
            direcao = 1;
        }
        if(direcao == 1 && conta == -300){
            direcao = 0;
        }
    }

    if(eixo == 1){
        angleX = 0.0f;
        if(direcao == 0){
            angleY = angleY + 0.01;
            conta++;
        }else{
            angleY= angleY - 0.01;
            conta--;
        }

        if(direcao == 0 && conta == 300){
            direcao = 1;
        }
        if(direcao == 1 && conta == -300){
            direcao = 0;
        }
    }

    // Pentaro colidiu em cima ou em baixo
    if (ball_pos_y + ball_radius > alturaCampo || ball_pos_y - ball_radius < -alturaCampo)
        ball_velocity_y = -ball_velocity_y;

    // P1 acertou o Pentaro
    if (ball_pos_x - ball_radius - 5 < -larguraCampo)// && ball_pos_x - ball_radius < -larguraCampo)
        if (ball_pos_y < P1y + 28 && ball_pos_y > P1y - 28) { //28 é a altura do player
            ball_velocity_x = -ball_velocity_x;

        }



    // P2 acertou o Pentaro
    if (ball_pos_x + ball_radius + 5 > larguraCampo)// && ball_pos_x + ball_radius < larguraCampo)
        if (ball_pos_y < P2y + 28 && ball_pos_y > P2y - 28) //28 é a altura do player
            ball_velocity_x = -ball_velocity_x;

    // P1 pontuou
    if (ball_pos_x + ball_radius > larguraCampo) {
        P1++;
        printf("Player 1 = %d \n", P1);
        ball_velocity_x = -ball_velocity_x;

        if(eixo == 0){
            eixo = 1;
        }else{
            eixo = 0;
        }
    }

    // P2 pontuou
    if (ball_pos_x - ball_radius < -larguraCampo) {
        P2++;
        printf("Player 2 = %d \n", P2);
        ball_velocity_x = -ball_velocity_x;

        if(eixo == 0){
            eixo = 1;
        }else{
            eixo = 0;
        }
    }

    if(P2 == 7 || P1 == 7){
        if(P1 == 7){
            winner = 1;
        }else{
            winner = 2;
        }
        ball_pos_x = ball_pos_y = 0;
        P1y = P2y = 0;
        P1 = P2 = 0;
        eixo = -1;
        rotate = 0;

        glutIdleFunc(NULL);
    }

    glutPostRedisplay();
}

void GerenciaMouse(int button, int state, int x, int y) {
    switch (button) {
        
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN){
            //ball_velocity_x = (rand() % 2);// -  (rand() % 3);
            //ball_velocity_y = (rand() % 4);// -  (rand() % 3);
            //ball_velocity_x = ((rand() % 5) -  (rand() % 3));
            //ball_velocity_y = ((rand() % 5) -  (rand() % 3));
            ball_velocity_x = -0.5f;
            ball_velocity_y = 0.3f;
            printf("velocidade x = %f \n", ball_velocity_x);
            printf("veloficade y = %f \n", ball_velocity_y);

            winner = 0;

            glutIdleFunc(startGame);
            }
            break;
        // Botao do meio reseta a bola, os jogadores e a pontuacao
        case GLUT_MIDDLE_BUTTON:
            // Reseta
            ball_pos_x = ball_pos_y = 0;
            P1y = P2y = 0;
            P1 = P2 = 0;
            eixo = -1;
            if (state == GLUT_DOWN)
                // Removendo bounce
                glutIdleFunc(NULL);
            break;
        default:
        break;
    }
}

void displayFunc() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glMatrixMode(GL_MODELVIEW);


        glPushMatrix();

        
        if(eixo == -1){
            glRotatef(0.0,1.0,0.0,0.0);
            glRotatef(0.0,0.0,1.0,0.0);
        }else{
            glRotatef(angleX,1.0,0.0,0.0);
            glRotatef(angleY,0.0,1.0,0.0);
        }

                //winner
        if(winner != 0){
            glPushMatrix();
            glScalef(100.0,100.0,100.0);
            
            picture(winner-1);
            glPopMatrix();
        }
        
        //foto primeiro jogador
        glPushMatrix();
          glTranslated(-190.0,150.0,0.0);
          glScalef(70.0,70.0,70.0);
            
          glRotatef(-270.0f,1.0,0.0,0.0);
          glRotatef(-90.0f,0.0,1.0,0.0);
          glRotatef(-90.0f,1.0,0.0,0.0);
          picture(0);
        glPopMatrix();


        //campo
        glPushMatrix();
          //glTranslated(-190.0,150.0,0.0);
          glScalef(400.0,255.0,-2.0);
        
          glRotatef(-270.0f,1.0,0.0,0.0);
          glRotatef(-90.0f,0.0,1.0,0.0);
          glRotatef(-90.0f,1.0,0.0,0.0);
          campo();
        glPopMatrix();


        //foto segundo jogador
        glPushMatrix();
          glTranslated(190.0,150.0,0.0);
          glScalef(70.0,70.0,70.0);
            
          glRotatef(-270.0f,1.0,0.0,0.0);
          glRotatef(-90.0f,0.0,1.0,0.0);
          glRotatef(-90.0f,1.0,0.0,0.0);
          picture(1);
        glPopMatrix();

        
        glColor3f(1.0f, 1.0f, 1.0f);

        //NUMERO PLAYER 1
        numero(1,P1);
  

        //NUMERO PLAYER 2
        numero(2,P2);

        //PLAYERS
        player(1,P1y,larguraCampo);

        player(2,P2y,larguraCampo);

        //BARRAS
        barras(alturaCampo);
        
        //BOLA
        Potaro(ball_pos_x,ball_pos_y,rotate); //nome da bola

        glPopMatrix();

    glFlush(); 
}

void rotacoes(int key, int x, int y){
     switch (key){
        case GLUT_KEY_UP :
           angleX+=15;
           break ;
        case GLUT_KEY_DOWN :
           angleX-=15;
           break ;
        case GLUT_KEY_LEFT :
           angleY-=15;
           break ;
        case GLUT_KEY_RIGHT :
           angleY+=15;
           break ;
        default:
           break;
     }
     glutPostRedisplay() ;
}


void keyboard(unsigned char key, int x, int y)
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


int main(int argc, char *argv[]){
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(800,600);
    glutCreateWindow("PONG");
    glutDisplayFunc(displayFunc);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(rotacoes);
    glutKeyboardFunc(keyboard);
    init();
    initTexture();
    glutMainLoop();
    return 0;
}