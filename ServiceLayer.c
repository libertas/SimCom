#include "DataLinkLayer.h"
#include "ServiceLayer.h"

void (*callbacks[SL_CALLBACK_NUM])(char*, SIMCOM_LENGTH_TYPE) = {0};
char sl_buf[SL_BUF_LEN];


bool sl_init()
{
  return dl_init();
}

bool sl_config(char port, void (*callback)(char*, SIMCOM_LENGTH_TYPE))
{
  if((int)port < SL_CALLBACK_NUM) {
    callbacks[port] = callback;
    return true;
  } else {
    return false;
  }
}

bool sl_send(char from_port, char to_port, char *data, SIMCOM_LENGTH_TYPE length)
{
  sl_buf[0] = from_port;
  sl_buf[1] = to_port;
  for(SIMCOM_LENGTH_TYPE i = 0; i < length; i++) {
    sl_buf[i + 2] = data[i];
  }
  return dl_send(sl_buf, length + 2);
}
