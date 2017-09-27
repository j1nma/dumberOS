#include <drivers.h>

/**
 * Driver teclado
 * Este driver se dedica a hacer el "handeling" de la interupcion de teclado y decodificar los scanncodes recibidos.
*/

void (*myCallback)(uint8_t, int);


static char shifted = FALSE;
static char capslocked = FALSE;
static char isArrow = FALSE;

static uint8_t kbdus[128] = {
   0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
   0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
  '\'', '`',   0,		/* Left shift */
  '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
   0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

static uint8_t kbdusShifted[128] = {
    0,  /* No key for scan code 0 */
    27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
    0,  /* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0,  /* Right shift */
    0,  /*PrtScr*/
    0,  /* Alt */
    ' ',/* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    0,
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    0,
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0  /* All other keys are undefined */
};

int isPrintable(int digit){
	return digit <= 162 && digit >= 32;
}

void setKeyboardCallback(void (*f)(uint8_t, int)){ //Esta funcion la usamos para poder devolver los eventos de teclado a quien sea que se registre aca.
	myCallback = f;
}

void keyboardHandler() {
	uint16_t scancode = sysInByte(0x60);


	if (isArrow){ //Si anteriormente se recibio un codigo de flecha, (ya que las flechas mandan 2 codigos, uno de flecha y el otro cual espesificamente) entonces entro directamente al switch de ARROWS.
		switch(scancode){
			isArrow = FALSE;
			case ARROW_UP_DOWN:{
				myCallback(1, RESPONSE_ARROWS);
				break;
			}
			case ARROW_UP_UP:{
				myCallback(1, RESPONSE_ARROWS);
				break;
			}
			case ARROW_DOWN_DOWN:{
				myCallback(2, RESPONSE_ARROWS);
				break;
			}
			case ARROW_DOWN_UP:{
				myCallback(2, RESPONSE_ARROWS);
				break;
			}
			case ARROW_RIGHT_DOWN:{
				myCallback(3, RESPONSE_ARROWS);
				break;
			}
			case ARROW_RIGHT_UP:{
				myCallback(3, RESPONSE_ARROWS);
				break;
			}
			case ARROW_LEFT_DOWN:{
				myCallback(4, RESPONSE_ARROWS);
				break;
			}
			case ARROW_LEFT_UP:{
				myCallback(4, RESPONSE_ARROWS);
				break;
			}
		}
	}
	
	switch(scancode){ //Este es el scancode de las teclas especiales.
		case BACKSPACE_UP:{
			break;
		}
		case SHIFT_RIGHT_DOWN:
		case SHIFT_LEFT_DOWN:{
			shifted = TRUE;
			break;
		}
		case SHIFT_LEFT_UP:
		case SHIFT_RIGHT_UP:{
			shifted = FALSE;
			break;
		}
		case CAPSLOCK_DOWN:{
			capslocked = !capslocked;
			break;
		}
		case CAPSLOCK_UP:{
			//No lo uso por ahora.
			break;
		}
		case BACKSPACE_DOWN:{
			myCallback(0, RESPONSE_BACKSPACE);
			return;
		}
		case ENTER_DOWN:{
			myCallback(0, RESPONSE_ENTER);
			return;
		}
		case ARROW_KEY:{
			isArrow = TRUE;
			break;
		}
		case TAB_DOWN:{
			myCallback(' ', RESPONSE_CHARACTER);
			myCallback(' ', RESPONSE_CHARACTER);
			myCallback(' ', RESPONSE_CHARACTER);
			myCallback(' ', RESPONSE_CHARACTER);
			return;
		}
	}

	if (isPrintable(kbdus[scancode]) && scancode < 126){ //Esto imprime devuelve la tecla character.
		if (shifted || capslocked){
			myCallback(kbdusShifted[scancode], RESPONSE_CHARACTER);
		}else{
			myCallback(kbdus[scancode], RESPONSE_CHARACTER);
		}
	}
}



