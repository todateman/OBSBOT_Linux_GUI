#!/bin/bash

# OBSBOT Linux GUI Build Script

echo "Building OBSBOT Linux GUI..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

# Install to rundir
make install

echo ""
echo "Build complete!"
echo "Run the application with: ./build/rundir/obsbot_pantilt_gui"
