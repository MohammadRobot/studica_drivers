#include "duty_cycle_encoder.h"
#include <chrono>
#include <thread>

/*
 * Duty-cycle encoder example checklist
 *
 * Run:
 *   ./duty_cycle_encoder_example
 *
 * Expected output cues:
 * - Prints absolute position [deg], rollover count, and total rotation [deg].
 * - Values should change as the shaft rotates.
 */

int main(int argc, char *argv[])
{
    // Use the PWM-capture capable channel connected to your duty-cycle encoder.
    const VMXChannelIndex encoder_port = 10;
    studica_driver::DutyCycleEncoder encoder(encoder_port);

    for (int i = 0; i < 20; ++i) {
        const double absolute_deg = encoder.GetAbsolutePosition();
        const int rollover = encoder.GetRolloverCount();
        const double total_deg = encoder.GetTotalRotation();

        printf("Sample %02d | abs=%.3f deg | rollover=%d | total=%.3f deg\n",
            i,
            absolute_deg,
            rollover,
            total_deg);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
