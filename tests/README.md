# Crazyflie Firmware Standalone C++ Tests

This directory contains a standalone C++ test suite for the Crazyflie firmware functions, providing an alternative to the Python bindings for testing and simulation purposes.

## Overview

Instead of using SWIG to create Python bindings, this approach compiles the Crazyflie firmware functions directly into a standalone C++ binary. This provides several advantages:

- **Performance**: Direct C++ calls without Python/SWIG overhead
- **Simplicity**: No need for SWIG compilation or Python dependencies
- **Integration**: Easier integration into C++ simulation frameworks
- **Debugging**: Better debugging capabilities with native C++ tools

## Tested Components

The following Crazyflie firmware components are included and tested:

- **Controllers**: Mellinger, PID, Brescianini, Lee controllers
- **Math**: 3D math utilities and vector operations
- **Trajectory Planning**: Piecewise polynomial trajectories (pptraj)
- **Path Planning**: Motion planning algorithms
- **Kalman Filter**: Core Kalman filtering functionality
- **Power Distribution**: Motor control and power distribution
- **Collision Avoidance**: Multi-agent collision avoidance
- **Utilities**: PID controllers, filters, numerical utilities

## Building

### Quick Start

```bash
# Make sure you're in the crazyflie-firmware directory
./build_tests.sh
```

### Manual Build

```bash
# Create build directory
mkdir build_standalone
cd build_standalone

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)
```

## Running Tests

### Run All Tests
```bash
cd build_standalone
./bin/crazyflie_tests
```

### Run Specific Test Suite
```bash
cd build_standalone
./bin/crazyflie_tests controller_mellinger
```

### Run Individual Test Executables
```bash
cd build_standalone
./bin/test_controller_mellinger
./bin/test_math3d
./bin/test_controller_pid
# etc.
```

## Available Test Suites

- `controller_mellinger` - Tests the Mellinger controller
- `controller_pid` - Tests the PID controller
- `math3d` - Tests 3D math utilities and vector operations
- `planner` - Tests motion planning functionality
- `pptraj` - Tests piecewise polynomial trajectories
- `kalman_core` - Tests Kalman filter core functionality
- `power_distribution` - Tests power distribution algorithms

## Adding New Tests

To add a new test suite:

1. **Create header file**: `tests/test_your_component.h`
   ```cpp
   #ifndef TEST_YOUR_COMPONENT_H
   #define TEST_YOUR_COMPONENT_H
   
   void test_your_component();
   
   #endif
   ```

2. **Create implementation**: `tests/test_your_component.cpp`
   ```cpp
   #include "test_your_component.h"
   #include "your_component.h"
   #include <iostream>
   #include <cassert>
   
   void test_your_component() {
       // Your test code here
       std::cout << "All YourComponent tests passed!" << std::endl;
   }
   ```

3. **Update CMakeLists.txt**: Add your test to the `TEST_SOURCES` list

4. **Update main.cpp**: Add the test registration

## Integration with Simulation

This standalone binary can be easily integrated into larger C++ simulation frameworks:

```cpp
#include "controller_mellinger.h"
#include "stabilizer_types.h"

// In your simulation loop:
controllerMellinger_t controller;
controllerMellingerInit(&controller);

// Use controller in your simulation
control_t control;
controllerMellinger(&controller, &control, &setpoint, &sensors, &state, tick);
```

## Comparison with Python Bindings

| Aspect | Python Bindings | C++ Standalone |
|--------|----------------|----------------|
| Performance | Slower (SWIG overhead) | Faster (native C++) |
| Dependencies | Python, SWIG | None (just C++) |
| Integration | Good for Python projects | Better for C++ simulations |
| Debugging | Limited | Full C++ debugging |
| Development | More complex build | Simpler CMake build |

## Troubleshooting

### Missing Headers
If you get missing header errors, ensure the firmware has been built at least once to generate the required configuration headers:

```bash
make # Build the firmware first
./build_tests.sh # Then build tests
```

### Link Errors
Make sure all required source files are included in the `FW_SOURCES` list in `CMakeLists.txt`.

### Assert Failures
The tests use a custom assert handler that throws exceptions. Failed assertions will show detailed error messages including file and line number.

## Future Enhancements

- Add performance benchmarking
- Include more comprehensive trajectory planning tests
- Add real-time constraint testing
- Integration with continuous integration systems
- Memory usage profiling
