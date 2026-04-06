#include "ultrasonic.h"
#include <thread>
#include <chrono>

/*
 * Ultrasonic example checklist
 *
 * Run:
 *   ./ultrasonic_example
 *
 * Expected output cues:
 * - First block prints distance in inches.
 * - Second block prints distance in millimeters.
 * - Readings should vary when target distance changes.
 */

int main(int argc, char *argv[])
{
    VMXChannelIndex ping = 8;
    VMXChannelIndex echo = 9;
    studica_driver::Ultrasonic ultrasonic(ping, echo);

    // Ping/read loop in inches.
    for (int i = 0; i < 100; i++) {
        ultrasonic.Ping();
        std::this_thread::sleep_for(std::chrono::milliseconds(55));
        printf("Distance (inches): %.2f\n", ultrasonic.GetDistanceIN());
    }
    // Ping/read loop in millimeters.
    for (int i = 0; i < 100; i++) {
        ultrasonic.Ping();
        std::this_thread::sleep_for(std::chrono::milliseconds(55));
        printf("Distance (mm): %.2f\n", ultrasonic.GetDistanceMM());
    }
    return 0;
}
