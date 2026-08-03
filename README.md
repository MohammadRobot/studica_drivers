# Studica VMXPi Drivers

This package builds the Studica VMXPi driver library inside your ROS 2 workspace.

## Attribution

Portions of this package are derived from
[`Studica-Robotics/ROS2`](https://github.com/Studica-Robotics/ROS2),
licensed under Apache-2.0.

Driver sources are synced with `Studica-Robotics/ROS2` `main` at commit
`0c31344` from 2026-06-18.

## Maintainer

- Mohammad Alshamsi (`alshamsi.mohammad@gmail.com`)

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

The upstream public headers now use `.hpp`. This package installs those headers and
keeps `.h` compatibility wrappers for older code.

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
- `light_tower` -> `examples/light_tower_example/light_tower_example.cpp`
- `parsec` -> `examples/parsec_example/parsec_example.cpp`
- `pwm` -> `examples/!pwm_example/pwm_example.cpp`
- `servo` -> `examples/servo_example/continuous.cpp`, `examples/servo_example/linear.cpp`, `examples/servo_example/standard.cpp`
- `sharp` -> `examples/sharp_example/sharp_example.cpp`
- `titan_encoder` -> `examples/titan_encoder_example/titan_encoder_example.cpp`
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

Run examples individually:

```bash
cd ~/ros2_ws/src/studica_drivers/examples/<example_directory>
make
sudo ./<example_binary>
```

For example directories whose names start with `!`, quote only the path component:

```bash
cd ~/ros2_ws/src/studica_drivers/examples/'!encoder_example'
make
sudo ./encoder_example
```

Parsec supports CAN and USB modes:

```bash
cd ~/ros2_ws/src/studica_drivers/examples/parsec_example
make
sudo ./parsec_example can 0
./parsec_example usb /dev/ttyACM0
```
