/*************************************************************
CONSOLA.H
LIBRER�A PARA AUMENTAR LA EXTENSI�N 
DE CONIO.H Y OTRAS EN VISUAL C++
(Da soporte a funciones como gotoxy, clrscr, etc.)

M�xico, D.F. 
�ltima Actualizaci�n: 2-Enero-2003
**************************************************************/


#ifndef _FUNCIONES_DE_CONSOLA_AUXILIARES_PARA_VISUAL_CPP_
#define _FUNCIONES_DE_CONSOLA_AUXILIARES_PARA_VISUAL_CPP_



//MACROS para argumentos de _setcursortype()
#define _NORMALCURSOR	0 //cursor como caracter de subrayado		
#define _NOCURSOR		1 //oculta el cursor
#define _SOLIDCURSOR	2 //cursor s�lido 


//MACROS para texcolor, textbackground, etc.
#define BLACK		 0	//NEGRO
#define BLUE		 1	//AZUL MARINO
#define GREEN		 2	//VERDE PERICO
#define CYAN		 3	//VERDE CLARO
#define RED			 4	//ROJO
#define MAGENTA		 5	//MORADO O VIOLETA
#define BROWN		 6	//CAF�
#define LIGHTGRAY	 7	//GRIS CLARO
#define DARKGRAY	 8	//GRIS OBSCURO
#define LIGHTBLUE	 9	//AZUL MARINO CLARO
#define LIGHTGREEN	 10	//VERDE CLARO
#define LIGHTCYAN	 11	//AZUL CLARO
#define LIGHTRED	 12	//ROJO CLARO
#define LIGHTMAGENTA 13	//ROSA MEXICANO
#define YELLOW		 14	//AMARILLO
#define WHITE		 15	//BLANCO


//////////////////////////////////////////////////////////
//ORIGINALES DE CONIO.H

void clrscr(void);//Borrar pantalla
void clreol(void);//Borrar hasta el final de l�nea

void delline(void);//Eliminar la l�nea actual
void insline(void);//Insertar una nueva l�nea

void gotoxy(int x,int y);//Mover el cursor
int wherex(void);//Coordenada x actual
int wherey(void);//Coordenada y actual

void textcolor(int color);//Colocar color de texto
void textbackground(int color);//Colocar color de fondo

void _setcursortype(int tipo);//Tipo de cursor


//////////////////////////////////////////////////////////
//ORIGINALES DE DOS.H

void delay(int miliseg);//pausa en milisegundos
void sleep(int segundos);//pausa en segundos



//////////////////////////////////////////////////////////
//OTRAS �TILES

int gettextcolor();//obtener color de texto
int gettextbackground();//obtener color de fondo

int getcols();//ancho en caracteres
int getrows();//alto en caracteres

void setsizedefault();//80x25 caracteres
void setsize8043();//80x43 caracteres
void setsize8050();//80x50 caracteres
void setsizexy(int x, int y);

int settitle(const char *cadena);//coloca t�tulo a la barra
int gettitle(char *cadena,int tam);//obtiene el t�tulo



#endif //_FUNCIONES_DE_CONSOLA_AUXILIARES_PARA_VISUAL_CPP_

