# **SCUT-RobotLab:Vrep Template Project**

## **Introduction**

- Project to simulate Robot with C++ edited in Vscode. 
- Please have a look at Gimble::task() located in the gimble.cpp, you 'll have a macro understanding of using vrep for motor/joint control.
- If you are interseted in this project and need more information, please contact Mentos Seetoo(1356046979@qq.com) or Clang(clangwu@163.com) to get an access.

## **How to run this program**?

### **1. Run with simulation**

- **Start your Simulation:**
  1. Open scence file `kr3.ttt` with CoppeliaSim.
  2. Compile and run program `./build/bin/kr3.exe`
  3. Start simulation using the button in CoppeliaSim.

### **2. Run on your real robot** 

## **Implementation details**


### **2. Motion Controller**

- **Balance controller based on kinematic model**


- **Joint Control in Simulation**

  In our real robot, we use PD controller for joint control. And We hope this lower layer controller would be well characterised in simulation. However, CoppeliSim Robotics does not provide a proper way to perform highband PD control, so we have to run PD controller in coppeliasim custom client, which control frequency is lower than 200Hz.

  We still don't know whether this method works well. Module testing is needed but we havn't finished.

  PS: In our test, static torque error is vrey small and we can assume that there's no error between target and simulation output. 

  Related URL：

  - https://forum.coppeliarobotics.com/viewtopic.php?f=9&t=3933&sid=0b2737c12ac1442b617536addbe9cfad
  - https://www.coppeliarobotics.com/helpFiles/en/jointCallbackFunctions.htm
  - https://www.coppeliarobotics.com/helpFiles/en/jointDynamicsProperties.htm

## **Dependencies**

### **Running Enviroment :**

> 1. Python 3 is required for matplotlib, but Numpy is optional.
> 2. It's highly recommended to use GCC. If you run on win32 platform, you should install MinGW.

**Ubuntu 18.04 LTS / Windows 10.0+**

- CMake (version 3.12.0 +) 

- Python 3 : Numpy,  Matplotlib

  ```bash
  ~$ sudo apt-get install python-matplotlib python-numpy python2.7-dev
  ```


### **Sumulation Enviroment :**

- Coppeliasim (Recommended version: 4.0.0)

- Qt ([version 5.12.0 +](http://download.qt.io/archive/qt/5.12/5.12.9/), Not required on Windows)

