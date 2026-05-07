# Chess Engine
## Summary
This repository is dedicated to a chess engine which is optimized for performance, written in modern C++20.

## Features
1. Mechanical sympathy and cache optimization
	- Utilization of bitboards which start on a new cache boundary, preventing false sharing in multithreaded searching algorithms and reducing data fetches to just one instruction.
2. Robustness (we all hate [bugs](https://de.wikipedia.org/wiki/Datei:First_Computer_Bug,_1947.jpg)!)
	- "If it isn't tested, it's broken" - Bruce Eckel
	- Unlike many pet projects, this engine values and heavily relies on unit tests.
3. Forsyth-Edwards Notation
	- Full support for FEN strings to import and export (in foreseeable future!) board states.
## Getting Started
### Dependencies
- Standard: C++20
- Build System: CMake 3.15+
- Compiler: GCC 11+, Clang 13+ or MSVC 19.3+
- Unit Testing: Google Test 1.12.0+
- Documentation: Doxygen 1.9.5+
### Build
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
### Run the Tests
```
./build/unit_tests
```
### Generate Documentation
The code is documented using Doxygen. To generate the HTML docs run:
```
make docs
```
## About the Author
I am self-taught low-level systems enthusiast who has been programming intensively since end of 2024. This project was started at the beginning of my second semester as a computer science major.

Thank you for taking the time to read through my project!