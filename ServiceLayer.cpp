#include <mutex>

#include "DataLinkLayer.h"
#include "ServiceLayer.h"

void (*callbacks[SL_CALLBACK_NUM])(char, char, char*, SIMCOM_LENGTH_TYPE) = {0};


bool sl_init()
{
  return dl_init();
}

bool sl_config(char port, void (*callback)(char, char, char*, SIMCOM_LENGTH_TYPE))
{
  if((int)port < SL_CALLBACK_NUM) {
    callbacks[port] = callback;
    return true;
  } else {
    return false;
  }
}


std::mutex sl_send_lock;

bool sl_send(char from_port, char to_port, char *data, SIMCOM_LENGTH_TYPE length)
{
  char sl_send_buf[SL_BUF_LEN];

  sl_send_lock.lock();

  sl_send_buf[0] = from_port;
  sl_send_buf[1] = to_port;
  for(SIMCOM_LENGTH_TYPE i = 0; i < length; i++) {
    sl_send_buf[i + 2] = data[i];
  }

  bool result = dl_send(sl_send_buf, length + 2);

  sl_send_lock.unlock();

  return result;
}

bool sl_receive_intr()
{
  char sl_receive_buf[SL_BUF_LEN];

  SIMCOM_LENGTH_TYPE length;
  if(dl_receive(sl_receive_buf, &length)) {
    if((unsigned char)sl_receive_buf[1] < SL_CALLBACK_NUM\
      && callbacks[(unsigned char)sl_receive_buf[1]] != 0) {
      callbacks[(unsigned char)sl_receive_buf[1]](sl_receive_buf[0],\
        sl_receive_buf[1],\
        sl_receive_buf + 2,\
        length - 2);
      return true;
    }
  }

  return false;
}
