#ifndef _PHYSICALLAYER_H
#define _PHYSICALLAYER_H

#include <stdbool.h>

#include "CharQueue.h"
#include "SimCom.h"

#define PH_BUF_LEN 500

/*
  These functions should be called only by the data link layer
*/
extern "C" bool ph_init(const char* device);
extern "C" bool ph_receive(char *data);
extern "C" bool ph_send(char data);

/*
  When data received by the device, call this function
  to tell the physical layer
*/
bool ph_receive_intr(char data);

/*
  Modify it and call it timely for your own project
*/
extern "C" void ph_send_intr();


#endif
