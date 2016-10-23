/*
-----------
Juego Racer
-----------

Licencia: GNU GPL 3


Compilación
===========

#define SO_LINUX //Para compliar en GNU/Linux.
#define SO_WINDOWS //Para compilar en Windows.

Compilación en sistemas UNIX
============================
`$ gcc Racer.cpp -lpthread -o Racer`

*/

#define SO_LINUX
//#define SO_WINDOWS

//Variables globales
char entrada='\0';
bool numeroDibujo=false, estadoJuego=true;

//Encabezados
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include"portabilidad_racer.h"


//#define's
#define LONGITUD_PISTA 8
#define MAXIMA_LONGITUD 16
#define ATRASO 20 //Atraso entre un auto y el otro
#define CAMBIO_VELOCIDAD 100
#define VELOCIDAD_INICIAL 1000000


//Funciones
void inicializador();
void dibujarPantalla(const short int coche0CoordenadaX, const short int coche0CoordenadaY, const short int coche1CoordenadaX, const short int coche1CoordenadaY, const long int distanciaRecorrida);
void dibujarPantalla1(void);
void dibujarPantalla2(void);
bool detectorColisiones(const short int coche0CoordenadaX, const short int coche0CoordenadaY, const short int coche1CoordenadaX, const short int coche1CoordenadaY);
void dibujarAutoPropio(const short int coche0CoordenadaX, const short int coche0CoordenadaY);
void dibujarAutosContricante(const short int coche1CoordenadaX, const short int coche1CoordenadaY);
short int presentacion();
short int sortearPosicion();
void dibujarChoque(const short int coche0CoordenadaX, const short int coche0CoordenadaY, const short int coche1CoordenadaX, const short int coche1CoordenadaY, const long int distanciaRecorrida);




int main()
{
#ifdef SO_LINUX
	pthread_t idHilo;
#endif
	
	short int dificultad, contador=0;
	long int distanciaRecorrida=0, velocidad=VELOCIDAD_INICIAL;
	
	//coche0 -> coche propio
	struct coche{
		short int coordenadaX;
		short int coordenadaY;
	}coche0, coche1;
	
	
	//Dificultad= 1-5
	dificultad = presentacion();
	velocidad /= dificultad;
	
	system(CLS);
	gotoxy(0,0);
	
	//Inicio de variables
	coche0.coordenadaX=1;
	coche0.coordenadaY=9;
	coche1.coordenadaX=sortearPosicion();
	coche1.coordenadaY=-4;
	
	inicializador();
	

	CREACION_HILO;	
	
	
	while(
            detectorColisiones(coche0.coordenadaX, coche0.coordenadaY, coche1.coordenadaX, coche1.coordenadaY) &&
            (entrada != 'q')
            ){
		dibujarPantalla(coche0.coordenadaX, coche0.coordenadaY, coche1.coordenadaX, coche1.coordenadaY, distanciaRecorrida);
		distanciaRecorrida++;
		entrada='\0';

		if (contador != ATRASO) {
            contador++;
        }
		else{
			contador = 0;
			coche1.coordenadaY = -4;
			coche1.coordenadaX=sortearPosicion();
		}

		//Aumento de velocidad.
		if ((distanciaRecorrida % CAMBIO_VELOCIDAD) == 0){
            velocidad= ((float)velocidad) / (1 + (0.1 * dificultad));
        }

		//Dormir proceso para dar la sensación de FPS
		usleep(velocidad);
		
		system(CLS);
		
		//Análisis de entrada
		switch(entrada)
		{
		case FLECHA_IZQ:
			if (coche0.coordenadaX > 0) {
                coche0.coordenadaX--;
            }
			break;
			
		case FLECHA_DER:
			if (coche0.coordenadaX < 2) {
                coche0.coordenadaX++;
            }
			break;
			
		case FLECHA_ARR:
			if (coche0.coordenadaY > 3){
                coche0.coordenadaY--;
            }
			break;
			
		case FLECHA_ABA:
			if (coche0.coordenadaY < 12) {
                coche0.coordenadaY++;
            }
			break;
		}

        //Bajar el coche
		coche1.coordenadaY++;
	}
		
	//Borrar hilo
	DESTRUCCION_HILO

	if( estadoJuego == false) {
        dibujarChoque(coche0.coordenadaX, coche0.coordenadaY, coche1.coordenadaX, coche1.coordenadaY, distanciaRecorrida);
    }
	
	system(CLS);
	
	return 0;
}


void dibujarPantalla(const short int coche0CoordenadaX, const short int coche0CoordenadaY, const short int coche1CoordenadaX, const short int coche1CoordenadaY, const long int distanciaRecorrida)
{
	//Cambio de dibujado. Crea la sensación de movimiento.
	if (numeroDibujo == false) {
		dibujarPantalla1();
		numeroDibujo=true;
	}
	else{
		dibujarPantalla2();
		numeroDibujo=false;
	}

	dibujarAutoPropio(coche0CoordenadaX, coche0CoordenadaY);
	dibujarAutosContricante(coche1CoordenadaX, coche1CoordenadaY);
	
	gotoxy(12, 14);
	printf("Distancia Recorrida: %ld", distanciaRecorrida);
	
	gotoxy(0, 0);
}

void dibujarPantalla1(void)
{
	short int contador;
	
	for(contador=0; contador < LONGITUD_PISTA;contador++)
	{
		printf("|         |\n");
		printf("$         $\n");
	}
	
	return;
}

void dibujarPantalla2(void)
{
	short int contador;
	
	for(contador=0; contador < LONGITUD_PISTA;contador++)
	{
		printf("$         $\n");
		printf("|         |\n");
	}
	
	return;
}

bool detectorColisiones(const short int coche0CoordenadaX, const short int coche0CoordenadaY, const short int coche1CoordenadaX, const short int coche1CoordenadaY)
{
	if(
		coche0CoordenadaX == coche1CoordenadaX &&
			(
				(coche1CoordenadaY >= coche0CoordenadaY && (coche0CoordenadaY + 3) >= coche1CoordenadaY) ||
				(coche0CoordenadaY >= coche1CoordenadaY && (coche1CoordenadaY + 3) >= coche0CoordenadaY) )
		) {
		estadoJuego=false;

		return false;
	}
	else {
		return true;
	}
}

void dibujarAutoPropio(const short int coche0CoordenadaX, const short int coche0CoordenadaY)
{
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf(" #");
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY + 1);
	printf("###");
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY + 2);
	printf(" #");
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY + 3);
	printf("###");
}

void dibujarAutosContricante(const short int coche1CoordenadaX, const short int coche1CoordenadaY)
{
	if (coche1CoordenadaY < MAXIMA_LONGITUD && coche1CoordenadaY >= 0) {
		gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY);
		printf(" #");
	}
	
	if (coche1CoordenadaY < (MAXIMA_LONGITUD - 1) && coche1CoordenadaY >= -1) {
		gotoxy(1 +coche1CoordenadaX * 3, coche1CoordenadaY + 1);
		printf("###");
	}
	if (coche1CoordenadaY < (MAXIMA_LONGITUD - 2) && coche1CoordenadaY >= -2) {
		gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY + 2);
		printf(" #");
	}
	if (coche1CoordenadaY < (MAXIMA_LONGITUD - 3) && coche1CoordenadaY >= -3) {
		gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY + 3);
		printf("###");
	}
	
}

void inicializador()
{
	system(CLS);
	srand(time(NULL));
	
	return;
}
short int sortearPosicion()
{
	return (rand() % 3);
}

short int presentacion()
{
	short int y=8, tomaCaracteres;
	
	system(CLS);
	printf("Bienvenidos a Racer!\nEl objetivo del juego es evitar colisionar con los coches contrarios.\n");
	printf("Presione las teclas direccionales para moverse hacia los costados,\narriba o abajo.\nPara salir del juego presione \"q\"\n\n");
	printf("Elija el nivel de dificultad con la teclas direccionales:\n\n");
	printf("-> Muy Fácil\n   Fácil\n   Medio\n   Dificil\n   Muy Dificil\n   Vivo al límite\n   Soy Chuck Norris");
	gotoxy(2, 8);
	
	while (entrada!=ENTER) {
		for (tomaCaracteres=0; tomaCaracteres < CANT_CARAC; tomaCaracteres++) {
			entrada = getch();

			if((entrada == ENTER) || (ENTRADA_DIFERENTE)){
				tomaCaracteres = CANT_CARAC;
			}
		}
		
		if (entrada == FLECHA_ARR) {
			if(y > 8) {
				y--;
			}
		}
		else if (entrada == FLECHA_ABA) {
			if (y < 14) {
				y++;
			}
		}
		
		gotoxy(0, 8);
		printf("   Muy Fácil\n   Fácil\n   Medio\n   Dificil\n   Muy Dificil\n   Vivo al límite\n   Soy Chuck Norris");
		
		gotoxy(0, y);
		printf("->");
	}

	//Devolver dificultad elegida
	return (y - 7);
	
}

void dibujarChoque(const short int coche0CoordenadaX, const short int coche0CoordenadaY, const short int coche1CoordenadaX, const short int coche1CoordenadaY, const long int distanciaRecorrida)
{
	//Efecto del choque
	if(numeroDibujo == false) {
		dibujarPantalla2();
	}
	else {
		dibujarPantalla1();
	}
	
	gotoxy(2 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf("\\");
	gotoxy(3 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf("/");
	gotoxy(2 + coche0CoordenadaX * 3, coche0CoordenadaY + 1);
	printf("/");
	gotoxy(3 + coche0CoordenadaX * 3, coche0CoordenadaY + 1);
	printf("\\");
	
	gotoxy(2 + coche1CoordenadaX * 3, coche1CoordenadaY);
	printf("\\");
	gotoxy(3 + coche1CoordenadaX * 3, coche1CoordenadaY);
	printf("/");
	gotoxy(2 + coche1CoordenadaX * 3, coche1CoordenadaY + 1);
	printf("/");
	gotoxy(3 + coche1CoordenadaX * 3, coche1CoordenadaY + 1);
	printf("\\");
	
	gotoxy(0,0);
	
	usleep(1000000);
	
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf("\\\\");
	gotoxy(3 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf("//");
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY + 1);
	printf("//");
	gotoxy(3 + coche0CoordenadaX * 3, coche0CoordenadaY + 1);
	printf("\\\\");
	
	gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY);
	printf("\\\\");
	gotoxy(3 + coche1CoordenadaX * 3, coche1CoordenadaY);
	printf("//");
	gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY + 1);
	printf("//");
	gotoxy(3 + coche1CoordenadaX * 3, coche1CoordenadaY + 1);
	printf("\\\\");
	
	gotoxy(0,0);

	usleep(1000000);
	
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf("##");
	usleep(500000);
	gotoxy(3 + coche0CoordenadaX * 3, coche0CoordenadaY);
	printf("##");
	usleep(500000);
	gotoxy(1 + coche0CoordenadaX * 3, coche0CoordenadaY+ 1);
	printf("##");
	usleep(500000);
	gotoxy(3 + coche0CoordenadaX * 3, coche0CoordenadaY + 1);
	printf("##");
	usleep(500000);

	gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY);
	printf("##");
	usleep(500000);
	gotoxy(3 + coche1CoordenadaX * 3, coche1CoordenadaY);
	printf("##");
	usleep(500000);
	gotoxy(1 + coche1CoordenadaX * 3, coche1CoordenadaY + 1);
	printf("##");
	usleep(500000);
	gotoxy(3 + coche1CoordenadaX * 3, coche1CoordenadaY + 1);
	printf("##");
	gotoxy(0,0);

	usleep(1000000);
	
	gotoxy(12, 14);
	printf("Distancia recorrida: %ld", distanciaRecorrida);
	gotoxy(12, 15);
	printf("Juego terminado. Presione enter para salir.");
	getchar();
}
