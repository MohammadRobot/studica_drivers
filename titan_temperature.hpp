#pragma once

#include <cstdint>

namespace studica_driver
{
namespace titan_protocol
{

/** Titan 2.0.5 publishes the MCU_TEMP whole/hundredths payload in Fahrenheit. */
inline bool firmware_reports_temperature_in_fahrenheit(
    uint8_t firmware_major, uint8_t firmware_minor, uint8_t firmware_patch)
{
    return firmware_major == 2u && firmware_minor == 0u && firmware_patch == 5u;
}

/** Decode MCU_TEMP data[0..1] and normalize the supported payload to Celsius. */
inline float decode_controller_temperature_c(
    uint8_t whole, uint8_t hundredths,
    uint8_t firmware_major, uint8_t firmware_minor, uint8_t firmware_patch)
{
    const float raw_temperature =
        static_cast<float>(whole) + (static_cast<float>(hundredths) / 100.0f);
    if (firmware_reports_temperature_in_fahrenheit(
            firmware_major, firmware_minor, firmware_patch))
    {
        return (raw_temperature - 32.0f) * (5.0f / 9.0f);
    }
    return raw_temperature;
}

}  // namespace titan_protocol
}  // namespace studica_driver
