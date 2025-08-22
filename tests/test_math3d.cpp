#include "test_math3d.h"

extern "C" {
#include "math3d.h"
}

#include <iostream>
#include <cassert>
#include <cmath>

void test_math3d() {
    // Test vec creation and access
    struct vec v = mkvec(1.0f, 2.0f, 3.0f);
    assert(v.x == 1.0f);
    assert(v.y == 2.0f);
    assert(v.z == 3.0f);
    std::cout << "Vec creation test passed" << std::endl;
    
    // Test normalize_radians
    float test_angles[] = {-100, -5, 0, M_PI + 0.1f, -M_PI - 0.1f, 100};
    for (float angle : test_angles) {
        float actual = normalize_radians(angle);
        float expected = atan2f(sinf(angle), cosf(angle));
        assert(fabsf(actual - expected) < 1e-6f);
    }
    std::cout << "normalize_radians test passed" << std::endl;
    
    // Test shortest_signed_angle_radians
    float angle_pairs[][2] = {{-M_PI/2, M_PI}, {M_PI/2, M_PI}, {M_PI, -M_PI/3}};
    for (auto& pair : angle_pairs) {
        float start = pair[0];
        float goal = pair[1];
        float actual = shortest_signed_angle_radians(start, goal);
        float expected = atan2f(sinf(goal - start), cosf(goal - start));
        assert(fabsf(actual - expected) < 1e-6f);
    }
    std::cout << "shortest_signed_angle_radians test passed" << std::endl;
    
    // Test vector operations
    struct vec v1 = mkvec(1.0f, 0.0f, 0.0f);
    struct vec v2 = mkvec(0.0f, 1.0f, 0.0f);
    
    // Vector addition
    struct vec sum = vadd(v1, v2);
    assert(sum.x == 1.0f && sum.y == 1.0f && sum.z == 0.0f);
    
    // Vector subtraction
    struct vec diff = vsub(v1, v2);
    assert(diff.x == 1.0f && diff.y == -1.0f && diff.z == 0.0f);
    
    // Vector scaling
    struct vec scaled = vscl(2.0f, v1);
    assert(scaled.x == 2.0f && scaled.y == 0.0f && scaled.z == 0.0f);
    
    // Vector magnitude
    float mag = vmag(mkvec(3.0f, 4.0f, 0.0f));
    assert(fabsf(mag - 5.0f) < 1e-6f);
    
    std::cout << "Vector operations test passed" << std::endl;
    
    // Test vector normalization
    struct vec unnormalized = mkvec(3.0f, 4.0f, 0.0f);
    struct vec normalized = vnormalize(unnormalized);
    float norm_mag = vmag(normalized);
    assert(fabsf(norm_mag - 1.0f) < 1e-6f);
    
    std::cout << "Vector normalization test passed" << std::endl;
    
    // Test cross product
    struct vec cross = vcross(v1, v2);  // (1,0,0) x (0,1,0) = (0,0,1)
    assert(cross.x == 0.0f && cross.y == 0.0f && cross.z == 1.0f);
    
    std::cout << "Vector cross product test passed" << std::endl;
    
    // Test dot product
    float dot = vdot(v1, v2);  // (1,0,0) . (0,1,0) = 0
    assert(dot == 0.0f);
    
    dot = vdot(v1, v1);  // (1,0,0) . (1,0,0) = 1
    assert(dot == 1.0f);
    
    std::cout << "Vector dot product test passed" << std::endl;
    
    std::cout << "All Math3D tests passed!" << std::endl;
}
