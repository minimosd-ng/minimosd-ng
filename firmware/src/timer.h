#ifndef __TIMER_H__
#define __TIMER_H__

struct time {
  unsigned char h, m, s;
};

void init_timer(void);
unsigned int nnow(void);
unsigned int now(void);
void clock_process(void);
void get_time(unsigned int seconds, struct time *t);
unsigned char get_toogle(void);
unsigned int get_uptime(void);

#endif
