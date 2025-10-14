#ifndef STEPPER_DRIVER_H_
#define STEPPER_DRIVER_H_

#include <zephyr/drivers/gpio.h>

struct stepper_t {
    struct gpio_dt_spec step_pin;
    struct gpio_dt_spec dir_pin;
    struct gpio_dt_spec en_pin;
    int steps_per_rev;
    int microsteps;
};

int stepper_init(const struct stepper_t *motor);
int stepper_move_deg(const struct stepper_t *motor, float degrees);

#endif /* STEPPER_DRIVER_H_ */
