#ifndef __TIMER_H__
#define __TIMER_H__

struct datetime {
  unsigned char h, m, s;
};

void init_timer(void);
unsigned int now(void);
void clock_process(void);
struct datetime* get_time(void);
unsigned char get_toogle(void);

#endif
