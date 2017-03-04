#include "Verify.h"
#include "DataLinkLayer.h"
#include "PhysicalLayer.h"

// [STX] [CRC] [DATA[n]] [ETX]
char dl_buf[DL_BUF_LEN];


bool dl_init()
{
  return ph_init();
}

bool dl_receive(char *data, SIMCOM_LENGTH_TYPE *length)
{
  SIMCOM_LENGTH_TYPE length_tmp;
  char c;

  static SIMCOM_LENGTH_TYPE i = 0;
  static bool start_found = false;
  static bool end_found = false;
  static bool esc_found = false;
  while(ph_receive(&c)) {
    /*
      if the buffer if full
    */
    if(i >= DL_BUF_LEN){
      i = 0;
      start_found = false;
      end_found = false;
      esc_found = false;
      continue;
    }

    if(!start_found) {
      if(c == 0x02) {
        printf("start\n");
        start_found = true;
        dl_buf[i] = c;
        i++;
        continue;
      }
    } else {
      if(i < 2) {
        dl_buf[i] = c;
        i++;
        continue;
      } else {
        if(esc_found) {
          esc_found = false;
          dl_buf[i] = c;
          i++;
          continue;
        } else {
          if(c== 0x1B) {
            esc_found = true;
            i++;
            continue;
          } else if(c == 0x02) {
            i = 0;
            start_found = false;
            esc_found = false;
            end_found = false;
            continue;
          } else if(c == 0x03) {
            end_found = true;
            length_tmp = i;
            i = 0;
            break;
          } else {
            dl_buf[i] = c;
            i++;
            continue;
          }
        }
      }
    }
  }

  if(start_found && end_found) {
    start_found = false;
    end_found = false;
    esc_found = false;

    if(verify(dl_buf + 2, length_tmp - 2) == dl_buf[1]) {
      for(SIMCOM_LENGTH_TYPE tmp = 2; tmp < length_tmp; tmp++) {
        data[tmp - 2] = dl_buf[tmp];
      }
      *length = length_tmp - 2;
      return true;
    }
  }

  return false;
}

bool dl_send(char *data, SIMCOM_LENGTH_TYPE length)
{
  SIMCOM_LENGTH_TYPE i, j;

  if((SIMCOM_DLENGTH_TYPE)(length << 1) + 3 > DL_BUF_LEN) {
    return false;
  }

  // STX
  dl_buf[0] = 0x02;
  dl_buf[1] = verify(data, length);

  for(i = 0, j = 2; i < length; i++, j++) {
    if(data[i] <= 0x1F) {
      dl_buf[j] = 0x1B;
      j++;
    }

    dl_buf[j] = data[i];
  }

  // ETX
  dl_buf[j] = 0x03;

  for(i = 0; i < j + 1; i++) {
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
