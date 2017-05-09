#ifndef _SIMCOM_H
#define _SIMCOM_H

#include <stdint.h>

/*
  You can modify this for your own project
*/
#define SIMCOM_LENGTH_TYPE uint16_t
#define SIMCOM_DLENGTH_TYPE uint32_t

extern "C" bool simcom_init(const char* device);
extern "C" bool simcom_close();

#endif
