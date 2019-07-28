#include "embARC.h"
#include "embARC_debug.h"

#include "board.h"
#include "dev_gpio.h"

static DEV_GPIO_PTR buzzer;

extern void buzzer_init(void);
extern void buzzer_open(void);
extern void buzzer_close(void);