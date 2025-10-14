#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "stepper_driver.h"

LOG_MODULE_REGISTER(stepper, LOG_LEVEL_INF);

/* Helper macro: convert degrees to step count */
#define DEG_TO_STEPS(motor, deg) ((int)((deg) / 360.0f * (motor)->steps_per_rev * (motor)->microsteps))

int stepper_init(const struct stepper_t *m)
{
    if (!device_is_ready(m->step_pin.port) ||
        !device_is_ready(m->dir_pin.port) ||
        !device_is_ready(m->en_pin.port)) {
        LOG_ERR("Stepper GPIO device not ready");
        return -ENODEV;
    }
    
    int ret;
    ret = gpio_pin_configure_dt(&m->step_pin, GPIO_OUTPUT_INACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure STEP pin: %d", ret);
        return ret;
    }
    
    ret = gpio_pin_configure_dt(&m->dir_pin, GPIO_OUTPUT_INACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure DIR pin: %d", ret);
        return ret;
    }
    
    ret = gpio_pin_configure_dt(&m->en_pin, GPIO_OUTPUT_INACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure EN pin: %d", ret);
        return ret;
    }

    LOG_INF("Stepper initialized (steps/rev=%d, microsteps=%d)", 
            m->steps_per_rev, m->microsteps);
    return 0;
}

int stepper_move_deg(const struct stepper_t *m, float degrees)
{
    if (degrees <= 0.0f || degrees > 360.0f) {
        LOG_WRN("Invalid input angle %.1f", (double)degrees);
        return -EINVAL;
    }

    int steps = DEG_TO_STEPS(m, degrees);
    LOG_INF("Moving %.1f degrees (%d steps)...", (double)degrees, steps);

    /* Enable driver (EN = LOW for TMC2208) */
    gpio_pin_set_dt(&m->en_pin, 0);
    k_msleep(10);  // Give driver time to wake up
    
    gpio_pin_set_dt(&m->dir_pin, 1); // CW rotation
    k_usleep(10);  // Direction setup time

    for (int i = 0; i < steps; i++) {
        gpio_pin_set_dt(&m->step_pin, 1);
        k_usleep(10);   // 10 µs HIGH (minimum for TMC2208)
        gpio_pin_set_dt(&m->step_pin, 0);
        k_usleep(1000); // 1ms LOW (slower speed = 1000 steps/sec)
    }

    /* Disable driver (EN = HIGH) */
    gpio_pin_set_dt(&m->en_pin, 1);
    LOG_INF("Move completed");
    return 0;
}
