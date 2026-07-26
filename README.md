# Quantitative Options Pricing Engine

A high-performance options pricing system implementing both Black-Scholes and Binomial Lattice models. The core logic is written in C++ for maximum computational speed, exposed to a FastAPI Python backend via PyBind11, and visualized through a professional React.js dashboard.

## Features
* **C++ Core Engine:** Supports continuous (Black-Scholes) and discrete (Binomial) pricing models.
* **Greeks Calculation:** Real-time computation of Delta, Gamma, Vega, Theta, and Rho.
* **Python Bindings:** Seamless integration between C++ and Python using `pybind11`.
* **Institutional UI:** A sleek, dark-mode React frontend optimized for financial data visualization.

## Dependencies
* [CMake](https://cmake.org/download/) (>= 3.10.0)
* C++ Compiler supporting C++17 (MSVC, MinGW, or GCC)
* [Python 3.8+](https://www.python.org/downloads/)
* [Node.js & npm](https://nodejs.org/) (v16+)
* [PyBind11](https://github.com/pybind/pybind11) (Fetched automatically via CMake)
* [nlohmann/json](https://github.com/nlohmann/json) (Fetched automatically via CMake)

## Installation (Windows)

1. **Clone the repository:**
   ```cmd
   git clone [https://github.com/andrew-moscheni/options-pricing-engine.git](https://github.com/andrew-moscheni/options-pricing-engine.git)
   cd options-pricing-engine