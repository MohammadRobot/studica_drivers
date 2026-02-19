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
# studica_drivers
