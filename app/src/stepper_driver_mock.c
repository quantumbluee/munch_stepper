#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "stepper_driver.h"

LOG_MODULE_REGISTER(stepper_mock, LOG_LEVEL_INF);

/* Helper macro: convert degrees to step count */
#define DEG_TO_STEPS(motor, deg) ((int)((deg) / 360.0f * (motor)->steps_per_rev * (motor)->microsteps))

int stepper_init(const struct stepper_t *m)
{
    LOG_INF("[MOCKED] stepper_init() called");
    LOG_INF("[MOCKED] steps_per_rev=%d, microsteps=%d", m->steps_per_rev, m->microsteps);
    return 0;
}

int stepper_move_deg(const struct stepper_t *m, float degrees)
{
    if (degrees <= 0.0f || degrees > 360.0f) {
        LOG_WRN("[MOCKED] move_stepper(%.1f degrees) - INVALID INPUT", (double)degrees);
        return -EINVAL;
    }

    int steps = DEG_TO_STEPS(m, degrees);
    LOG_INF("[MOCKED] move_stepper(%.1f degrees, %d steps) called", (double)degrees, steps);
    
    /* Simulate the time it would take */
    k_msleep(steps / 4);
    
    LOG_INF("[MOCKED] move_stepper completed");
    return 0;
}