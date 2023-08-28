default: run

# Build the project
build:
    g++ src/main.cpp

# Execute the built file
execute:
    ./a.out

# Build then execute
run: build execute
