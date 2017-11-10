#ifndef _MUTUAL_EXCLUSION_H_
#define _MUTUAL_EXCLUSION_H_

int * newBolt();

int acquireBolt(int * bolt);

int releaseBolt(int * bolt);

int deleteBolt(int * ptr);

#endif