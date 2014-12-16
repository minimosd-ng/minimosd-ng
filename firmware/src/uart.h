#ifndef __UART_H__
#define __UART_H__

void init_uart(unsigned int b);
void set_baudrate(unsigned int b);
unsigned char uart_getc(unsigned char *c);

#endif
