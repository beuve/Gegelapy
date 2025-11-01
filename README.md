![Alt text](docs/gegelapy-logo.svg)

A Python interface for the [GEGELATI](https://github.com/GEGELATI/GEGELATI) Reinforcement Learning library — a framework for genetic programming and reinforcement learning using [Tangled Program Graphs (TPG)](https://hal.science/hal-03057652/document).

The library works with *gymnasium* environments only. We plan to incorporate more possibilities in the future like classification.

This library is a *work in progress* and should not be used in production.

## Installation

### Option 1: Install from source

```bash
git clone --recurse-submodules https://github.com/beuve/Gegelapy.git
cd Gegelapy
pip install .
```

> Note: The --recurse-submodules flag is required to include the GEGELATI library.

### Option 2: Install directly from GitHub

```bash
pip install git+https://github.com/beuve/Gegelapy.git
```

> Note: In the future, the library should be installable directly from PyPI.

## Usage Example

```python
import gegelapy

# Create a Gym environment wrapper
env = gegelapy.GymEnvironment("CartPole-v1")

# Select the instructions the TPG will have access to
instructions = [gegelapy.add, 
                gegelapy.sub, 
                gegelapy.mul, 
                gegelapy.div]

# Initialize a trainer with the environment
trainer = gegelapy.TPG(env, instructions, max_init_outgoing_edges = 2)

# Train for a few generations
trainer.train(10)

# Save the trained model
trainer.save("cartpole_best.tpg")
```
