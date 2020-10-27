# **SCUT-RobotLab: Infantry Assassin**
## **Introduction**
- Project to simulate Two Wheeled Jumping Robot. 
- Refer to: ETH Ascento.
- If you are interseted in this project and need more information, please contact Mentos Seetoo(1356046979@qq.com) to get an access.

## **How to run this program**?

### **1. Run with simulation**

- **Start your Simulation:**
  1. Open scence file `Infantry_Assassin.ttt` with CoppeliaSim and start simulation.
  2. Compile and run program `./robot/build/bin/Launcher`

### **2. Run on your real robot** 

## **Implementation details**


### **2. Motion Controller**
- **Balance controller based on kinematic model**


- **Joint Control in Simulation**

  In our real robot, we use PD controller for joint control. And We hope this lower layer controller would be well characterised in simulation. However, CoppeliSim Robotics does not provide a proper way to perform highband PD control, so we have to run PD controller in coppeliasim custom client, which control frequency is lower than 200Hz.

  We still don't know whether this method work well under this situation. Module testing is necessary but we havn't finished.



## **Dependencies**

### **Running Enviroment :**

> - Python 3 is required for matplotlib, but Numpy is optional.
> - It's highly recommended to use GCC. If you run on win32 platform, you should install MinGW.

**Ubuntu 18.04 LTS / Windows 10.0+**

- CMake (version 3.12.0 +) 

- Python 3 : Numpy,  Matplotlib

  ```bash
  ~$ sudo apt-get install python-matplotlib python-numpy python2.7-dev
  ```


### **Sumulation Enviroment :**

- Coppeliasim 4.0.1 (Recommended)

- Qt ([version 5.12.0 +](http://download.qt.io/archive/qt/5.12/5.12.9/))

