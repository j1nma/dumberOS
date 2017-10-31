// Requerimientos

// Se   debe   implementar   envío   y   recepción   de   mensajes   bloqueantes,   de   cantidad   fija   de   bytes,
// enviados   a   un   identificador   común   de   tipo   cadena   de   caracteres   definida   entre   los   procesos
// que   van   a   comunicarse   (puede   ser   una   combinación   de   PIDs,   un   nombre   de   dominio,   u   otra cosa).

// También   deberán   implementar   mutexes,   los   mismos   pueden   ejecutar   sus   operaciones   de   up y   down
// sobre   un   identificador   acordado.


// Syscalls   involucradas

// Send   y   Receive   para   mensajes,   siendo   la   segunda   bloqueante.

// Up   y   Down   para   mutexes. El   diseño   del   scheduler   debería   contemplar   estas   syscalls.
// Un   thread   esperando   por   un mensaje   o   mutex   no   debería   bloquear   a   los   otros   threads.


//inicial las variables no deterministicamente

void testIPC();

void testIPC() {

}

// int main(int argc, char const *argv[])
// {
	
// 	testIPC();
	
// 	return 0;
// }