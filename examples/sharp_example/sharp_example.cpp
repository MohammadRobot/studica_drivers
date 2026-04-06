#include "sharp.h"
#include <thread>
#include <chrono>

/*
 * Sharp IR distance sensor example checklist
 *
 * Run:
 *   ./sharp_example
 *
 * Expected output cues:
 * - Prints voltage and computed distance repeatedly.
 * - Distance should decrease as an object gets closer to the sensor.
 */

int main(int argc, char *argv[])
{
    VMXChannelIndex port = 22; // either one of 22~25
    studica_driver::Sharp sharp(port);
    // Sample voltage/distance in a short loop for quick sensor validation.
    for (int i = 0; i < 10; ++i) {
        printf("%d |  Voltage: %f, ", i, sharp.GetVoltage());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        printf("Distance: %f\n", sharp.GetDistance());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
