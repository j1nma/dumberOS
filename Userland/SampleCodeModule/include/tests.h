#include "libc.h"
#include "commands.h"

/* Message passing */
void processMessagePassing();

void processSender();

void processReceiver();

/* Mutex exclusion */
void processMutualExclusion();

void processMutualExclusionSetup();

void processThatAcquiresMulti();

void processMultiMutualExclusionSetup();

/* Process killing */
void processKilled();

void processKiller();

/* Process listing */
void processListing();

void killListing();

/* Memory testing */
void memoryError(int error);

void testMemory();

void testSharedMemory();