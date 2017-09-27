#include <leeryConsole.h>

char *v = (char *) 0xB8000;
static char buffer[64] = { '0' };
int currentPointer = 0;

/*
* COPIADO DEL NAIVECONCOLE
*/
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}



int findLastSpace(char *c, int end){
	for (int i = 0; i < end; i++){
		if (*(c + end - i) == ' '){
			return i;
		}
	}
	return 0;
}

int strlenght(char *c){
	int lenght = 0;
	while(*c != 0 && lenght < 0xFFFFFFFF){
		lenght++;
		c++;
	}
	return lenght;
}

void moveUp(){
	currentPointer = 0;
	for (int y = 0; y < 26; y++){
		for (int x = 0; x < 80; x++){
			v[x*2 + (y)*2*80] = v[x*2 + (y+1)*2*80];
			v[x*2 + (y)*2*80 +1] = v[x*2 + (y+1)*2*80 +1];
		}
	}
}

void cleanScreen(){
	for (int x = 0; x < 80; ++x){
		for (int y = 0; y < 25; ++y){
			writeCharacter(0, x, y, 0x00);

		}
	}
}
void printStringStartingAtPointWithLenght(char *str, int x, int y, char color, int lenght){

	for (int index = 0; index < lenght; index++){
		if (str[index] == '\n'){ //Si a console le llega un '\n', hace un newline.
			lcNewLine();
			currentPointer = 0;
		}else if (str[index] == '\b'){ //Si a console le llega un '\b' hace un backspace.
			lcBackSpace();
			currentPointer--;
		}else{
			writeCharacter(str[index], x + index, y, color);
			currentPointer++;
		}
	}
	
}
void printStringStartingAtPoint(char *str, int x, int y, char color){
	int index = 0;

	while(str[index] != 0){
		if (str[index] == '\n'){ //Si a console le llega un '\n', hace un newline.
			lcNewLine();
			currentPointer = 0;
		}else if (str[index] == '\b'){ //Si a console le llega un '\b' hace un backspace.
			lcBackSpace();
			currentPointer--;
		}else{
			writeCharacter(str[index], x + index, y, color);
			currentPointer++;
		}
		index++;
	}
}

void putCharacter(char c, char color){
	int position = currentPointer*2 + 80*2*24;

	*(v + position) = c;
	*(v + position + 1) = color;
}

void printString(char *str, char color){
	int index = 0;

	while(str[index] != 0){
		if (str[index] == '\n'){ //Si a console le llega un '\n', hace un newline.
			lcNewLine();
			currentPointer = 0;
		}else if (str[index] == '\b'){ //Si a console le llega un '\b' hace un backspace.
			lcBackSpace();
			currentPointer--;
		}else{
			putCharacter(str[index], color);
			currentPointer++;
		}
		index++;
	}

}
void printStringLenght(char *str, int lenght, char color){
	int index = 0;

	for (index = 0; index < lenght; index++){
		if (str[index] == '\n'){ //Si a console le llega un '\n', hace un newline.
			lcNewLine();
			currentPointer = 0;
		}else if (str[index] == '\b'){ //Si a console le llega un '\b' hace un backspace.
			lcBackSpace();
			currentPointer--;
		}else{
			putCharacter(str[index], color);
			currentPointer++;
		}
	}

}



void writeCharacter(char c, int x, int y, char color){
	if (x > 79 || y > 24){
		return;
	}
	if (x < 0 || x < 0){
		return;
	}
	int position = x*2 + 80*2*y;

	*(v + position) = c;
	*(v + position + 1) = color;
}

void lcNewLine(){
	moveUp();
	currentPointer = 0;
}

void lcPrintChar(char c){
	lcPrint(&c);
}

void lcPrintHex(uint16_t value){

	uintToBase(value, buffer, 16);
	lcPrint(buffer);
}

void lcPrintInt(int number){
	uintToBase((uint64_t)number, buffer, 10);
	lcPrint(buffer);
}

void lcPrint(char *str){

	while (currentPointer + strlenght(str) > 80){
		int to = currentPointer;
		printStringLenght(str, 80 - to, 0x0F);
		str = str + 80 - to;
		moveUp();
	}


	int strln = strlenght(str);
	if (currentPointer + strln <= 80){
		printString(str, 0x0F);
		//currentPointer = currentPointer + strln;
		//moveUp();
		return;
	}
}

void lcBackSpace(){ //Este backspace es solo en el video, no cambia nada del buffer.

	int position = (currentPointer-1)*2 + 80*2*24;

	*(v + position) = ' ';
	*(v + position + 1) = 0x00;


}


