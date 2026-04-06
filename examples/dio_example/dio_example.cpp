#include "dio.h"
#include <chrono>
#include <thread>

/*
 * DIO example checklist
 *
 * Run:
 *   ./dio_example
 *
 * Expected output cues:
 * - Toggles a digital output pin HIGH/LOW.
 * - Prints readback from output pin and a separate input pin.
 */

int main(int argc, char *argv[])
{
    // Adjust these channels to match your wiring.
    const VMXChannelIndex output_pin = 0;
    const VMXChannelIndex input_pin = 1;

    studica_driver::DIO dio_out(output_pin, studica_driver::PinMode::OUTPUT);
    studica_driver::DIO dio_in(input_pin, studica_driver::PinMode::INPUT);

    printf("DIO OUT initialized: %s\n", dio_out.IsInitialized() ? "true" : "false");
    printf("DIO IN initialized:  %s\n", dio_in.IsInitialized() ? "true" : "false");

    for (int i = 0; i < 10; ++i) {
        const bool value = (i % 2) == 0;
        dio_out.Set(value);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        printf("Cycle %d | out_set=%s out_read=%s in_read=%s\n",
            i,
            value ? "HIGH" : "LOW",
            dio_out.Get() ? "HIGH" : "LOW",
            dio_in.Get() ? "HIGH" : "LOW");

        dio_out.Toggle();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    dio_out.Set(false);
    return 0;
}
