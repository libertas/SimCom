#include <QSerialPort>

#include "PhysicalLayer.h"

#include <mutex>

using namespace std;

mutex ph_send_lock;

mutex ssLock;
QSerialPort *ss;

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

  ss = new QSerialPort();
  ss->setBaudRate(115200);
  ss->setPortName(device);
  ss->setDataBits(QSerialPort::Data8);
  ss->setParity(QSerialPort::NoParity);
  ss->setStopBits(QSerialPort::OneStop);
  ss->setFlowControl(QSerialPort::NoFlowControl);

  if(!ss->isOpen()) {
    delete ss;
    return false;
  }

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
  if(ss->bytesAvailable()) {
    char d;
    ssLock.lock();
    d = ss->read(&d, 1);
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
    ss->write(&c, 1);
  }

  ph_send_lock.unlock();
  ssLock.unlock();
}
