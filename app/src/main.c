#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "stepper_driver.h"

LOG_MODULE_REGISTER(main_app, LOG_LEVEL_INF);

static const struct stepper_t motor = {
    .step_pin = {
        .port = DEVICE_DT_GET(DT_NODELABEL(gpiob)),
        .pin = 3,
        .dt_flags = GPIO_ACTIVE_HIGH
    },
    .dir_pin = {
        .port = DEVICE_DT_GET(DT_NODELABEL(gpiob)),
        .pin = 5,
        .dt_flags = GPIO_ACTIVE_HIGH
    },
    .en_pin = {
        .port = DEVICE_DT_GET(DT_NODELABEL(gpiob)),
        .pin = 4,
        .dt_flags = GPIO_ACTIVE_HIGH
    },
    .steps_per_rev = 200,
    .microsteps = 16
};

int main(void)
{
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