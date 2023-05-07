#ifndef _STEP_MOTOT_H
#define _STEP_MOTOT_H

#include "dev_gpio.h"

#define CW_DIR   0
#define CCW_DIR  1
extern void step_motor_init(uint32_t gpio_port, uint32_t gpio_port_mask);
extern void step_motor_active(uint8_t step_dir, uint16_t step_angle, uint8_t step_speed);

#endif