#include "DataLinkLayer.h"
#include "PhysicalLayer.h"

char_queue *dl_send_queue;
char_queue *dl_receive_queue;
bool dl_initialized = false;

bool dl_init(char_queue *send_queue, char_queue *receive_queue)
{
  if(dl_initialized) {
    return false;
  }

  dl_send_queue = send_queue;
  dl_receive_queue = receive_queue;
  dl_initialized = true;
  return true;
}

bool dl_receive(char *data, SIMCOM_LENGTH_TYPE length)
{

}

bool dl_send(char *data, SIMCOM_LENGTH_TYPE length)
{

}

void dl_daemon()
{

}
