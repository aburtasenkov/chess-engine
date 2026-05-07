# Chess Engine
## Summary
This repository is dedicated to a chess engine which is optimized for performance, written in modern C++20.

## Features
1. Mechanical sympathy and cache optimization
	- Utilization of bitboards which start on a new cache boundary, preventing false sharing in multithreaded searching algorithms and reducing data fetches to just one instruction.
2. Robustness (we all hate [bugs](https://de.wikipedia.org/wiki/Datei:First_Computer_Bug,_1947.jpg)!)
	- Strict FEN parsing
	- Unit tests (and you don't see these very often in pet projects haha!)
## Getting Started
### Dependencies
- CMake 3.15+
- Modern compiler (e.g. GCC 11+ or Clang 13+)
- C++20 or newer
### Build
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
### Run The Tests
```
./build/unit_tests
```
### Generate Documentation
```
make docs
```