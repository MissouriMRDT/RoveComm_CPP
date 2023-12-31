\mainpage

# RoveComm C++

Welcome to the RoveComm C++ repository of the Mars Rover Design Team (MRDT) at Missouri University of Science and Technology (Missouri S&T)! API reference contains the source code and other resources for the development of RoveComm C++. The RoveComm C++ project is the RoveComm interface for high level systems and should not be used for microcontrollers.

### API Documentation
Everytime a commit is made to the development branch, a GitHub Action is ran that autonomatically generates documentation for files, classes, methods, functions, namespaces, etc. using Doxygen. To ensure that Doxygen can properly document and use your comments, make sure to use the documentation templates HERE. If you're running the development container in VSCode the templates can be automatically generated by typing `/**<enter>`. Regardless, all file, method, and function documentation must be use the template's style across the whole project.

#### [RoveComm C++ API Docs HERE](https://missourimrdt.github.io/RoveComm_CPP/)


## Overview

The RoveComm C++ project is organized into different directories, each serving a specific purpose. Here's a brief overview of the directories:

- **tests**: Contains test cases and test scripts to ensure the correctness and reliability of our networking protocols. These files help validate the functionality and performance of our code, ensuring robustness and accuracy in real-world scenarios.

- **tools**: Contains files that provide utility scripts, development tools, or miscellaneous functionalities for our project. These files assist in various development tasks, such as data analysis, visualization, or simulation, enhancing the overall development experience.

- **examples**: Contains example code snippets, demos, or sample implementations related to our Rover project. These files showcase specific functionalities, best practices, or usage scenarios, helping us understand and leverage the capabilities of RoveComm C++ effectively.

- **docs**: Contains documentation files and resources for our Rover project. These files provide comprehensive and accessible documentation to guide developers, users, and contributors in understanding, configuring, and extending of RoveComm C++.

- **data**: Contains data files and resources used in our Rover project. These files include calibration parameters, control parameters, network protocols, facilitating the development, testing, and analysis of our autonomy algorithms.

The **src** directory is what is packaged into the finished library to be used by other projects within the Mars Rover Design Team.

## Getting Started

To get started with our RoveComm C++ development, follow these steps:

1. Clone the repository to your local machine using the command:
   ```
   git clone --recurse-submodules -j8 https://github.com/MissouriMRDT/RoveComm_CPP.git
   ```

2. Navigate to the cloned directory:
   ```
   cd RoveComm_CPP
   ```

3. Explore the different directories to understand the structure and purpose of each.

4. Refer to the specific README files within each directory for detailed information and guidelines on organizing files and using the functionalities.

5. Install any required dependencies or external libraries mentioned in the documentation or README files.

6. Start developing
