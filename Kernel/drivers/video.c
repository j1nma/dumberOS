#include <stdarg.h>
#include <drivers.h>
#include <naiveConsole.h>
#include <leeryConsole.h>

/**
 * Esta driver es el que se ocupa de manejo de input y output de la pantalla.
 * Tecnico: Este driver te da la linea de edicion (ultima) y si hay un scanf (por ejempo) devuelver el valor, etc, etc.
 */

char buffer[500];
int startBuffering = FALSE;
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
 *En conclucion, el usuario tiene que hacer polling hasta que encuentre en la respuesta el character '/n'.
 */
int read(char *str){

	startBuffering = TRUE; //Cuando se llama a read, startBuffering se pone en true, lo que hacer que la funcion "write" guarde todo en el buffer.

	int i = 0;
	for (i = 0; i < bufferIndex; i++){ //Este for copia el buffer al vector de parametro.
		str[i] = buffer[i];
		if (buffer[i] == '\n'){ //Si encuentro un '\n' significa que el usuario presiono "return" entonces pongo el flag de buffer en false.
			startBuffering = FALSE;
		}
	}
	str[i] = 0; //Pongo un 0 al final del vector del usuario para que sea leible en C.

	int aux = bufferIndex; //Pongo bufferIndex en 0 y devulevo su valor al usuario, su valor es la longitud del buffer.
	bufferIndex = 0;
	return aux;
}

/**
 *Esta funcion escribe en pantalla el string recibido a la longitud pedida.
 */
void write(char *str, int length){
	
	if (startBuffering == FALSE){
		lcPrint(str);
	}else{
		for (int i = 0; i < length; i++){
			if (startBuffering){ //Si startBuffering esta en true entonces guardo todo lo impreso.
				buffer[bufferIndex++] = str[i];
			}
		}
		lcPrintChar(str[0]); //Si start buffering esta en TRUE entonces los characteres entran por teclado, o sea de a uno.
	}
}

void backspace(){
	char nl = '\b'; //Backspace aca lo que hace es meter el character '\b' en el buffer de "read", cuando el user le llega, tiene que hacer el backspace
					//en su buffer propio.
	write(&nl, 1);
}

