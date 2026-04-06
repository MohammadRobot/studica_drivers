#include "i2c.h"
#include <memory>

/*
 * I2C example checklist
 *
 * Run:
 *   ./i2c_example
 *
 * Expected output cues:
 * - Scans bus and prints first detected I2C address.
 * - Attempts one-byte transaction and optional register read.
 */

int main(int argc, char *argv[])
{
    std::shared_ptr<VMXPi> vmx = std::make_shared<VMXPi>(true, 50);
    studica_driver::I2C i2c(vmx);

    if (!i2c.isOpen()) {
        printf("VMX is not open; aborting I2C example.\n");
        return 1;
    }

    const uint8_t device = i2c.scanI2CBus();
    if (device == 0) {
        printf("No I2C device found on bus.\n");
        return 0;
    }

    printf("Using first detected device at 0x%02X\n", device);

    uint8_t rx = 0;
    const bool txn_ok = i2c.i2cTransaction(device, nullptr, 0, &rx, 1);
    printf("Raw transaction ok=%s rx=0x%02X\n", txn_ok ? "true" : "false", rx);

    // Write one byte to register 0x00 (many devices may ignore or NACK this register).
    uint8_t tx = 0x01;
    const bool write_ok = i2c.WriteI2C(device, 0x00, &tx, 1);
    printf("WriteI2C success=%s\n", write_ok ? "true" : "false");

    uint8_t reg_data = 0;
    const bool read_ok = i2c.ReadI2C(device, 0x00, &reg_data, 1);
    printf("ReadI2C success=%s data=0x%02X\n", read_ok ? "true" : "false", reg_data);

    return 0;
}
