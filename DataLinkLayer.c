#include "CRC.h"
#include "DataLinkLayer.h"
#include "PhysicalLayer.h"

// [STX] [CRC] [DATA[n]] [ETX]
char dl_buf[DL_BUF_LEN];

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
  if(dl_initialized) {
    return false;
  }

  return true;
}

bool dl_send(char *data, SIMCOM_LENGTH_TYPE length)
{
  int i, j;
  if(dl_initialized) {
    return false;
  }

  if((length << 1) + 3 > DL_BUF_LEN) {
    return false;
  }

  // STX
  dl_buf[0] = 0x02;

  for(i = 0, j = 2; i < length; i++, j++) {
    if(dl_buf[i] <= 0x1F) {
      dl_buf[j] = 0x1B;
      j++;
    }

    dl_buf[j] = data[i];
  }

  // ETX
  dl_buf[j] = 0x03;
  dl_buf[1] = crc(dl_buf + 2);

  for(i = 0; i < j; i++) {
    int count = 0;

    while(!ph_send(dl_buf[i])) {
      if(count > DL_RETRY_TIMES) {
        return false;
      }
      count++;
    }
  }

  return true;
}

void dl_daemon()
{

}
