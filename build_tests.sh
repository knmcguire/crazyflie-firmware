#!/bin/bash

# Build script for Crazyflie standalone tests

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building Crazyflie Standalone Tests...${NC}"

# Create build directory
BUILD_DIR="build_standalone"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Check if we need to generate config header
if [ ! -f "../build/include/generated/autoconf.h" ]; then
    echo -e "${YELLOW}Generating configuration headers...${NC}"
    mkdir -p ../build/include/generated
    
    # Create a minimal autoconf.h for testing
    cat > ../build/include/generated/autoconf.h << 'EOF'
#ifndef __AUTOCONF_H__
#define __AUTOCONF_H__

// Minimal configuration for standalone tests
#define CONFIG_KALMAN_CORE_ENABLED 1
#define CONFIG_ESTIMATOR_KALMAN_ENABLE 1
#define CONFIG_CONTROLLER_PID_IMPROVED_BARO_HOLD 1

#endif // __AUTOCONF_H__
EOF
fi

# Run CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo -e "${YELLOW}Building...${NC}"
make -j$(nproc)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful!${NC}"
    echo -e "${GREEN}Executables are in: $(pwd)/bin${NC}"
    echo ""
    echo "To run all tests:"
    echo "  cd $BUILD_DIR && ./bin/crazyflie_tests"
    echo ""
    echo "To run a specific test:"
    echo "  cd $BUILD_DIR && ./bin/crazyflie_tests controller_mellinger"
    echo ""
    echo "To run the integration example:"
    echo "  cd $BUILD_DIR && ./bin/standalone_example"
    echo ""
    echo "Available test suites:"
    echo "  - controller_mellinger : Test Mellinger controller"
    echo "  - controller_pid       : Test PID controller" 
    echo "  - math3d              : Test 3D math utilities"
    echo "  - planner             : Test motion planner"
    echo "  - pptraj              : Test polynomial trajectories"
    echo "  - power_distribution  : Test power distribution"
    echo "  - kalman_core         : Test Kalman filter (disabled due to C++ issues)"
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
