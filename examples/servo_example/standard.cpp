#include "servo.h"
#include <thread>
#include <chrono>

/*
 * Standard servo example checklist
 *
 * Run:
 *   ./standard
 *
 * Expected output cues:
 * - Servo sweeps to one end, the other end, then center.
 * - Range here is configured as [-150, 150] for this specific setup.
 */

#define SERVO_PIN 13

int main(int argc, char *argv[])
{
    VMXChannelIndex port = SERVO_PIN;
    // Standard positional servo with configured min/max angle range.
    studica_driver::Servo servo(port, studica_driver::ServoType::Standard, -150, 150);  
    servo.SetAngle(150);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servo.SetAngle(-150);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servo.SetAngle(0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Last servo command: %.1f\n", servo.GetLastAngle());
    return 0;
}
