//Este encabezado posee las constantes para lograr portabilidad del juego de Ruleta entre los SO Windows y GNU/Linux.


//****Sistema Operativo GNU/Linux
#ifdef SO_LINUX
#define _A_ 193//Imprime "Á"
#define _a_ 225//Imprime "á"
#define _E_ 201//Imprime "É"
#define _e_ 233//Imprime "é"
#define _I_ 205//Imprime "Í"
#define _i_ 237//Imprime "í"
#define _O_ 211//Imprime "Ó"
#define _o_ 243//Imprime "ó"
#define _U_ 218//Imprime "Ú"
#define _u_ 250//Imprime "ú"
#define _N_ 209//Imprime "Ñ"
#define _n_ 241//Imprime "ñ"
#define _sp_ 191//Imprime "¿"
#define _se_ 161//Imprime "¡"
#define _sg_ 176//Imprime "°"

#define ENTER 10

//Borrar pantalla.
#define CLS "clear"

#define FLECHA_ARR 65
#define FLECHA_ABA 66
#define FLECHA_DER 67
#define FLECHA_IZQ 68
//Cantidad de caracteres que componen la flecha de dirección.
#define CANT_CARAC 3
#define ENTRADA_DIFERENTE entrada!=27&&entrada!=91

#define SO 0//Linux

#define DECLARACION_HILO
#define CREACION_HILO pthread_create(&idHilo, NULL, entradaDatos, NULL)
#define DESTRUCCION_HILO pthread_cancel(idHilo);

//Encabezados.
#include <termios.h>
#include <unistd.h>
#include<pthread.h>

//Funciones
int getch( void ) {
	struct termios oldt,
		newt;
	int            ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

//Ubica el cursor en diferentes coordenadas
void gotoxy(const short int x, const short int y)
{
	printf("\033[%d;%df", y+1, x+1);
	fflush(stdout);
}

void *entradaDatos(void *parametro)
{
	while(entrada!='q')
	{
		entrada=getch();
	}
	
	return NULL;
}

#endif



//*****Sistema Operativo Windows
#ifdef SO_WINDOWS

//Macros.
#define _A_ 181//Imprime "Á"
#define _a_ 160//Imprime "á"
#define _E_ 144//Imprime "É"
#define _e_ 130//Imprime "é"
#define _I_ 214//Imprime "Í"
#define _i_ 161//Imprime "í"
#define _O_ 244//Imprime "Ó"
#define _o_ 162//Imprime "ó"
#define _U_ 233//Imprime "Ú"
#define _u_ 163//Imprime "ú"
#define _N_ 165//Imprime "Ñ"
#define _n_ 164//Imprime "ñ"
#define _sp_ 168//Imprime "¿"
#define _se_ 173//Imprime "¡"
#define _sg_ 248//Imprime "°"

#define ENTER 13

#define CLS "cls"//Borrar pantalla.

#define FLECHA_ARR 72
#define FLECHA_ABA 80
#define FLECHA_DER 77
#define FLECHA_IZQ 75
//Cantidad de caracteres que componen la flecha de dirección.
#define CANT_CARAC 2
#define ENTRADA_DIFERENTE entrada!=-32

#define SO 1//Windows

#define CREACION_HILO CreateThread(NULL, 0, entradaDatos, NULL, 0, NULL )
#define DESTRUCCION_HILO 

//Encabezados
#include<conio.h>
#include<windows.h>

//Funciones

//Ubica el cursor en diferentes coordenadas
void gotoxy( const int x, const int y )
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
}



DWORD WINAPI entradaDatos(LPVOID arg ) { 
	while(entrada!='q')
	{
		entrada=getch();
	}
	
	return NULL;
}

#endif