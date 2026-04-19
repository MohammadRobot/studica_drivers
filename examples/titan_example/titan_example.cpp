#include "titan.h"
#include <thread>
#include <chrono>
#include <cstdint>
#include <cstring>

/*
 * Titan example test checklist
 *
 * Run:
 *   ./titan_example
 *   ./titan_example --run-autotune
 *
 * Expected output cues:
 * 1. "=== Basic Info ==="
 *    - Prints ID, frequency, serial, firmware, hardware, temperature.
 *    - If CAN is not healthy, fields may be zeros or "(read failed)".
 *
 * 2. "=== Encoder Setup ==="
 *    - No errors expected; encoders are configured and reset.
 *
 * 3. "=== Enable + Open-Loop Speed Commands ==="
 *    - Robot/wheels should move forward then reverse.
 *    - Commands are resent every 50 ms to satisfy keepalive timing.
 *
 * 4. "=== Telemetry Readback ==="
 *    - RPM or encoder counts should change after motion.
 *    - TryGetRPM "ok=true" indicates a valid RPM frame was read.
 *
 * 5. "=== Titan2 API Calls ==="
 *    - Calls complete without crash.
 *    - GetTargetRPMFromDevice may return false on older firmware.
 *
 * 6. "=== Inversion Helpers (runtime flags) ==="
 *    - Confirms inversion helper methods are exercised.
 *
 * 7. "=== Stop + Disable ==="
 *    - Final commands send zero speed and disable outputs.
 *    - Ends with "Titan example complete."
 */

namespace {

// Titan expects regular command traffic; this helper keeps time units explicit.
void keepalive_sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Send a 4-motor open-loop command for a fixed duration.
// Commands are resent every 50 ms to stay well below the CAN timeout window.
void drive_for(studica_driver::Titan &titan, double m0, double m1, double m2, double m3, int duration_ms)
{
    const int step_ms = 50; // < TITAN_CAN_KEEPALIVE_MS
    for (int elapsed = 0; elapsed < duration_ms; elapsed += step_ms) {
        titan.SetSpeed(0, m0);
        titan.SetSpeed(1, m1);
        titan.SetSpeed(2, m2);
        titan.SetSpeed(3, m3);
        keepalive_sleep_ms(step_ms);
    }
}

} // namespace

int main(int argc, char *argv[])
{
    // Optional flag: run controller autotune (can move hardware unexpectedly).
    bool run_autotune = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--run-autotune") == 0) {
            run_autotune = true;
        }
    }

    // Demo defaults used by the rest of this repository.
    const uint8_t can_id = 42;
    const uint16_t motor_freq = 15600;
    const float dist_per_tick = 0.0006830601f; // ~1464 ticks / rotation in this setup

    // Construct driver and allow startup configuration messages to settle.
    studica_driver::Titan titan(can_id, motor_freq, dist_per_tick);
    keepalive_sleep_ms(1000); // wait for Titan config

    // Basic board identity / status reads.
    printf("=== Basic Info ===\n");
    printf("ID: %u\n", titan.GetID());
    printf("Configured Frequency: %u\n", titan.GetFrequency());
    printf("Serial Number: %s\n", titan.GetSerialNumber().c_str());
    printf("Firmware Version: %s\n", titan.GetFirmwareVersion().c_str());
    printf("Hardware Version: %s\n", titan.GetHardwareVersion().c_str());
    printf("Controller Temp: %.2f C\n", titan.GetControllerTemp());

    // Initialize encoder scaling and clear counts before motion tests.
    printf("\n=== Encoder Setup ===\n");
    for (uint8_t m = 0; m < 4; ++m) {
        titan.SetupEncoder(m);
        titan.ConfigureEncoder(m, dist_per_tick);
        titan.ResetEncoder(m);
    }

    // Open-loop motor exercise in both directions.
    printf("\n=== Enable + Open-Loop Speed Commands ===\n");
    titan.Enable(true);
    titan.SetPIDType(0); // required for SetSpeed duty-cycle control on current Titan firmware
    printf("PID Type set to 0 (SetSpeed mode)\n");
    drive_for(titan, 0.20, 0.20, 0.20, 0.20, 1000);
    drive_for(titan, -0.20, -0.20, -0.20, -0.20, 1000);

    // Exercise broadcast speed command API.
    for (int i = 0; i < 10; ++i) {
        titan.SetSpeedAll(0.15);
        keepalive_sleep_ms(50);
    }
    for (int i = 0; i < 10; ++i) {
        titan.SetSpeedAll(0.0);
        keepalive_sleep_ms(50);
    }

    // Readback after motion to verify counters and telemetry APIs.
    printf("\n=== Telemetry Readback ===\n");
    for (uint8_t m = 0; m < 4; ++m) {
        int16_t rpm_try = 0;
        const bool got_rpm = titan.TryGetRPM(m, &rpm_try);
        printf(
            "M%u | RPM(GetRPM)=%d RPM(TryGetRPM)=%d ok=%s | EncRaw=%d EncDist=%.6f\n",
            m,
            titan.GetRPM(m),
            rpm_try,
            got_rpm ? "true" : "false",
            titan.GetEncoderCount(m),
            titan.GetEncoderDistance(m));
        printf(
            "M%u | Limit(reverse)=%s Limit(forward)=%s\n",
            m,
            titan.GetLimitSwitch(m, 0) ? "true" : "false",
            titan.GetLimitSwitch(m, 1) ? "true" : "false");
    }
    printf("Cypher Angle Port0: %.2f\n", titan.GetCypherAngle(0));
    printf("Cypher Angle Port1: %.2f\n", titan.GetCypherAngle(1));

    // Titan2 feature calls: setpoints and controller configuration APIs.
    printf("\n=== Titan2 API Calls ===\n");
    for (uint8_t m = 0; m < 4; ++m) {
        titan.SetTargetVelocity(m, (m % 2 == 0) ? 120 : -120);
        titan.SetTargetDistance(m, 1200 + static_cast<int32_t>(m) * 100);
        titan.SetTargetAngle(m, 45.0 + static_cast<double>(m) * 15.0);
        titan.SetPositionHold(m, true);
        titan.SetEncoderResolution(m, 1464);
        titan.SetCurrentLimit(m, 2.0f);
        titan.SetCurrentLimitMode(m, 1);
        titan.SetSensitivity(m, 5);
    }
    titan.SetMotorStopMode(1);
    {
        int16_t target_rpm[4] = {0, 0, 0, 0};
        const bool got_target_rpm = titan.GetTargetRPMFromDevice(target_rpm);
        printf(
            "GetTargetRPMFromDevice ok=%s values=[%d, %d, %d, %d]\n",
            got_target_rpm ? "true" : "false",
            target_rpm[0], target_rpm[1], target_rpm[2], target_rpm[3]);
    }
    titan.DisableMotor(0);

    if (run_autotune) {
        // Autotune can be disruptive on a live robot, so it is opt-in.
        printf("Running AutotuneAll()...\n");
        titan.AutotuneAll();
        keepalive_sleep_ms(200);
    } else {
        printf("Skipping AutotuneAll() (pass --run-autotune to execute it)\n");
    }

    // Inversion helper calls update driver-side sign conventions.
    printf("\n=== Inversion Helpers (runtime flags) ===\n");
    titan.InvertMotorDirection(0);
    titan.InvertMotorRPM(1);
    titan.InvertEncoderDirection(2);
    titan.InvertMotor(3);

    // Explicit stop + disable for deterministic shutdown.
    printf("\n=== Stop + Disable ===\n");
    titan.SetSpeed(0, 0.0);
    titan.SetSpeed(1, 0.0);
    titan.SetSpeed(2, 0.0);
    titan.SetSpeed(3, 0.0);
    titan.Enable(false);
    keepalive_sleep_ms(500);

    printf("Titan example complete.\n");
    return 0;
}
