# OpenCV Installation Guide

### Download

You can download OpenCV from [here](https://opencv.org/releases/).

### Setup

After installing the package follow the steps below:

1. Copy **opencv.hpp** file from `opencv\build\include\opencv2` directory to `opencv\build\include` directory.

2. Add the `bin` directory to Path in Environment Variables. In my case, I added `D:\ProgramFiles\opencv\build\x64\vc15\bin` to Path.

3. Add a new `variable` in Environment Variables and name it `OPEN_CV` with the value of `build` directory. e.g. `D:\ProgramFiles\opencv\build`

<p align="center">
  <img alt="OpenCV-Environment-Variable" src="/Setup/OpenCV/OpenCV-Environment-Variable.jpg" width="500px"></img>
</p>

4. Now in Visual Studio open Property Manager tab and based on running the code in Debug, Release, x64 or Win32 you can add this property file ([OpenCV-vc15-x64.props](/Setup/OpenCV/OpenCV-vc15-x64.props)) and run the code.

<p align="center">
  <img alt="OpenCV-Property-Manager" src="/Setup/OpenCV/OpenCV-Property-Manager.jpg" width="320px"></img>
</p>

You are all done OpenCV should work for your program.

##### Note: You might need to restart you windows.
