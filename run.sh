#!/bin/bash

# OBSBOT Linux GUI Run Script

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
RUN_DIR="$SCRIPT_DIR/build/rundir"

# Set library path
export LD_LIBRARY_PATH="$RUN_DIR:$LD_LIBRARY_PATH"

# Run the application
cd "$RUN_DIR"
./OBSBOT_Linux_GUI "$@"
