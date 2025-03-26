# Mindset - Neuronal scene Representation and Management Library

© 2025. VG-Lab / URJC

## Introduction

Mindset is a C++ library designed for efficient representation, management, and analysis of complex neural scenes. It provides robust data structures and utility functions to manage datasets consisting of neurons, synapses, morphologies, hierarchical node structures, and associated properties.

### Key Features:
- Efficient neuron and synapse management
- Hierarchical organization of scene components
- Rich property management system
- Simple version tracking of dataset modifications
- Flexible loading mechanism supporting multiple input formats

## Dependencies

### Required:
- C++20 compatible compiler
- [Rush](https://github.com/gaeqs/Rush) (Mathematical operations and transformations)
- [Hey!](https://github.com/gaeqs/Hey) (Observability pattern)

### Optional:
- Brion (for loading BlueConfig and MorphoIO file types)

## Building

Mindset has been successfully built on Linux (Ubuntu 22.04), MacOS (Monterey, Sonoma), and Windows (Visual Studio 2022).

Build steps:
```bash
git clone https://gitlab.gmrv.es/g.rial/mindset
mkdir mindset/build && cd mindset/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Usage

After building Mindset, you can integrate it directly into your project by linking against the generated library:

```cmake
find_package(mindset REQUIRED)
target_link_libraries(your_target PRIVATE mindset)
```

## Example

```cpp
#include <mindset/Dataset.h>

int main()
{
    mindset::Dataset dataset;
    dataset.reserveSpaceForNeurons(100);
    
    // Add a neuron
    mindset::Neuron neuron(mindset::UID(1));
    dataset.addNeuron(std::move(neuron));

    // Manage neurons...

    return 0;
}
```