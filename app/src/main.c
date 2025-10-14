#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "stepper_driver.h"

LOG_MODULE_REGISTER(main_app, LOG_LEVEL_INF);

// GPIO port and pin definitions
#define STEP_PORT  DT_NODELABEL(gpiob)
#define STEP_PIN   3
#define DIR_PORT   DT_NODELABEL(gpiob)
#define DIR_PIN    5
#define EN_PORT    DT_NODELABEL(gpiob)
#define EN_PIN     4

static struct stepper_t motor;

int main(void)
{
    const struct device *step_dev = DEVICE_DT_GET(STEP_PORT);
    const struct device *dir_dev = DEVICE_DT_GET(DIR_PORT);
    const struct device *en_dev  = DEVICE_DT_GET(EN_PORT);

    motor.step_pin.port = step_dev;
    motor.step_pin.pin = STEP_PIN;
    motor.step_pin.dt_flags = GPIO_ACTIVE_HIGH;

    motor.dir_pin.port = dir_dev;
    motor.dir_pin.pin = DIR_PIN;
    motor.dir_pin.dt_flags = GPIO_ACTIVE_HIGH;

    motor.en_pin.port = en_dev;
    motor.en_pin.pin = EN_PIN;
    motor.en_pin.dt_flags = GPIO_ACTIVE_HIGH;

    motor.steps_per_rev = 200;
    motor.microsteps = 16;

    LOG_INF("Munch Stepper Control App Started");

    if (stepper_init(&motor) != 0) {
        LOG_ERR("Stepper initialization failed");
        return -1;
    }

    const float angles[] = {30.0f, 45.0f, 90.0f, 180.0f};

    for (int i = 0; i < ARRAY_SIZE(angles); i++) {
        LOG_INF("Moving motor %.1f degrees", (double)angles[i]);
        stepper_move_deg(&motor, angles[i]);
        k_sleep(K_MSEC(1000));
    }

    LOG_INF("All moves completed successfully!");
    return 0;
}
