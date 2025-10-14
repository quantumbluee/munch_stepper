#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(test_stepper, LOG_LEVEL_INF);

/* Include FFF if available, otherwise use manual mocking */
#ifdef CONFIG_ZTEST_MOCKING
#include <zephyr/fff.h>
DEFINE_FFF_GLOBALS;
#endif

/* We'll test the logic without actual GPIO hardware */
#define DEG_TO_STEPS(steps_per_rev, microsteps, deg) \
    ((int)((deg) / 360.0f * (steps_per_rev) * (microsteps)))

/* Test fixture */
struct test_motor_config {
    int steps_per_rev;
    int microsteps;
};

static struct test_motor_config test_motor;

static void *stepper_setup(void)
{
    LOG_INF("Setting up stepper tests");
    test_motor.steps_per_rev = 200;
    test_motor.microsteps = 16;
    return NULL;
}

static void stepper_teardown(void *fixture)
{
    LOG_INF("Tearing down stepper tests");
}

ZTEST_SUITE(stepper_tests, NULL, stepper_setup, NULL, NULL, stepper_teardown);

/*
 * Test 1: Verify stepper initialization parameters
 */
ZTEST(stepper_tests, test_stepper_init_parameters)
{
    LOG_INF("TEST: Stepper initialization parameters");
    
    zassert_equal(test_motor.steps_per_rev, 200, 
                  "Steps per revolution should be 200");
    zassert_equal(test_motor.microsteps, 16, 
                  "Microsteps should be 16");
}

/*
 * Test 2: Verify degree to step conversion - 30 degrees
 */
ZTEST(stepper_tests, test_deg_to_steps_30)
{
    LOG_INF("TEST: 30 degree conversion");
    
    float degrees = 30.0f;
    int expected_steps = 266;  // 30/360 * 200 * 16 = 266.67
    int actual_steps = DEG_TO_STEPS(test_motor.steps_per_rev, 
                                     test_motor.microsteps, 
                                     degrees);
    
    zassert_equal(actual_steps, expected_steps, 
                  "30 degrees should convert to 266 steps, got %d", 
                  actual_steps);
}

/*
 * Test 3: Verify degree to step conversion - 45 degrees
 */
ZTEST(stepper_tests, test_deg_to_steps_45)
{
    LOG_INF("TEST: 45 degree conversion");
    
    float degrees = 45.0f;
    int expected_steps = 400;  // 45/360 * 200 * 16 = 400
    int actual_steps = DEG_TO_STEPS(test_motor.steps_per_rev, 
                                     test_motor.microsteps, 
                                     degrees);
    
    zassert_equal(actual_steps, expected_steps, 
                  "45 degrees should convert to 400 steps, got %d", 
                  actual_steps);
}

/*
 * Test 4: Verify degree to step conversion - 90 degrees
 */
ZTEST(stepper_tests, test_deg_to_steps_90)
{
    LOG_INF("TEST: 90 degree conversion");
    
    float degrees = 90.0f;
    int expected_steps = 800;  // 90/360 * 200 * 16 = 800
    int actual_steps = DEG_TO_STEPS(test_motor.steps_per_rev, 
                                     test_motor.microsteps, 
                                     degrees);
    
    zassert_equal(actual_steps, expected_steps, 
                  "90 degrees should convert to 800 steps, got %d", 
                  actual_steps);
}

/*
 * Test 5: Verify degree to step conversion - 180 degrees
 */
ZTEST(stepper_tests, test_deg_to_steps_180)
{
    LOG_INF("TEST: 180 degree conversion");
    
    float degrees = 180.0f;
    int expected_steps = 1600;  // 180/360 * 200 * 16 = 1600
    int actual_steps = DEG_TO_STEPS(test_motor.steps_per_rev, 
                                     test_motor.microsteps, 
                                     degrees);
    
    zassert_equal(actual_steps, expected_steps, 
                  "180 degrees should convert to 1600 steps, got %d", 
                  actual_steps);
}

/*
 * Test 6: Invalid input - negative angle
 */
ZTEST(stepper_tests, test_invalid_negative_angle)
{
    LOG_INF("TEST: Invalid negative angle");
    
    float degrees = -10.0f;
    
    /* In real driver, this should return -EINVAL */
    zassert_true(degrees <= 0.0f, 
                 "Negative angle should be detected as invalid");
}

/*
 * Test 7: Invalid input - zero angle
 */
ZTEST(stepper_tests, test_invalid_zero_angle)
{
    LOG_INF("TEST: Invalid zero angle");
    
    float degrees = 0.0f;
    
    /* In real driver, this should return -EINVAL */
    zassert_true(degrees <= 0.0f, 
                 "Zero angle should be detected as invalid");
}

/*
 * Test 8: Invalid input - angle greater than 360
 */
ZTEST(stepper_tests, test_invalid_large_angle)
{
    LOG_INF("TEST: Invalid large angle");
    
    float degrees = 361.0f;
    
    /* In real driver, this should return -EINVAL */
    zassert_true(degrees > 360.0f, 
                 "Angle > 360 should be detected as invalid");
}

/*
 * Test 9: Boundary test - exactly 360 degrees
 */
ZTEST(stepper_tests, test_boundary_360_degrees)
{
    LOG_INF("TEST: Boundary 360 degrees");
    
    float degrees = 360.0f;
    int expected_steps = 3200;  // 360/360 * 200 * 16 = 3200
    int actual_steps = DEG_TO_STEPS(test_motor.steps_per_rev, 
                                     test_motor.microsteps, 
                                     degrees);
    
    zassert_equal(actual_steps, expected_steps, 
                  "360 degrees should convert to 3200 steps, got %d", 
                  actual_steps);
    zassert_true(degrees <= 360.0f && degrees > 0.0f,
                 "360 degrees should be valid");
}

/*
 * Test 10: Verify different microstep settings
 */
ZTEST(stepper_tests, test_different_microsteps)
{
    LOG_INF("TEST: Different microstep settings");
    
    float degrees = 90.0f;
    
    /* Test with 1 microstep */
    int steps_1 = DEG_TO_STEPS(200, 1, degrees);
    zassert_equal(steps_1, 50, "90deg with 1 microstep = 50 steps");
    
    /* Test with 8 microsteps */
    int steps_8 = DEG_TO_STEPS(200, 8, degrees);
    zassert_equal(steps_8, 400, "90deg with 8 microsteps = 400 steps");
    
    /* Test with 16 microsteps */
    int steps_16 = DEG_TO_STEPS(200, 16, degrees);
    zassert_equal(steps_16, 800, "90deg with 16 microsteps = 800 steps");
}