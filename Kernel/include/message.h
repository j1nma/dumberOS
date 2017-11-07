#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define MESSAGE_SIZE 240

struct message {

	int source_pid; //the source process that sends the message

	int destination_pid; //the destination process that receives

	char message[MESSAGE_SIZE]; //the body of the message
	
};

#endif