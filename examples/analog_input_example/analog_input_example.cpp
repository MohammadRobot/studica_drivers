#include "analog_input.h"
#include <chrono>
#include <thread>

/*
 * Analog input example checklist
 *
 * Run:
 *   ./analog_input_example
 *
 * Expected output cues:
 * - Prints averaged voltage from the selected analog channel.
 * - Voltage should change when sensor/input signal changes.
 */

int main(int argc, char *argv[])
{
    // Default analog-capable channel for quick testing; adjust for your wiring.
    const VMXChannelIndex port = 22;
    studica_driver::AnalogInput analog(port);

    for (int i = 0; i < 10; ++i) {
        float average_voltage = 0.0f;
        const bool ok = analog.GetAverageVoltage(average_voltage);
        printf("Sample %d | ok=%s | avg_voltage=%.4f V\n",
            i,
            ok ? "true" : "false",
            average_voltage);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
