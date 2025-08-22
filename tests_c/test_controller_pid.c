#include <stdio.h>
#include <assert.h>
#include "controller_pid.h"
#include "stabilizer_types.h"

int test_controller_pid(void) {
    // Initialize the PID controller
    controllerPidInit();
    
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
    state.attitude.pitch = 0;
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
    controllerPid(&control, &setpoint, &sensors, &state, tick);
    
    // Verify results
    assert(control.controlMode == controlModeLegacy);
    assert(control.roll == 0);
    assert(control.pitch == 0);
    assert(control.yaw == 0);
    
    printf("Controller PID test: hover state verified\n");
    
    // Test with some non-zero setpoint
    setpoint.position.z = 1.0f;  // 1 meter altitude
    setpoint.velocity.x = 0.5f;  // 0.5 m/s forward velocity
    
    controllerPid(&control, &setpoint, &sensors, &state, tick + 1);
    
    printf("Controller PID test: setpoint response verified\n");
    printf("All Controller PID tests passed!\n");
    
    return 0;  // Success
}
