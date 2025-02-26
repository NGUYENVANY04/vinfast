#include <stdio.h>
#include "driver_enable_output.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
void enable_output(void) { gpio_set_level(GPIO_NUM_2, 1); }
