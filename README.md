# **SCUT-RobotLab:Vrep Template Project**

## **Introduction**

- Project to simulate Robot with C++ edited in Vscode. 
- KUKA(China) is allowed to use internally. Not available to other companies without permission.
- If you are interseted in this project and need more information, please contact Mentos Seetoo(1356046979@qq.com) or Clang(clangwu@163.com) to get an access.  

## **How to run this program**?

### **1. Run with simulation**

- **Start your Simulation:**
  1. Open scence file `test.ttt` with CoppeliaSim.
  2. Compile and run program `./build/bin/test.exe` on Windows, (`./build/bin/test` on Linux)
  3. Start simulation using the button in CoppeliaSim.

### **2. Run on your real robot** 

## **Implementation details**


### **Motion Controller**


- **Joint Control in Simulation**

  ​	In our real robot, we use PD controller for joint control. And We hope this lower layer controller would be well characterised in simulation. However, CoppeliSim Robotics does not provide a proper way to perform highband joint control, which control frequency can only achieve 100Hz.

  ​	To use the template project, you just need to focus on 3 functions:

  ​															`void Usr_ConfigSimulation()；`

  ​															`void Usr_SendFromSimulation()；`

  ​															`void Usr_ReadFromSimulation()；`

  1. Specify the joint:

  ```c++
  void Usr_ConfigSimulation()
  {
      Joint = CoppeliaSim->Add_Object("joint", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_RW });
  }
  ```

  2. Send the torque command to the joint:

  ```C++
  void Usr_SendToSimulation()
  {
      Joint->obj_Target.torque_f = gimble.gimble_joint.tau_d;
  }
  ```

  3. To get the position, velocity and torque of joint:

  ```c++
  void Usr_ReadFromSimulation()
  {
      gimble.gimble_joint.tau_fb = Joint->obj_Data.torque_f;
      gimble.gimble_joint.pos_fb = Joint->obj_Data.angle_f;
      gimble.gimble_joint.vel_fb = Joint->obj_Data.angVelocity_f;
  }
  ```

  ​	If you want to design our controller, you may have a look at the demo:

  ​	`void Usr_Init()；`    and	`void Usr_Main()；`

   

  PS: In our test, both static and dynamic torque error are vrey small and we can assume that there's no error between target and simulation output. 

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

- Python 3 : Numpy,  Matplotlib, Eigen3

  ```bash
  ~$ sudo apt-get install python-matplotlib python-numpy python2.7-dev libeigen3-dev  
  ```


### **Sumulation Enviroment :**

- Coppeliasim (Recommended newer than(also include) version: 4.0.0)

- Qt ([version 5.12.0 +](http://download.qt.io/archive/qt/5.12/5.12.9/), Not required on Windows)

