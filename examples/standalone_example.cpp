#include <iostream>
#include <vector>
#include <chrono>

extern "C" {
#include "controller_mellinger.h"
#include "stabilizer_types.h"
#include "math3d.h"
}

int main() {
    std::cout << "=== Crazyflie Firmware Integration Example ===" << std::endl;
    
    // Initialize Mellinger controller
    controllerMellinger_t controller;
    controllerMellingerInit(&controller);
    
    // Simulation parameters
    const double dt = 0.001; // 1ms timestep
    const int simulation_steps = 1000; // 1 second simulation
    
    // Initial state
    state_t state = {0};
    state.position.z = 0.0f;
    
    // Target setpoint - hover at 1 meter
    setpoint_t setpoint = {0};
    setpoint.mode.z = modeAbs;
    setpoint.position.z = 1.0f;
    setpoint.mode.x = modeVelocity;
    setpoint.velocity.x = 0.0f;
    setpoint.mode.y = modeVelocity;
    setpoint.velocity.y = 0.0f;
    setpoint.mode.yaw = modeVelocity;
    setpoint.attitudeRate.yaw = 0.0f;
    
    // Sensor data (simulated)
    sensorData_t sensors = {0};
    
    // Control output
    control_t control = {0};
    
    std::vector<float> altitude_history;
    std::vector<float> thrust_history;
    
    std::cout << "Simulating takeoff to 1 meter altitude..." << std::endl;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Simulation loop
    for (int step = 0; step < simulation_steps; ++step) {
        uint32_t tick = step;
        
        // Run controller
        controllerMellinger(&controller, &control, &setpoint, &sensors, &state, tick);
        
        // Simple physics simulation (very basic)
        // In a real simulation, you'd integrate proper dynamics
        float thrust_normalized = control.thrust / 65536.0f; // Normalize thrust
        float acceleration = (thrust_normalized - 1.0f) * 9.81f; // Approximate
        
        // Update state (crude integration)
        state.velocity.z += acceleration * dt;
        state.position.z += state.velocity.z * dt;
        
        // Damping to prevent oscillation in this simple model
        state.velocity.z *= 0.999f;
        
        // Log every 100ms
        if (step % 100 == 0) {
            altitude_history.push_back(state.position.z);
            thrust_history.push_back(control.thrust);
            
            std::cout << "Step " << step << ": "
                      << "Alt=" << state.position.z << "m, "
                      << "Thrust=" << control.thrust << ", "
                      << "Vel=" << state.velocity.z << "m/s"
                      << std::endl;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << std::endl;
    std::cout << "=== Simulation Results ===" << std::endl;
    std::cout << "Final altitude: " << state.position.z << " meters" << std::endl;
    std::cout << "Target altitude: " << setpoint.position.z << " meters" << std::endl;
    std::cout << "Simulation time: " << duration.count() << " microseconds" << std::endl;
    std::cout << "Performance: " << (simulation_steps * 1000000.0) / duration.count() 
              << " steps per second" << std::endl;
    
    // Test math3d functions
    std::cout << std::endl;
    std::cout << "=== Math3D Example ===" << std::endl;
    
    struct vec v1 = mkvec(1.0f, 2.0f, 3.0f);
    struct vec v2 = mkvec(4.0f, 5.0f, 6.0f);
    
    struct vec sum = vadd(v1, v2);
    struct vec cross = vcross(v1, v2);
    float dot = vdot(v1, v2);
    float magnitude = vmag(v1);
    
    std::cout << "v1 = (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
    std::cout << "v2 = (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;
    std::cout << "v1 + v2 = (" << sum.x << ", " << sum.y << ", " << sum.z << ")" << std::endl;
    std::cout << "v1 × v2 = (" << cross.x << ", " << cross.y << ", " << cross.z << ")" << std::endl;
    std::cout << "v1 · v2 = " << dot << std::endl;
    std::cout << "|v1| = " << magnitude << std::endl;
    
    std::cout << std::endl;
    std::cout << "Integration complete! This demonstrates how to use Crazyflie firmware" << std::endl;
    std::cout << "functions directly in a C++ application for simulation and testing." << std::endl;
    
    return 0;
}
