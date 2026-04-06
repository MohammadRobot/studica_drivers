#include "servo.h"
#include <thread>
#include <chrono>

/*
 * Linear servo example checklist
 *
 * Run:
 *   ./linear
 *
 * Expected output cues:
 * - Actuator moves to minimum, maximum, then midpoint extension.
 * - Uses SetBounds() to match your actuator pulse calibration.
 */

#define SERVO_PIN 12
int main(int argc, char *argv[])
{
    VMXChannelIndex port = SERVO_PIN;
    studica_driver::Servo servo(port, studica_driver::ServoType::Linear, 0, 100);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // Typical [ms] pulse-width bounds: min, center, max.
    servo.SetBounds(1, 1.5, 2);
    servo.SetAngle(0);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    servo.SetAngle(100);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    servo.SetAngle(50);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printf("Last servo command: %.1f\n", servo.GetLastAngle());
    return 0;
}
