#include <stdio.h>
#include <math.h>
#include <time.h>
#include "controller_mellinger.h"
#include "stabilizer_types.h"
#include "math3d.h"

static double get_time_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000.0 + ts.tv_nsec / 1000.0;
}

int main(void) {
    printf("=== Crazyflie Firmware Integration Example (Pure C) ===\n");
    
    // Initialize controller
    controllerMellinger_t controller;
    controllerMellingerInit(&controller);
    
    // Simulation setup
    state_t state = {0};
    setpoint_t setpoint = {0};
    sensorData_t sensors = {0};
    control_t control = {0};
    
    // Set target altitude
    setpoint.mode.z = modeAbs;
    setpoint.position.z = 1.0f;  // 1 meter
    setpoint.mode.x = modeVelocity;
    setpoint.velocity.x = 0.0f;
    setpoint.mode.y = modeVelocity;  
    setpoint.velocity.y = 0.0f;
    setpoint.mode.yaw = modeVelocity;
    setpoint.attitudeRate.yaw = 0.0f;
    
    // Simple simulation parameters
    const float dt = 0.01f;  // 10ms timestep
    const int max_steps = 1000;
    
    printf("Simulating takeoff to 1 meter altitude...\n");
    
    double start_time = get_time_us();
    
    for (int step = 0; step < max_steps; step++) {
        // Run controller
        controllerMellinger(&controller, &control, &setpoint, &sensors, &state, step);
        
        // Simple physics simulation
        float thrust_acceleration = (control.thrust - 132000.0f) / 27.0f / 1000.0f;  // Rough conversion
        state.velocity.z += thrust_acceleration * dt;
        state.position.z += state.velocity.z * dt;
        
        // Print progress every 100 steps
        if (step % 100 == 0) {
            printf("Step %d: Alt=%.5fm, Thrust=%.0f, Vel=%.5fm/s\n", 
                   step, state.position.z, control.thrust, state.velocity.z);
        }
        
        // Stop if we're close to target
        if (fabsf(state.position.z - setpoint.position.z) < 0.01f && fabsf(state.velocity.z) < 0.01f) {
            break;
        }
    }
    
    double end_time = get_time_us();
    double simulation_time = end_time - start_time;
    
    printf("\n=== Simulation Results ===\n");
    printf("Final altitude: %.6f meters\n", state.position.z);
    printf("Target altitude: %.0f meters\n", setpoint.position.z);
    printf("Simulation time: %.0f microseconds\n", simulation_time);
    printf("Performance: %.5g steps per second\n", max_steps / (simulation_time / 1000000.0));
    
    // Test Math3D functions
    printf("\n=== Math3D Example ===\n");
    struct vec v1 = mkvec(1.0f, 2.0f, 3.0f);
    struct vec v2 = mkvec(4.0f, 5.0f, 6.0f);
    struct vec sum = vadd(v1, v2);
    struct vec cross = vcross(v1, v2);
    float dot = vdot(v1, v2);
    float mag = vmag(v1);
    
    printf("v1 = (%.0f, %.0f, %.0f)\n", v1.x, v1.y, v1.z);
    printf("v2 = (%.0f, %.0f, %.0f)\n", v2.x, v2.y, v2.z);
    printf("v1 + v2 = (%.0f, %.0f, %.0f)\n", sum.x, sum.y, sum.z);
    printf("v1 × v2 = (%.0f, %.0f, %.0f)\n", cross.x, cross.y, cross.z);
    printf("v1 · v2 = %.0f\n", dot);
    printf("|v1| = %.5f\n", mag);
    
    printf("\nIntegration complete! This demonstrates how to use Crazyflie firmware\n");
    printf("functions directly in a pure C application for simulation and testing.\n");
    
    return 0;
}
