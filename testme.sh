#!/bin/bash

## To execute the script without running CMake, run ./testme.sh.
## To execute the script with CMake configuration, run ./testme.sh -b.

# Check if build directory exists, create if not
if [ ! -d "build" ]; then
    mkdir build
fi

# Go to the build directory
cd build || exit

# Check the command-line flags
while getopts ":b" flag; do
    case $flag in
        b)
            # Execute cmake command
            cmake ..
            if [ $? -ne 0 ]; then
                echo "Error: CMake configuration failed."
                exit 1
            fi
            ;;
        \?)
            echo "Error: Invalid flag -$OPTARG."
            exit 1
            ;;
    esac
done

# Execute make command
make
if [ $? -ne 0 ]; then
    echo "Error: Make command failed."
    exit 1
fi

# Go to the src directory
cd src || exit

# Run the generated executable
./MyBPM
if [ $? -ne 0 ]; then
    echo "Error: Failed to run the generated executable."
    exit 1
fi
