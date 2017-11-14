#include <stdarg.h>
#include <drivers.h>
#include <naiveConsole.h>
#include <leeryConsole.h>

/**
 * Esta driver es el que se ocupa de manejo de input y output de la pantalla.
 * Tecnico: Este driver te da la linea de edicion (ultima) y si hay un scanf (por ejempo) devuelver el valor, etc, etc.
 */

char buffer[200];
int max = 200;
int startBuffering = TRUE;
int bufferIndex = 0;


void clean(){

}
/**
 *Esta funcion imprime la linea nueva y tambien pone el character limitador en el buffer ('\n').
 */
void newLine(){
	
	char nl = '\n';
	write(&nl, 1);
	
}

/**
 *El funcionamiento de read esta pensado de la siguiente manera:
 *Cuando el usuario (en userland) quiere leer de pantalla, desde su api va a llamar a la syscall de read (esta funcion),
 *y esta funcion solamente va a devolver, inmediatamente, lo que tenga en el buffer, en caso que no tenga nada va a retornar 0.
 *Esto habilita al usuario a hacer polling desde su api.
 *Terminamos implementando read de esta forma porque cuando el usuario llama a la syscall desde una interrupcion, si queriamos que 
 *retorne solamente cuando el usuario presionaba "return" entonces la interupcion quedaba pendiente el retorno, lo que hacia que el 
 *micro no pueda recibir mas interupciones, como las del teclado, entonces no era posible hacerlo de esta forma.
 *En conclusion, el usuario tiene que hacer polling hasta que encuentre en la respuesta el character '/n'.
 */
int read(char *str){

	int i = 0;
	for (i = 0; i < bufferIndex; i++){ //Este for copia el buffer al vector de parametro.
		str[i] = buffer[i];
	}
	str[i] = 0; //Pongo un 0 al final del vector del usuario para que sea leible en C.

	int aux = bufferIndex; //Pongo bufferIndex en 0 y devulevo su valor al usuario, su valor es la longitud del buffer.
	bufferIndex = 0;
	return aux;
}

/**
 *Esta funcion escribe en pantalla el string recibido a la longitud pedida.
 */
void write(char * str, int length){

	if (bufferIndex >= max)
		return;
	
	if (length == 1){
		lcPrintChar(str[0]); //Si start buffering esta en TRUE entonces los characteres entran por teclado, o sea de a uno.
		for (int i = 0; i < length; i++){
			buffer[bufferIndex++] = str[i];
		}	
	}else{
		lcPrint(str);
	}
	
}

void backspace(){
	char nl = '\b';
	if (bufferIndex > 0){
		buffer[bufferIndex - 1] = 0;
		bufferIndex--;
		lcPrintChar(nl);
	}
}

