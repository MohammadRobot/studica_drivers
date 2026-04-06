# Studica VMXPi Drivers

This package builds the Studica VMXPi driver library inside your ROS 2 workspace.

## Requirements

- VMXPi library installed on the target machine.
  See the Studica VMXPi OS image/docs for installation.

## Build (Workspace)

From your workspace root:

```bash
cd ~/ros2_ws
colcon build --packages-select studica_drivers
source install/setup.bash
```

If the VMXPi library is not found, this package builds as a stub with no hardware support.
This allows simulation builds to proceed, but driver-based nodes will be skipped.

## Optional: Standalone Makefile Build

If you need a standalone driver build (outside of colcon):

```bash
make
make install
make clean
```

## Examples

Examples use a Makefile and expect VMXPi to be installed.

```bash
cd examples
make
```

Available C++ examples (one per driver):

- `analog_input` -> `examples/analog_input_example/analog_input_example.cpp`
- `cobra` -> `examples/cobra_example/cobra_example.cpp`
- `dio` -> `examples/dio_example/dio_example.cpp`
- `duty_cycle_encoder` -> `examples/duty_cycle_encoder_example/duty_cycle_encoder_example.cpp`
- `encoder` -> `examples/!encoder_example/encoder_example.cpp`
- `i2c` -> `examples/i2c_example/i2c_example.cpp`
- `imu` -> `examples/imu_example/imu_example.cpp`
- `pwm` -> `examples/!pwm_example/pwm_example.cpp`
- `servo` -> `examples/servo_example/continuous.cpp`, `examples/servo_example/linear.cpp`, `examples/servo_example/standard.cpp`
- `sharp` -> `examples/sharp_example/sharp_example.cpp`
- `titan` -> `examples/titan_example/titan_example.cpp`
- `ultrasonic` -> `examples/ultrasonic_example/ultrasonic_example.cpp`

## Run Examples

Before running examples:

```bash
cd ~/ros2_ws
source install/setup.bash
```

Build all examples at once:

```bash
cd src/studica_drivers/examples
make
```

Run each example individually:

```bash
# analog_input
cd ~/ros2_ws/src/studica_drivers/examples/analog_input_example
make
sudo ./analog_input_example

# cobra
cd ~/ros2_ws/src/studica_drivers/examples/cobra_example
make
sudo ./cobra_example

# dio
cd ~/ros2_ws/src/studica_drivers/examples/dio_example
make
sudo ./dio_example

# duty_cycle_encoder
cd ~/ros2_ws/src/studica_drivers/examples/duty_cycle_encoder_example
make
sudo ./duty_cycle_encoder_example

# encoder (note the quoted path because of '!')
cd '~/ros2_ws/src/studica_drivers/examples/!encoder_example'
make
sudo ./encoder_example

# i2c
cd ~/ros2_ws/src/studica_drivers/examples/i2c_example
make
sudo ./i2c_example

# imu
cd ~/ros2_ws/src/studica_drivers/examples/imu_example
make
sudo ./imu_example
# optional: exercise callback/register API path
sudo ./imu_example --exercise-callback-api

# pwm (note the quoted path because of '!')
cd '~/ros2_ws/src/studica_drivers/examples/!pwm_example'
make
sudo ./pwm_example

# servo
cd ~/ros2_ws/src/studica_drivers/examples/servo_example
make
sudo ./continuous
sudo ./linear
sudo ./standard

# sharp
cd ~/ros2_ws/src/studica_drivers/examples/sharp_example
make
sudo ./sharp_example

# titan
cd ~/ros2_ws/src/studica_drivers/examples/titan_example
make
sudo ./titan_example
# optional: runs autotune call
sudo ./titan_example --run-autotune

# ultrasonic
cd ~/ros2_ws/src/studica_drivers/examples/ultrasonic_example
make
sudo ./ultrasonic_example
```
