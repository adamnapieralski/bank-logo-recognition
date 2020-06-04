# bank-logo-recognition
Project for Digital Image Processing course [POBR] on FEIT | WUT. </br>

## Installation

### Prerequisites
- CMake >=v3.0
- OpenCV

### Steps (on Linux)
1. Create directory for the build output: e.g. `mkdir build/`
2. In build directory (`cd build`) execute cmake to generate build scripts: `cmake ..`
3. Build project with `make`.

## Usage
File that executes logo recognition pipeline is `build/main`. It has to be provided with argument defining path to the image that is supposed to be processed. E.g. (in main project directory):
```
./build/main data/img4.png
```
This will show and save image with marked detected logos.<br>
Adding second argument `-s` will provide more complex information about processing in console, as well as save multiple files from certain steps of processing algorithm.
```
./build/main data/img4.png -s
```
