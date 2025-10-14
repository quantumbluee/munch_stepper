#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(stepper_test, LOG_LEVEL_INF);

// Replace these with your actual GPIO PORT and PIN numbers as needed
#define STEP_PORT  DT_NODELABEL(gpiob)
#define STEP_PIN   3
#define DIR_PORT   DT_NODELABEL(gpiob)
#define DIR_PIN    5
#define EN_PORT    DT_NODELABEL(gpiob)
#define EN_PIN     4

void main(void)
{
    const struct device *step_dev = DEVICE_DT_GET(STEP_PORT);
    const struct device *dir_dev = DEVICE_DT_GET(DIR_PORT);
    const struct device *en_dev  = DEVICE_DT_GET(EN_PORT);

    gpio_pin_configure(step_dev, STEP_PIN, GPIO_OUTPUT);
    gpio_pin_configure(dir_dev, DIR_PIN, GPIO_OUTPUT);
    gpio_pin_configure(en_dev, EN_PIN, GPIO_OUTPUT);

    LOG_INF("Stepper hardware test started");

    // Set DIR (choose either HIGH or LOW for direction)
    gpio_pin_set(dir_dev, DIR_PIN, 1);

    // Enable driver
    LOG_INF("Driver EN low (should hold torque)");
    gpio_pin_set(en_dev, EN_PIN, 0);
    k_msleep(2000); // Wait 2 seconds to feel holding torque

    // Disable driver
    LOG_INF("Driver EN high (should freely rotate)");
    gpio_pin_set(en_dev, EN_PIN, 1);
    k_msleep(2000); // Wait 2 seconds for no holding torque

    // Re-enable driver for step pulse test
    LOG_INF("Driver EN low (test step pulses)");
    gpio_pin_set(en_dev, EN_PIN, 0);
    k_msleep(200);

    // Send 20 step pulses (should make motor move slightly)
    for (int i = 0; i < 20; i++) {
        gpio_pin_set(step_dev, STEP_PIN, 1);
        k_usleep(30);           // 30 µs HIGH pulse
        gpio_pin_set(step_dev, STEP_PIN, 0);
        k_usleep(1000);         // 1 ms LOW
    }

    LOG_INF("Pulse test done: repeat to diagnose.");
    k_msleep(10000); // Wait so you can observe result
}
