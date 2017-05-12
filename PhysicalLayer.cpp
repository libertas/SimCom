#include <QCoreApplication>
#include <QSerialPort>
#include <QTimer>

#include <mutex>
#include <thread>

#include "PhysicalLayer.h"

using namespace std;

mutex ph_send_lock;

mutex ssLock;
QSerialPort *ss;

char ph_send_queue_buf[PH_BUF_LEN];
char ph_receive_queue_buf[PH_BUF_LEN];

char_queue ph_send_queue;
char_queue ph_receive_queue;
bool ph_initialized = false;

const char* devicename = NULL;
mutex qtLock;

char qtSendChar;
mutex qtSendLock;
mutex qtSendLock1;

void ph_receive_intr();

void thread_qt()
{
  QCoreApplication* qa = new QCoreApplication(* new int(), (char**)"");

  QTimer *qtm = new QTimer(qa);

  ss = new QSerialPort(qa);
  ss->setBaudRate(115200);
  ss->setPortName(devicename);
  ss->setDataBits(QSerialPort::Data8);
  ss->setParity(QSerialPort::NoParity);
  ss->setStopBits(QSerialPort::OneStop);
  ss->setFlowControl(QSerialPort::NoFlowControl);

  qa->connect(qtm, &QTimer::timeout, [=]() {
    qtSendLock.lock();

    ss->waitForBytesWritten(-1);

    ss->write(&qtSendChar, 1);

    qtSendLock1.unlock();

  });

  qa->connect(ss, &QSerialPort::readyRead, [=]() {
    {
      char d;

      ss->waitForBytesWritten(-1);

      ss->read(&d, 1);

      ph_receive_intr(d);
    }
  });

  if(!ss->open(QIODevice::ReadWrite)) {
    qtLock.unlock();
    delete ss;
    delete qa;
    return;
  }
  qtm->start(1);

  ph_initialized = true;
  qtLock.unlock();
  qa->exec();
}

bool ph_init(const char* device)
{
  if(ph_initialized) {
    return false;
  }

  init_char_queue(&ph_send_queue, ph_send_queue_buf, PH_BUF_LEN);
  init_char_queue(&ph_receive_queue, ph_receive_queue_buf, PH_BUF_LEN);

  devicename = device;
  qtSendLock.lock();
  qtSendLock1.lock();
  qtLock.lock();
  thread * tq = new thread(thread_qt);
  qtLock.lock();
  qtLock.unlock();

  return ph_initialized;
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

void ph_send_intr()
{
  /*
    You must call this function timely to send the data in the queue
    This function must be modified to use different types of physical devices
  */
  char c;

  ph_send_lock.lock();

  while(out_char_queue(&ph_send_queue, &c)) {
    qtSendChar = c;
    qtSendLock.unlock();
    qtSendLock1.lock();
  }

  ph_send_lock.unlock();
  ssLock.unlock();
}
