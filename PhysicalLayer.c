#include <SerialStream.h>

#include "PhysicalLayer.h"

using namespace std;
using namespace LibSerial;

SerialStream ss;

char ph_send_queue_buf[PH_BUF_LEN];
char ph_receive_queue_buf[PH_BUF_LEN];

char_queue ph_send_queue;
char_queue ph_receive_queue;
bool ph_initialized = false;

bool ph_init()
{
  if(ph_initialized) {
    return false;
  }

  init_char_queue(&ph_send_queue, ph_send_queue_buf, PH_BUF_LEN);
  init_char_queue(&ph_receive_queue, ph_receive_queue_buf, PH_BUF_LEN);

  ss.Open("/dev/ttyUSB0", ios_base::in | ios_base::out);
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

  return in_char_queue(&ph_send_queue, data);
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

void ph_send_intr()
{
  /*
    You must call this function timely to send the data in the queue
    This function must be modified to use different types of physical devices
  */
  #if (defined TEST_PHYSICAL) || (defined TEST_DATALINK) || (defined TEST_SERVICE)
  char c;
  while(out_char_queue(&ph_send_queue, &c)) {
    ss << c;
  }

  if(ss.rdbuf()->in_avail()) {
    char d;
    d = ss.get();
    ph_receive_intr(d);
  }

  #endif
}
