#include "SimCom.h"


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
  if(!sl_init()) {
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

  while(1) {
    ph_send_intr();
    ph_receive_intr();
    sl_receive_intr();
    usleep(100);
  }

  return 0;
}

#endif


#ifdef TEST_DATALINK

#include <stdio.h>
#include "DataLinkLayer.h"
#include "PhysicalLayer.h"

int main()
{
  char c;
  char s[200];
  SIMCOM_LENGTH_TYPE length;

  dl_init();

  dl_send("Hello, World!\nHello, World!\nHello, World!\nHello, World!\n", 56);
  ph_send_intr();
  ph_receive_intr();
  dl_receive(s, &length);
  s[length] = 0;
  printf("%d\n%s", length, s);

  return 0;
}

#endif


#ifdef TEST_PHYSICAL

#include <stdio.h>
#include "PhysicalLayer.h"

int main()
{
  unsigned char c;

  ph_init();

  for(c = 0; c < 0x80; c++) {
    ph_send(c);
  }
  ph_send_intr();
  ph_receive_intr();
  while(ph_receive((char*)&c)) {
    putchar(c);
  }

  return 0;
}
#endif
