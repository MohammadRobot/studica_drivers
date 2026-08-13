#include "titan_temperature.hpp"

#include <gtest/gtest.h>

namespace titan_protocol = studica_driver::titan_protocol;

TEST(TitanTemperature, KeepsLegacyCelsiusPayloadsInCelsius)
{
    EXPECT_FLOAT_EQ(
        titan_protocol::decode_controller_temperature_c(52u, 50u, 2u, 0u, 4u),
        52.5f);
}

TEST(TitanTemperature, ConvertsFirmware205FahrenheitPayloadToCelsius)
{
    EXPECT_NEAR(
        titan_protocol::decode_controller_temperature_c(91u, 0u, 2u, 0u, 5u),
        32.7778f,
        0.001f);
    EXPECT_NEAR(
        titan_protocol::decode_controller_temperature_c(103u, 40u, 2u, 0u, 5u),
        39.6667f,
        0.001f);
}

TEST(TitanTemperature, RestrictsFahrenheitWorkaroundToConfirmedFirmware)
{
    EXPECT_TRUE(titan_protocol::firmware_reports_temperature_in_fahrenheit(2u, 0u, 5u));
    EXPECT_FALSE(titan_protocol::firmware_reports_temperature_in_fahrenheit(2u, 0u, 6u));
    EXPECT_FALSE(titan_protocol::firmware_reports_temperature_in_fahrenheit(2u, 1u, 0u));
}

TEST(TitanTemperature, LeavesUnknownFirmwareFamiliesFailSafe)
{
    EXPECT_FLOAT_EQ(
        titan_protocol::decode_controller_temperature_c(91u, 0u, 3u, 0u, 0u),
        91.0f);
}
