#include "servo.h"
#include <thread>
#include <chrono>

/*
 * Continuous servo example checklist
 *
 * Run:
 *   ./continuous
 *
 * Expected output cues:
 * - Servo spins one direction, then the opposite direction, then stops.
 * - Uses angle API as signed speed command for continuous mode.
 */

#define SERVO_PIN 14
int main(int argc, char *argv[])
{
    VMXChannelIndex port = SERVO_PIN;
    // Continuous mode maps [-100, 100] to reverse/forward speed.
    studica_driver::Servo servo(port, studica_driver::ServoType::Continuous, -100, 100);  
    // Exercise Servo::SetBounds override.
    servo.SetBounds(1.0, 1.5, 2.0);

    // Exercise Servo::SetSpeed.
    servo.SetSpeed(80);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servo.SetSpeed(-80);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servo.SetSpeed(0);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Exercise Servo::SetAngle (same PWM path for this implementation).
    servo.SetAngle(100);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servo.SetAngle(-100);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servo.SetAngle(0);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Exercise Servo::GetLastAngle.
    printf("Last servo command: %.1f\n", servo.GetLastAngle());
    return 0;
}
