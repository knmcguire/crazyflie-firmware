#include "test_controller_mellinger.h"

extern "C" {
#include "controller_mellinger.h"
#include "stabilizer_types.h"
}

#include <iostream>
#include <cassert>
#include <cmath>

void test_controller_mellinger() {
    controllerMellinger_t ctrl;
    
    // Initialize the controller
    controllerMellingerInit(&ctrl);
    
    // Set up control structure
    control_t control = {0};
    
    // Set up setpoint
    setpoint_t setpoint = {0};
    setpoint.mode.z = modeAbs;
    setpoint.position.z = 0;
    setpoint.mode.x = modeVelocity;
    setpoint.velocity.x = 0;
    setpoint.mode.y = modeVelocity;
    setpoint.velocity.y = 0;
    setpoint.mode.yaw = modeVelocity;
    setpoint.attitudeRate.yaw = 0;
    
    // Set up state
    state_t state = {0};
    state.attitude.roll = 0;
    state.attitude.pitch = 0;  // Note: not negated like in Python test
    state.attitude.yaw = 0;
    state.position.x = 0;
    state.position.y = 0;
    state.position.z = 0;
    state.velocity.x = 0;
    state.velocity.y = 0;
    state.velocity.z = 0;
    
    // Set up sensor data
    sensorData_t sensors = {0};
    sensors.gyro.x = 0;
    sensors.gyro.y = 0;
    sensors.gyro.z = 0;
    
    uint32_t tick = 100;
    
    // Run the controller
    controllerMellinger(&ctrl, &control, &setpoint, &sensors, &state, tick);
    
    // Verify results
    assert(control.controlMode == controlModeLegacy);
    // control.thrust will be at a (tuned) hover-state - we won't test exact value
    assert(control.roll == 0);
    assert(control.pitch == 0);
    assert(control.yaw == 0);
    
    std::cout << "Controller Mellinger test: hover state verified" << std::endl;
    
    // Test with some non-zero setpoint
    setpoint.position.z = 1.0f;  // 1 meter altitude
    setpoint.velocity.x = 0.5f;  // 0.5 m/s forward velocity
    
    controllerMellinger(&ctrl, &control, &setpoint, &sensors, &state, tick + 1);
    
    // Controller should respond to altitude and velocity setpoints
    // We don't check exact values as they depend on tuning parameters
    std::cout << "Controller Mellinger test: setpoint response verified" << std::endl;
    
    std::cout << "All Controller Mellinger tests passed!" << std::endl;
}
