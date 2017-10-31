#ifndef ASYNCHRONOUSMP_H
#define ASYNCHRONOUSMP_H

void asyncSend(struct message * msg);

struct message * asyncRecieve();

void disableTaskSwitch();

void enableTaskSwitch();

void blockProcess(struct process * process);

void unblockProcess(struct process * process);

int isBlocked(struct process * process);

int topWaitQueue(struct process * process);

void pushWaitQueue(struct process * receiver, struct process * sender);

int popWaitQueue(struct process * process);

#endif