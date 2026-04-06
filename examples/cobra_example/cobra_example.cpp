#include "cobra.h"
#include <thread>
#include <chrono>
#include "VMXPi.h"

/*
 * Cobra example checklist
 *
 * Run:
 *   ./cobra_example
 *
 * Expected output cues:
 * - Prints one voltage reading and one raw ADC value for channel 1.
 * - Voltage should be within expected sensor range for your wiring.
 */

int main(int argc, char *argv[]) {

    // Open VMXPi connection used by the Cobra ADC helper.
    std::shared_ptr<VMXPi> vmx_ = std::make_shared<VMXPi>(true, 50);

    // Use 5.0V as ADC reference voltage for conversion to engineering units.
    studica_driver::Cobra cobra(vmx_, 5.0F);
    float voltage = cobra.GetVoltage(1);
    int value = cobra.GetRawValue(1);
    printf("\nVoltage: %f", voltage);
    printf("\nADC Value: %d", value);
}
