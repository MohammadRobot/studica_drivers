#include "pwm.h"
#include <chrono>
#include <thread>

/*
 * PWM base-class example checklist
 *
 * Run:
 *   ./pwm_example
 *
 * Expected output cues:
 * - Initializes a PWM generator channel.
 * - Applies custom pulse bounds used by higher-level servo classes.
 *
 * Note:
 * - The PWM class is a base utility; direct motion commands are provided by Servo.
 */

int main(int argc, char *argv[])
{
    const VMXChannelIndex pwm_port = 13;

    // Standard profile with angle-style min/max range.
    studica_driver::PWM pwm(
        pwm_port,
        studica_driver::PWMType::Standard,
        -150,
        150);

    // Set 1.0 ms min, 1.5 ms center, 2.0 ms max pulse bounds.
    pwm.SetBounds(1.0, 1.5, 2.0);
    printf("PWM initialized on port %d and bounds configured.\n", pwm_port);

    // Keep process alive briefly so logs/state can be observed.
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
