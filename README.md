# pe_forge
A Windows PE (Process Executable) format editor and viewer. Originally built as an internal tool for another project, this library is under active development and evolves alongside its parent project's needs.

## Features
- Parse and modify PE files
- Add custom sections
- Read and write PE headers

## Requirements
C++23

## Installation
1. Build the static library (.lib) for your target architecture
2. Include the headers from the `include/` directory in your project.
3. Link against the compiled library.
4. Use includes like
```c
#include <pe_forge/<subfolder>/<header>>
```