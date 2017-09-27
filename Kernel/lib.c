#include <stdint.h>

void getTime(char *ptr){

	sysOutByte(0x70, 0x0B);
	uint16_t RTCConfig = sysInByte(0x71);
	RTCConfig = RTCConfig | (1 << 2);
	sysOutByte(0x70, 0x0B);
	sysOutByte(0x71, RTCConfig);


	
	sysOutByte((uint16_t)0x70, (uint16_t)0x00);
	unsigned int seconds = 0;
	seconds = sysInByte((uint16_t)0x71);
	
	sysOutByte((uint16_t)0x70, (uint16_t)0x02);
	unsigned int minutes = 0;
	minutes = sysInByte((uint16_t)0x71);

	sysOutByte((uint16_t)0x70, (uint16_t)0x04);
	unsigned int hours = 0;
	hours = sysInByte((uint16_t)0x71);


	char secondsString[3];
	uintToBase(seconds, secondsString, 10);

	char minutesString[3];
	uintToBase(minutes, minutesString, 10);

	char hoursString[3];
	uintToBase(hours, hoursString, 10);
	

	ptr[0] = hoursString[0];
	ptr[1] = hoursString[1];

	ptr[2] = ':';

	ptr[3] = minutesString[0];
	ptr[4] = minutesString[1];

	ptr[5] = ':';

	ptr[6] = secondsString[0];
	ptr[7] = secondsString[1];

	ptr[8] = 0;
	
}


int mystrlen(char *s){
	int c = 0;
	while(*(s++) != 0){
		c++;
	}

	return c;
}

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}
