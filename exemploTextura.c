#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "objetos2.h"

GLfloat angle, fAspect;

//Pontuacao
int P1 = 0;
int P2 = 0;
GLdouble obsX=0, obsY=-150, obsZ=400;
//Posicao Players
GLdouble P1y=0;
GLdouble P2y=0;

//Configuracao da altura do campo
GLdouble alturaCampo = 30;
GLdouble larguraCampo = 35;

//Posicao do Potaro (Bola)
static GLdouble ball_pos_x = 0, ball_pos_y = 0, ball_radius = 10;
static GLfloat ball_velocity_x = 0, ball_velocity_y = 0, speed_increment = 0.1;
static GLfloat rotate = 0;

typedef struct BMPImagem
{
    int   width;
    int   height;
    char *data;
}BMPImage;

/* Qtd máxima de texturas a serem usadas no programa */
#define MAX_NO_TEXTURES 6

/* vetor com os números das texturas */
GLuint texture_id[MAX_NO_TEXTURES];

char* filenameArray[MAX_NO_TEXTURES] = {
		"EAGLE.bmp",
		"EAGLE.bmp",
		"EAGLE.bmp",
		"EAGLE.bmp",
		"EAGLE.bmp",
		"EAGLE.bmp"
};

GLUquadricObj *obj;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;

//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
//-----------------------------------------------------------------------------
void getBitmapImageData( char *pFileName, BMPImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
		printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

    // Seek forward to width and height info
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

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

	// Calculate the image's total size in bytes. Note how we multiply the
	// result of (width * height) by 3. This is becuase a 24 bit color BMP
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);

    //
	// Finally, rearrange BGR to RGB
	//

	char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
	{
		charTemp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = charTemp;
    }
}


/*Função para Carregar uma imagem .BMP */
void CarregaTexturas()
{
    BMPImage textura;

    /* Define quantas texturas serão usadas no programa  */
    glGenTextures(MAX_NO_TEXTURES, texture_id); /* 1 = uma textura; */
								/* texture_id = vetor que guardas os números das texturas */

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

/* **********************************************************************
  void initTexture(void)
		Define a textura a ser usada

 ********************************************************************** */
void initTexture (void)
{

	/* Habilita o uso de textura bidimensional  */
	glEnable ( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*Carrega os arquivos de textura */
  //  CarregaTextura("tunnelTexture.bmp");
    //CarregaTextura("tex2.bmp");
    CarregaTexturas();

}

void lightning(){
    GLfloat light0_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0,GL_POSITION,light0_pos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,black);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white);
    glLightfv(GL_LIGHT0,GL_SPECULAR,white);

    //Atenuação radial
    //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);   //define a0
    //glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.15f);    //define a1
    //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1f);  //define a2

    //Fonte de Luz Direcional - Efeito de Holofote
    GLfloat light1_pos[] = {-2.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT1,GL_POSITION,light1_pos);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1,GL_SPECULAR, white);
    GLfloat direction[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,direction);       //vetor direção
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,45.0f);               //espalhamento angular
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.1f);              //atenuação angular

    //Parâmetros definidos globalmente
    //GLfloat global_ambient[] = {0.9f, 0.9f, 0.9f, 1.0f};
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    ////Reflexão especular definida como a posição de visão corrente
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    ////Habilitar cálculos de iluminação para ambas as faces dos polígonos
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void init(void)
{
    glEnable ( GL_COLOR_MATERIAL );
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //define a cor de fundo
    glEnable(GL_DEPTH_TEST); //habilita o teste de profundidade

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW); //define que a matrix é a model view
    glLoadIdentity(); //carrega a matrix de identidade
    gluLookAt(0.0, -30.0, 100.0,   //posição da câmera
              0.0, 0.0, 0.0,   //para onde a câmera aponta
              0.0, 1.0, 0.0);  //vetor view-up
    //gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);

    glMatrixMode(GL_PROJECTION); //define que a matrix é a de projeção
    glLoadIdentity(); //carrega a matrix de identidade
    //glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção ortogonal
    
    //gluPerspective(45.0, 1.0, 2.0, 8.0); //define uma projeção perspectiva
    gluPerspective(45.0, 1.0, 0.5, 500);
    //gluPerspective(angle, fAspect, 0.5, 500);

    //glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva simétrica
    //glFrustum(-2.0, 1.0, -1.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva obliqua
    glViewport(0, 0, 800, 600);

    //lightning();
}

void cubo(){
     float size = 1.0;
     GLfloat n[6][3] =
     {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
     };
     GLint faces[6][4] =
     {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
     };
     GLfloat v[8][3];
     GLint i;

     v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
     v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
     v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
     v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
     v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
     v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

     for (i = 5; i >= 0; i--) {
         glBindTexture(GL_TEXTURE_2D, texture_id[i]);
         glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glTexCoord2f(0.0f,0.0f);
            glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(1.0f,0.0f);
            glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(1.0f,1.0f);
            glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0.0f,1.0f);
            glVertex3fv(&v[faces[i][3]][0]);
         glEnd();
     }
}

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
        if (ball_pos_y < P1y + 7 && ball_pos_y > P1y - 7) { //7 é a altura do player
            ball_velocity_x = -ball_velocity_x;
            //ball_velocity_x += speed_increment;
            //paddile_velocity += speed_increment;
        }


    // P2 acertou o Pentaro
    if (ball_pos_x + ball_radius + 5 > larguraCampo && ball_pos_x + ball_radius < larguraCampo)
        if (ball_pos_y < P2y + 7 && ball_pos_y > P2y - 7) //7 é a altura do player
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
            ball_velocity_x = rand();//(rand() % 2);// -  (rand() % 3);
            ball_velocity_y = rand();//(rand() % 2);// -  (rand() % 3);
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

void displayFunc() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa o buffer

        glMatrixMode(GL_MODELVIEW); //define que a matrix é a de modelo


        glPushMatrix();
        
        glRotatef(angleX,1.0,0.0,0.0);
        glRotatef(angleY,0.0,1.0,0.0);
        
        /*glPushMatrix();
          glScalef(20.0,20.0,20.0);
          cubo();
        glPopMatrix();*/
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        //NUMERO PLAYER 1
        //numero(1,P1);
  

        //NUMERO PLAYER 2
        //numero(2,P2);

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


int main(int argc, char *argv[]){
    glutInit(&argc,argv);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(800,600);
    glutCreateWindow("Textura");
    glutDisplayFunc(displayFunc);
    glutMouseFunc(GerenciaMouse);
    glutSpecialFunc(rotacoes);
    glutKeyboardFunc(keyboard);
    init();
    initTexture();
    glutMainLoop();
    return 0;
}