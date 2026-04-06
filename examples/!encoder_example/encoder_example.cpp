#include "encoder.h"
#include <chrono>
#include <string>
#include <thread>

/*
 * Quadrature encoder example checklist
 *
 * Run:
 *   ./encoder_example
 *
 * Expected output cues:
 * - Prints tick count and direction.
 * - Count should increase/decrease as wheel/shaft rotates.
 */

int main(int argc, char *argv[])
{
    // Adjust A/B channels to your encoder wiring.
    const VMXChannelIndex encoder_a = 2;
    const VMXChannelIndex encoder_b = 3;

    studica_driver::Encoder encoder(encoder_a, encoder_b);

    for (int i = 0; i < 30; ++i) {
        const int count = encoder.GetCount();
        const std::string direction = encoder.GetDirection();
        printf("Sample %02d | count=%d | direction=%s\n", i, count, direction.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
