#include "SimCom.h"


#ifdef TEST_DATALINK

#include <stdio.h>
#include "DataLinkLayer.h"
#include "PhysicalLayer.h"

int main()
{
  char c;
  char s[20];
  SIMCOM_LENGTH_TYPE length;

  ph_init();

  dl_send("Hello, World!\n", 14);
  ph_send_intr();
  printf("%d %d %s", dl_receive(s, &length), length, s);

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
  while(ph_receive((char*)&c)) {
    putchar(c);
  }

  return 0;
}
#endif
