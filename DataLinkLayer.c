#include "Verify.h"
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

bool dl_receive(char *data, SIMCOM_LENGTH_TYPE *length)
{
  char data;

  if(dl_initialized) {
    return false;
  }

  static SIMCOM_LENGTH_TYPE i = 0;
  static bool start_found = false;
  static bool end_found = false;
  static bool esc_found = false;
  while(ph_receive(&data)) {
    if(!start_found) {
      if(data == 0x02) {
        start_found = true;
        dl_buf[i] = data;
        i++;
      }
    } else {
      if(!end_found) {
        if(i >= 2) {
          if(esc_found) {
            dl_buf[i] = data;
            esc_found = false;
          } else {
            if(data == 0x1B) {
              esc_found = true;
            } else if(data == 0x02) {
              i = 0;
              break;
            } else if(data == 0x03) {
              end_found = true;
              i = 0;
              break;
            } else {
              dl_buf[i] = data;
            }
          }

        }
      }
    }
  }

  if(start_found && end_found) {
    /*
      data processing
    */
  } else {
    return false;
  }

  return true;
}

bool dl_send(char *data, SIMCOM_LENGTH_TYPE length)
{
  SIMCOM_LENGTH_TYPE i, j;
  if(dl_initialized) {
    return false;
  }

  if(SIMCOM_DLENGTH_TYPE(length << 1) + 3 > DL_BUF_LEN) {
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
  dl_buf[1] = verify(dl_buf + 2, j - 2);

  for(i = 0; i < j; i++) {
    SIMCOM_LENGTH_TYPE count = 0;

    while(!ph_send(dl_buf[i])) {
      if(count > DL_RETRY_TIMES) {
        return false;
      }
      count++;
    }
  }

  return true;
}
