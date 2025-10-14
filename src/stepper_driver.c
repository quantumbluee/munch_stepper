#include "stepper_driver.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(stepper, LOG_LEVEL_INF);

#define DEG_TO_STEPS(motor, deg) ((int)((deg) / 360.0f * (motor)->steps_per_rev * (motor)->microsteps))

int stepper_init(const struct stepper_t *m)
{
    if (!device_is_ready(m->step_pin.port) ||
        !device_is_ready(m->dir_pin.port) ||
        !device_is_ready(m->en_pin.port)) {
        LOG_ERR("Stepper GPIO device not ready");
        return -ENODEV;
    }

    gpio_pin_configure_dt(&m->step_pin, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&m->dir_pin, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&m->en_pin, GPIO_OUTPUT_ACTIVE); // Active LOW (disabled)

    LOG_INF("Stepper initialized");
    return 0;
}

int stepper_move_deg(const struct stepper_t *m, float degrees)
{
    if (degrees <= 0 || degrees > 360) {
        LOG_WRN("Invalid input angle %.1f", degrees);
        return -EINVAL;
    }

    int steps = DEG_TO_STEPS(m, degrees);

    // Enable driver (LOW = ON)
    gpio_pin_set_dt(&m->en_pin, 0);
    gpio_pin_set_dt(&m->dir_pin, 1); // Direction (CW)

    for (int i = 0; i < steps; i++) {
        gpio_pin_set_dt(&m->step_pin, 1);
        k_busy_wait(700);   // High pulse ~700 µs
        gpio_pin_set_dt(&m->step_pin, 0);
        k_busy_wait(700);   // Low pulse
    }

    gpio_pin_set_dt(&m->en_pin, 1); // Disable driver (LOW→HIGH)
    LOG_INF("Moved %.1f degrees (%d steps)", degrees, steps);
    return 0;
}
