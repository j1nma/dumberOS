#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define MESSAGE_SIZE 240

struct message {

	struct process * source; //the source process that sends the message

	struct process * destination; //the destination process that receives

	char message[MESSAGE_SIZE]; //the body of the message
	
};

#endif