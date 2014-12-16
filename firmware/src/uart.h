#ifndef _UART_H_
#define _UART_H_

void init_uart(unsigned int b);
void set_baudrate(unsigned int b);
unsigned char uart_getc(unsigned char *c);
void uart_redirect_input(void *f);

#endif
