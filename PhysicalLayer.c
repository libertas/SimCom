#include "PhysicalLayer.h"

char_queue *ph_send_queue;
char_queue *ph_receive_queue;
bool initialized = false;

bool ph_init(char_queue *send_queue, char_queue *receive_queue)
{
  if(initialized) {
    return false;
  }

  ph_send_queue = send_queue;
  ph_receive_queue = receive_queue;
  initialized = true;
  return true;
}

bool ph_send(char data)
{
  if(!initialized) {
    return false;
  }

  return in_char_queue(ph_send_queue, data);
}

bool ph_receive(char *data)
{
  if(!initialized) {
    return false;
  }

  return out_char_queue(ph_receive_queue, data);
}

bool ph_received(char data)
{
  if(!initialized) {
    return false;
  }

  return in_char_queue(ph_receive_queue, data);
}

void ph_daemon()
{
  /*
    You must call this function timely to send the data in the queue
    This function must be modified to use different types of physical devices
  */
}
