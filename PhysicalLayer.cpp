#include <SerialStream.h>

#include "PhysicalLayer.h"

#include <mutex>

using namespace std;
using namespace LibSerial;

mutex ph_send_lock;

mutex ssLock;
SerialStream ss;

char ph_send_queue_buf[PH_BUF_LEN];
char ph_receive_queue_buf[PH_BUF_LEN];

char_queue ph_send_queue;
char_queue ph_receive_queue;
bool ph_initialized = false;

bool ph_init(const char* device)
{
  if(ph_initialized) {
    return false;
  }

  init_char_queue(&ph_send_queue, ph_send_queue_buf, PH_BUF_LEN);
  init_char_queue(&ph_receive_queue, ph_receive_queue_buf, PH_BUF_LEN);

  ss.Open(device, ios_base::in | ios_base::out);
  if(!ss.IsOpen()) {
    return false;
  }
  ss.SetBaudRate(SerialStreamBuf::BAUD_115200);
  ss.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
  ss.SetNumOfStopBits(1);
  ss.SetParity(SerialStreamBuf::PARITY_NONE);
  ss.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);

  ph_initialized = true;
  return true;
}

bool ph_send(char data)
{
  if(!ph_initialized) {
    return false;
  }

  bool result;

  ph_send_lock.lock();

  result = in_char_queue(&ph_send_queue, data);

  ph_send_lock.unlock();

  return result;
}

bool ph_receive(char *data)
{
  if(!ph_initialized) {
    return false;
  }

  return out_char_queue(&ph_receive_queue, data);
}

bool ph_receive_intr(char data)
{
  if(!ph_initialized) {
    return false;
  }

  return in_char_queue(&ph_receive_queue, data);
}

void ph_receive_intr()
{
  if(ss.rdbuf()->in_avail()) {
    char d;
    ssLock.lock();
    d = ss.get();
    ssLock.unlock();
    ph_receive_intr(d);
  }
}

void ph_send_intr()
{
  /*
    You must call this function timely to send the data in the queue
    This function must be modified to use different types of physical devices
  */
  char c;

  ssLock.lock();
  ph_send_lock.lock();

  while(out_char_queue(&ph_send_queue, &c)) {
    ss << c;
  }

  ph_send_lock.unlock();
  ssLock.unlock();
}
