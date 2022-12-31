# Project Setup

After cloning the repository and making an Empty Project in Visual Studio you need to follow steps below to run the sample programs.

## OpenCV

[Read Here](/Setup/OpenCV)

## Visual Studio

You need to follow these three steps to complete the setup:

1. From **Solution Explorer** tab you need to add all the required header and source files to the project. Also add the source file of the sample code you want to run, and open it. e.g. `NN-Sample-01.cpp`

<p align="center">
  <img alt="Project-Solution-Explorer" src="/Setup/Project-Solution-Explorer.jpg" width="400px"></img>
</p>

2. From Project > Properties > Configuration Properties > General you need to change **C++ Language Standard** to **`ISO C++17 Standard (/std:c++17)`**.

<p align="center">
  <img alt="Project-Properties-1" src="/Setup/Project-Properties-1.jpg" width="650px"></img>
</p>

3. And also from Configuration Properties > C/C++ you need to edit **Additional Include Directories** and add [`Include`](/Include) directory. e.g. `D:\MyCodes\GitHub\neural-network-cpp\Include`

<p align="center">
  <img alt="Project-Properties-2" src="/Setup/Project-Properties-2.jpg" width="650px"></img>
</p>

#### Note: Make sure all the setup is done in the same compile mode (Debug or Release, x64 or x86)
