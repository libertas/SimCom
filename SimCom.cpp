#include <thread>

#include "SimCom.h"
#include "ServiceLayer.h"
#include "PhysicalLayer.h"


bool simcom_running = false;

void simcom_send_daemon()
{
  while(simcom_running) {
		ph_send_intr();
	}
}

void simcom_receive_daemon()
{
  while(simcom_running) {
    ph_receive_intr();
		sl_receive_intr();
	}
}

std::thread *th_send;
std::thread *th_receive;

bool simcom_init(const char* device)
{
  if(sl_init(device)) {
    simcom_running = true;

    th_send = new std::thread(simcom_send_daemon);
    th_receive = new std::thread(simcom_receive_daemon);
  } else {
    return false;
  }

  return true;
}

bool simcom_close()
{
  if(simcom_running) {
    simcom_running = false;

    th_send->join();
    th_receive->join();

    delete th_send;
    delete th_receive;

    return true;
  }

  return false;
}


#ifdef TEST_SERVICE

#include <stdio.h>
#include <unistd.h>
#include "ServiceLayer.h"
#include "PhysicalLayer.h"

void callback0(char from, char to, const char* data, SIMCOM_LENGTH_TYPE length)
{
  printf("callback %d, from %d, length %d\tdata:%s",\
    to, from, length, data);
}
void callback2(char from, char to, const char* data, SIMCOM_LENGTH_TYPE length)
{
  printf("Welcome to callback2\n");
}

int main()
{
  char c;
  char s[200];
  SIMCOM_LENGTH_TYPE length;

  sl_config(0, callback0);
  sl_config(1, callback0);
  sl_config(2, callback2);
  if(!simcom_init("/dev/ttyUSB0")) {
    printf("Unable to open the serial port\n");
    return -1;
  }

  sl_send(0, 0, "Welcome to SimCom!\n\n", 21);
  sl_send(0, 2, "", 0);
  sl_send(0, 3, "", 0);
  sl_send(0, 0, "Hello, World!Hello, World!\n", 28);
  sl_send(0, 1, "Hello, World!Hello, World!\n", 28);
  sl_send(0, 0, "Hello, World!Hello, World!\n", 28);
  sl_send(0, 1, "Hello, World!Hello, World!\n", 28);
  sl_send(1, 2, "", 0);

  sleep(1);

  simcom_close();

  return 0;
}

#endif


#ifdef TEST_DATALINK

#include <stdio.h>
#include <unistd.h>
#include "DataLinkLayer.h"
#include "PhysicalLayer.h"

int main()
{
  char c;
  char s[200];
  SIMCOM_LENGTH_TYPE length;

  dl_init("/dev/ttyUSB0");

  dl_send("Hello, World!\nHello, World!\nHello, World!\nHello, World!\n", 56);

  ph_send_intr();

  while(1) {
    usleep(100);
    ph_receive_intr();
    dl_receive(s, &length);

    if(length != 0) {
      s[length] = 0;
      printf("%d\n%s", length, s);
      break;
    }
  }

  return 0;
}

#endif


#ifdef TEST_PHYSICAL

#include <stdio.h>
#include <unistd.h>
#include "PhysicalLayer.h"

int main()
{
  unsigned char c;

  if(!ph_init("/dev/ttyUSB0")) {
    printf("Unable to open the serial port\n");
    return -1;
  }

  for(c = 0; c < 0x80; c++) {
    ph_send(c);
    ph_send_intr();
    ph_receive_intr();
    usleep(100);
  }

  while(ph_receive((char*)&c)) {
    putchar(c);
  }

  return 0;
}
#endif
