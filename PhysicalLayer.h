#ifndef _PHYSICALLAYER_H
#define _PHYSICALLAYER_H

#include <stdbool.h>

#include "CharQueue.h"
#include "SimCom.h"

/*
  These functions should be called only by the data link layer
*/
bool ph_init(char_queue *send_queue, char_queue *receive_queue);
bool ph_receive(char *data);
bool ph_send(char data);

/*
  When data received by the device, call this function
  to tell the physical layer
*/
bool ph_receive_intr(char data);

/*
  Modify it and call it timely for your own project
*/
void ph_send_intr();


#endif
