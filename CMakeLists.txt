cmake_minimum_required(VERSION 3.16)
project(crazyflie-standalone-tests VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C standard for C files
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fno-strict-aliasing -Wno-address-of-packed-member -DUNIT_TEST_MODE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-strict-aliasing -Wno-address-of-packed-member -DUNIT_TEST_MODE")

# Include directories (from bindings/setup.py)
set(INCLUDE_DIRS
    src/modules/interface
    src/modules/interface/controller
    src/modules/interface/kalman_core
    src/modules/interface/outlierfilter
    src/hal/interface
    src/utils/interface/lighthouse
    src/utils/interface
    build/include/generated
    src/config
    src/drivers/interface
    src/platform/interface
    vendor/CMSIS/CMSIS/DSP/Include
    vendor/CMSIS/CMSIS/Core/Include
)

# Add include directories
foreach(INCLUDE_DIR ${INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR})
endforeach()

# Source files (from bindings/setup.py)
set(FW_SOURCES
    vendor/CMSIS/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/CommonTables/arm_common_tables.c
    vendor/CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/StatisticsFunctions/arm_power_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/MatrixFunctions/arm_mat_mult_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/MatrixFunctions/arm_mat_scale_f32.c
    vendor/CMSIS/CMSIS/DSP/Source/MatrixFunctions/arm_mat_trans_f32.c
    src/modules/src/pptraj.c
    src/modules/src/pptraj_compressed.c
    src/modules/src/planner.c
    src/modules/src/collision_avoidance.c
    src/modules/src/controller/controller_pid.c
    src/modules/src/controller/position_controller_pid.c
    src/modules/src/controller/attitude_pid_controller.c
    src/modules/src/controller/controller_mellinger.c
    src/modules/src/controller/controller_brescianini.c
    src/modules/src/controller/controller_lee.c
    src/utils/src/pid.c
    src/utils/src/filter.c
    src/utils/src/num.c
    src/modules/src/power_distribution_quadrotor.c
    src/modules/src/axis3fSubSampler.c
    src/modules/src/kalman_core/kalman_core.c
    src/modules/src/kalman_core/mm_tdoa.c
    src/modules/src/outlierfilter/outlierFilterTdoa.c
)

# Create a static library from the firmware sources
add_library(cffirmware_static STATIC ${FW_SOURCES})

# Add any necessary compile definitions
target_compile_definitions(cffirmware_static PRIVATE
    UNIT_TEST_MODE=1
    ARM_MATH_CM4=1
    __FPU_PRESENT=1
)

# Find and link math library
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(cffirmware_static ${MATH_LIBRARY})
endif()

# Create the main standalone test executable
add_executable(crazyflie_tests
    tests/main.cpp
    tests/test_controller_mellinger.cpp
    tests/test_controller_pid.cpp
    tests/test_math3d.cpp
    tests/test_planner.cpp
    tests/test_pptraj.cpp
    tests/test_power_distribution.cpp
)

# Link the static library to the executable
target_link_libraries(crazyflie_tests cffirmware_static)

# Add custom assert handler
target_sources(crazyflie_tests PRIVATE tests/cf_assert_handler.cpp)

# Enable testing
enable_testing()

# Add a test that runs our binary
add_test(NAME CrazyflieStandaloneTests COMMAND crazyflie_tests)

# Optional: Create individual test executables for better granularity
# Commenting out for now due to template complexity
# set(TEST_SOURCES
#     test_controller_mellinger
#     test_controller_pid
#     test_math3d
#     test_planner
#     test_pptraj
#     test_power_distribution
# )

# foreach(TEST_NAME ${TEST_SOURCES})
#     add_executable(${TEST_NAME}
#         tests/${TEST_NAME}.cpp
#         tests/cf_assert_handler.cpp
#         tests/test_main_template.cpp
#     )
#     target_link_libraries(${TEST_NAME} cffirmware_static)
#     target_compile_definitions(${TEST_NAME} PRIVATE TEST_FUNCTION_NAME=${TEST_NAME})
#     add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
# endforeach()

# Set output directories
set_target_properties(crazyflie_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Create example executable
add_executable(standalone_example
    examples/standalone_example.cpp
    tests/cf_assert_handler.cpp
)
target_link_libraries(standalone_example cffirmware_static)
set_target_properties(standalone_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Print configuration info
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C flags: ${CMAKE_C_FLAGS}")
message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS}")
