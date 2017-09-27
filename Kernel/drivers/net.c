#include <drivers.h>
#include <leeryConsole.h>


#define IOADDR 0xC000 //Direccion de la placa

#define TSAD0 (IOADDR + 0x20) //En este registro escribo la direccion de memoria a transmitir.
#define TSD0 (IOADDR + 0x10) //En este registro escribo la longitud del mensaje a transmitir.

#define ISR (IOADDR + 0x3E) //ISR es el registro de estado que quiero ver lo que hizo la placa.

#define TSD_OWN (1 << 13) //Este es el valor del registro de estado que quiero que no este para poder transmitir.

#define RECEIVE_OK 1
#define TRANSMIT_OK (1 << 2)

#define RX_OVERHEAD 4 //De protocolo de ethernet.
#define RX_DATA_OFFSET (RX_OVERHEAD + ETH_HLEN) //De protocolo de ethernet.

#define BUFFER_SIZE 8*1024+16 //Buffer size segun OSDev.

#define MAC_SIZE 6 //Mac size en bytes.
#define MAC_STRING_LENGHT 17 //Mac size en formato FF:FF:FF:FF:FF:FF con un 0 al final para compatible con C.


ethframe frame; //Este es el frame que se usa para transmitir. 
	
static uint8_t myMac[6]; //Mi mac.
static char macString[MAC_STRING_LENGHT + 1]; //Mi mac en formato FF:FF:FF:FF:FF:FF
static char* macBroadcast = "FF:FF:FF:FF:FF:FF"; //Direccion de brodcast para comparar.

static uint8_t receiveBuffer[BUFFER_SIZE] = {0}; //Mi buffer de recepcion.


/**
	Esta funcion hablar con un la configuracion del PCI y cada dispositivo.
	Sacado de OSDev.
*/
uint16_t pciConfigReadWord (uint8_t bus, uint8_t func, uint8_t offset){
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = 0;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    sysOutLong (0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((sysInLong (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }
 /**
	Esta funcion hablar con un la configuracion del PCI y cada dispositivo.
	Sacado de OSDev.
*/
void pciConfigWriteWord (uint8_t bus, uint8_t func, uint8_t offset, uint8_t value){
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = 0;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    sysOutLong (0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    sysOutLong (0xCFC, value);
}


/**
	Habilitamos DMA para nuestros perifericos.
*/
void dma_start(){
	uint32_t reg = pciConfigReadWord(0, 0x18, 0x04); //Esto es el PCI Configuration Space de la placa.
	reg |= (1<<2); //Pongo el bit 2 en 1 para habilitar DMA para que pueda escribir en la memoria (los buffers).
	pciConfigWriteWord(0, 0x18, 0x04, reg);
}

void net_start(){

	

	sysOutByte( IOADDR + 0x52, 0x0); //Aca prendemos la placa de red pasandole un 0x00 en el registro 0x52.



	sysOutByte( IOADDR + 0x37, 0x10); //Aca reseteamos la placa mandandole un 0x10 al registro 0x37.
	while( (sysInByte(IOADDR + 0x37) & 0x10) != 0) { } //Me quedo esperando hasta que el registro devuelva un 0x10.



	//Aca le pasamos a la placa de red la direccion de memoria de lo que seria mi buffer de lectura.
	sysOutLong(IOADDR + 0x30, (uint32_t)receiveBuffer); 



	//Aca le definimos la mascara de interupciones de la placa de red, pongo todo en 1.
	sysOutWord(IOADDR + 0x3C, 0x000f);



	//Aca configuramos el buffer de recepcion, le digo que accepte todo y que trate al buffer como un verctor tradicional.
	sysOutLong(IOADDR + 0x44, 0xf | (1 << 7));


	//Aca prendemos la recepcion y emicion de paquetes. Esto basicamente habilita la comunicacion.
	sysOutByte(IOADDR + 0x37, 0x0C);


	//Esto es el puntero a la memoria a transmitir, frame es basado en el protocolo de ethernet.
	//En realidad hay 4 punteros a memoria que se pueden usar pero solo usamos 1.
	sysOutLong(TSAD0, (uint32_t)&frame);


	for(int i = 0; i < MAC_SIZE ; i++){
		unsigned char readByte = sysInByte(IOADDR + i);
		frame.hdr.src[i] = readByte; //Saco la mac del registro 0x00 y lo guardo en el frame ethernet.
		myMac[i] = readByte; //Guardo en memoria local.

		//Aca guardo la mac en string formato FF:FF:FF:FF:FF:FF que es como nuestro protocolo funciona.
		macString[i*3] = ((readByte / 16) >= 9) ? (readByte / 16) + 'A' - 10 : (readByte / 16) + '0';
		macString[i*3 + 1] = ((readByte % 16) >= 9) ? (readByte % 16) + 'A' - 10 : (readByte % 16) + '0';
		macString[i*3 + 2] = ':';
	}
}

/**
	Esto se fija si el mensaje es para mi o broadcast.
*/
void checkAddress(){
	if (macEqual(macString, (char *)&receiveBuffer[RX_DATA_OFFSET]) 
		|| macEqual(macBroadcast, (char *)&receiveBuffer[RX_DATA_OFFSET])){
		//Do nothing.
	}else{
		receiveBuffer[RX_DATA_OFFSET] = 0; //No es para mi, buffer a 0.
	}
}

/**
	Aca me interumpio la placa de red. Me tengo que fijar no mas que me dice, si me llego algo o envie algo correctamente.
	Si envie correctamente, no hago nada.
	Si recibi correctamente, leo del buffer.
*/
void rtlHandler(){
	int i;
	uint16_t isr = sysInWord(ISR); //ISR es el registro de estado que quiero ver lo que hizo la placa.

	switch(isr){
		case TRANSMIT_OK:{
			//Do nothing.
			break;
		}
		case RECEIVE_OK:{
			//Es para mi?
			checkAddress();
			break;
		}
		default:{
			//Do nothing.
			break;
		}
	}
	
	net_start(); 
}

//Compara strings de MAC de formato FF:FF:FF:FF:FF:FF
int macEqual(char *m1, char *m2){
	for (int i = 0; i < MAC_STRING_LENGHT; ++i){
		if (m1[i] != m2[i])
			return 0;
	}
	return 1;
}

/**
	Protocolo: FF:FF:FF:FF:FF:FF[Mensaje].
	Esta funcion recive un vector al cual tengo que llenar si hay un mensaje dispoinble en el buffer para mi.
	Devuelve la longitud de lo devuelto.
*/
int net_read(char *b){
	int index = 0;

	//Este loop se fija si llegue al final del mensaje y si todo lo que llego es imprimible, byte por byte.
	while (receiveBuffer[index + RX_DATA_OFFSET + MAC_STRING_LENGHT] != 0 && 
			(isPrintable(receiveBuffer[index + RX_DATA_OFFSET + MAC_STRING_LENGHT]) || 
				receiveBuffer[index + RX_DATA_OFFSET + MAC_STRING_LENGHT] == '\n')){
		
		b[index] = (char)receiveBuffer[index + RX_DATA_OFFSET + MAC_STRING_LENGHT];
		receiveBuffer[index + RX_DATA_OFFSET + MAC_STRING_LENGHT] = 0; //Limpia el buffer mientras lo lee.

		index++;
	}

	if (macEqual(macString, (char *)&receiveBuffer[RX_DATA_OFFSET])  //Esto se fija si el mensaje es para mi o broadcast.
		|| macEqual(macBroadcast, (char *)&receiveBuffer[RX_DATA_OFFSET])){

		b[index] = 0; //Terminacion en 0 para C.
		return index;
	}else{
		b[0] = 0; //No es para mi, devuelvo 0.
		return 0;
	}
	
}
/**
	Protocolo: FF:FF:FF:FF:FF:FF[Mensaje].
	Esta funcion envia a la red.
*/
void net_send(char *msg){

	for (int i = 0; i < MAC_SIZE; i++)
		frame.hdr.dst[i] = 0xFF;
	
	memcpy(frame.data, msg, mystrlen(msg)); //Lleno el buffer de transmicion.

	frame.hdr.proto = ETH_P_802_3; //Defino protocolo.

	uint32_t size = ETH_HLEN + mystrlen(msg); //Tamano de la transmision.
	
	while (!(sysInLong(TSD0) & TSD_OWN)){ } //Espero...

	sysOutLong(TSD0, size); //Transmito! Enviando el tamano al registro TDS0.
}


