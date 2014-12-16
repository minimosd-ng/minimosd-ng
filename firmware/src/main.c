/* MinimOSD-ng */

#include <stdio.h>
#include "uart.h"

#define DEBUG

int main(void)
{
  init_uart(19200);
#ifdef DEBUG
  printf("\nRESET!\n\n");
  printf("MinimOSD-ng\n");
#endif

  /* init max7456 */
  init_max7456();

  while (1)
  {

  }
}

