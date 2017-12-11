> This project is in a very rough state. Please use with caution!

# Biomechanic Depth Matching

This project is a prototype implementation of the method described in *[Reichard, Häntsch et al. (2017). "Projective biomechanical depth matching for soft tissue registration in laparoscopic surgery". International Journal of Computer Assisted Radiology and Surgery, 1-10.]*


## Getting Started

### Prerequisities

You'll need to install:

 * PHP (Minimum 7.0)
 * G++ Compiler (Minimum 4.8)
 * CUDA/NVCC (Minimum 7.0)
 * CMAKE (Minimum 3.1)
 * GLFW (Minimum 3.2)
 
 * External requirements for SOFA (https://github.com/sofa-framework) 


### Building

```bash
git clone https://github.com/Reichard/biomechanic_depth_matching.git
cd biomechanic_depth_matching
mkdir build
cd build
cmake ..
make
```

## Usage

### Tool

The project comes with two sample scenes that illustrate how experiments can be implemented.

```bash
bin/run ../data/scenes/liver/scene.scn
```

## Authors

See the list of [contributors](https://github.com/reichard/biomechanic_depth_matching/contributors) who participated in this project.

