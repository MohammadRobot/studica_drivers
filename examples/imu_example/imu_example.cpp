#include "imu.h"
#include <thread>
#include <chrono>
#include <cstdint>
#include <cstring>

/*
 * IMU example checklist
 *
 * Run:
 *   ./imu_example
 *   ./imu_example --exercise-callback-api
 *
 * Expected output cues:
 * - Exercises all public IMU driver APIs at least once.
 * - Core orientation/accel/gyro values should change with robot motion.
 * - Callback/register functions are optional (flag-gated) because they are
 *   transport/firmware dependent.
 */

int main(int argc, char *argv[])
{
    bool exercise_callback_api = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--exercise-callback-api") == 0) {
            exercise_callback_api = true;
        }
    }

    // Construct IMU with default VMXPi context.
    studica_driver::Imu imu;

    printf("=== IMU Status ===\n");
    printf("Connected=%s Calibrating=%s Moving=%s Rotating=%s\n",
        imu.IsConnected() ? "true" : "false",
        imu.IsCalibrating() ? "true" : "false",
        imu.IsMoving() ? "true" : "false",
        imu.IsRotating() ? "true" : "false");

    printf("\n=== Orientation ===\n");
    printf("Yaw=%.3f Pitch=%.3f Roll=%.3f Compass=%.3f Fused=%.3f\n",
        imu.GetYaw(),
        imu.GetPitch(),
        imu.GetRoll(),
        imu.GetCompassHeading(),
        imu.GetFusedHeading());
    printf("Quaternion [w x y z] = [%.5f %.5f %.5f %.5f]\n",
        imu.GetQuaternionW(),
        imu.GetQuaternionX(),
        imu.GetQuaternionY(),
        imu.GetQuaternionZ());

    printf("\n=== Linear Accel / Velocity / Displacement ===\n");
    printf("WorldAccel [x y z] = [%.4f %.4f %.4f]\n",
        imu.GetWorldLinearAccelX(),
        imu.GetWorldLinearAccelY(),
        imu.GetWorldLinearAccelZ());
    imu.ResetDisplacement();
    imu.UpdateDisplacement(0.0f, 0.0f, 50.0f, false);
    printf("Velocity [x y z] = [%.4f %.4f %.4f]\n",
        imu.GetVelocityX(),
        imu.GetVelocityY(),
        imu.GetVelocityZ());
    printf("Displacement [x y z] = [%.4f %.4f %.4f]\n",
        imu.GetDisplacementX(),
        imu.GetDisplacementY(),
        imu.GetDisplacementZ());

    printf("\n=== Raw Sensors ===\n");
    printf("Gyro [x y z] = [%.4f %.4f %.4f]\n",
        imu.GetRawGyroX(),
        imu.GetRawGyroY(),
        imu.GetRawGyroZ());
    printf("Accel [x y z] = [%.4f %.4f %.4f]\n",
        imu.GetRawAccelX(),
        imu.GetRawAccelY(),
        imu.GetRawAccelZ());
    printf("Mag [x y z] = [%.4f %.4f %.4f]\n",
        imu.GetRawMagX(),
        imu.GetRawMagY(),
        imu.GetRawMagZ());

    printf("\n=== Environment / Meta ===\n");
    printf("BarometricPressure=%.3f Altitude=%.3f AltitudeValid=%s\n",
        imu.GetBarometricPressure(),
        imu.GetAltitude(),
        imu.IsAltitudeValid() ? "true" : "false");
    printf("Pressure=%.3f TempC=%.3f\n", imu.GetPressure(), imu.GetTempC());
    printf("ByteCount=%d UpdateCount=%d LastSensorTimestamp=%d\n",
        imu.GetByteCount(),
        imu.GetUpdateCount(),
        imu.GetLastSensorTimestamp());
    printf("RequestedUpdateRate=%d ActualUpdateRate=%d\n",
        imu.GetRequestedUpdateRate(),
        imu.GetActualUpdateRate());
    printf("MagDisturbance=%s MagCalibrated=%s\n",
        imu.IsMagneticDisturbance() ? "true" : "false",
        imu.IsMagnetometerCalibrated() ? "true" : "false");

    // These methods return opaque AHRS state values; still exercised here.
    const BoardYawAxis board_yaw_axis = imu.GetBoardYawAxis();
    (void)board_yaw_axis;
    printf("FirmwareVersion=%s\n", imu.GetFirmwareVersion().c_str());
    printf("Angle=%.3f Rate=%.3f\n", imu.GetAngle(), imu.GetRate());

    // Exercise state-management APIs.
    imu.ZeroYaw();
    imu.Reset();

    if (exercise_callback_api) {
        printf("\n=== Callback/Register API (Optional) ===\n");
        // Passing nullptr callback is only for API-path exercise.
        const bool reg_ok = imu.RegisterCallback(nullptr, nullptr);
        const bool dereg_ok = imu.DeregisterCallback(nullptr);

        uint8_t first_reg_addr = 0;
        uint8_t data[32] = {0};
        uint8_t out_len = 0;
        const bool block_ok = imu.BlockOnNewCurrentRegisterData(
            5, &first_reg_addr, data, static_cast<uint8_t>(sizeof(data)), &out_len);
        const bool read_cfg_ok = imu.ReadConfigurationData(0, data, 8);

        printf("RegisterCallback=%s DeregisterCallback=%s\n",
            reg_ok ? "true" : "false",
            dereg_ok ? "true" : "false");
        printf("BlockOnNewCurrentRegisterData=%s first_reg=0x%02X len=%u\n",
            block_ok ? "true" : "false",
            first_reg_addr,
            out_len);
        printf("ReadConfigurationData=%s\n", read_cfg_ok ? "true" : "false");
    } else {
        printf("\nSkipping callback/register API (use --exercise-callback-api)\n");
    }

    // Explicit stop API; destructor also stops AHRS.
    imu.Stop();
    return 0;
}
