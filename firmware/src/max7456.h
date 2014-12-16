#ifndef __MAX7456__
#define __MAX7456_H__

typedef enum {
  MAX7456_REG_VM0   = 0x00,
  MAX7456_REG_DMM   = 0x04,
  MAX7456_REG_DMAH  = 0x05,
  MAX7456_REG_DMAL  = 0x06,
  MAX7456_REG_DMDI  = 0x07,
  MAX7456_REG_RB0   = 0x10,
  MAX7456_REG_RB1   = 0x11,
  MAX7456_REG_RB2   = 0x12,
  MAX7456_REG_RB3   = 0x13,
  MAX7456_REG_RB4   = 0x14,
  MAX7456_REG_RB5   = 0x15,
  MAX7456_REG_RB6   = 0x16,
  MAX7456_REG_RB7   = 0x17,
  MAX7456_REG_RB8   = 0x18,
  MAX7456_REG_RB9   = 0x19,
  MAX7456_REG_RB10  = 0x1a,
  MAX7456_REG_RB11  = 0x1b,
  MAX7456_REG_RB12  = 0x1c,
  MAX7456_REG_RB13  = 0x1d,
  MAX7456_REG_RB14  = 0x1e,
  MAX7456_REG_RB15  = 0x1f,
  MAX7456_REG_STAT  = 0x20,
  MAX7456_REG_OSDBL = 0x6c,
} max7456_reg_t;

/* VM0 register */
#define MAX7456_VM0_VBUFFER 0x01
#define MAX7456_VM0_RESET   0x02
#define MAX7456_VM0_VSYNC   0x04
#define MAX7456_VM0_ENABLE  0x08
#define MAX7456_VM0_SYNC    0x30
#define MAX7456_VM0_PAL     0x40

/* DMM register */
#define MAX7456_DMM_AINC    0x01
#define MAX7456_DMM_VSCLR   0x02
#define MAX7456_DMM_CLR     0x04
#define MAX7456_DMM_INV     0x08
#define MAX7456_DMM_BLINK   0x10
#define MAX7456_DMM_BG      0x20
#define MAX7456_DMM_BG      0x20
#define MAX7456_DMM_8BIT    0x40

/* RB[0-15] */
#define MAX7456_RB_BL0      0x00
#define MAX7456_RB_BL10     0x04
#define MAX7456_RB_BL20     0x08
#define MAX7456_RB_BL30     0x0c
#define MAX7456_RB_WH120    0x00
#define MAX7456_RB_WH100    0x01
#define MAX7456_RB_WH90     0x02
#define MAX7456_RB_WH80     0x03


/* STAT register */
#define MAX7456_STAT_PAL    0x01
#define MAX7456_STAT_NTSC   0x02

/* OSDBL register */
#define MAX7456_OSDBL_AUTO  0x10


void init_max7456(void);
void max7456_putc(char c);
void max7456_xy(unsigned char x, unsigned char y);

#endif
