# **SCUT-RobotLab: Scarlet Tiger**
## **Introduction**
- Project of General Quadruped Platform 
- Refer to: MIT Mini Cheetah, ETH Anymal, IIT HyQReal, Stanford Doggo.
- If you are interseted in this project and need more information, please contact Mentos Seetoo(1356046979@qq.com) to get an OneNote access.
  
<div align=center>
<img src="./docs/brief.png" alt="Brief" >
</div>

## **How to run this program**?

### **1. Run with simulation**

- **Start your Simulation:**
  1. Open scence file `tiger.ttt` with CoppeliaSim and start simulation.
  2. Compile and run program `./robot/build/bin/Launcher`

### **2. Run on your real robot** 

## **Implementation details**

### **1. Motion Scheduler**

#### **Foot step placement:**

#### **The following gaits is supported for now:**
                   
1. **Stand**:  phase     0.0->0.5->0.0->0.5
                period     0.6                        Unit:second

2. **Walk**:   phase     0.0->0.5->0.25->0.75
                period    0.6                        Unit:second
3. **Trot**:   phase     0.0->0.5->0.25->0.75
                period     0.6                        Unit:second

4. **Pace**:   phase     0.0->0.5->0.5->0.0
                period    0.36                       Unit:second
               
5. **Gallop**: phase     0.0->0.5->0.5->0.0
                period    0.36                       Unit:second

#### **The following trajectory types are supported for now:**

- **Sinusoidal curve, Cycloid curve, Polynomial curve**.  
To get more information for the trajectory,please contact Silang Wu(1134138135@qq.com).

### **2. Motion Controller**
- **Balance controller based on kinematic model**


- **Joint Control in Simulation**

  In our real robot, we use PD controller for joint control. And We hope this lower layer controller would be well characterised in simulation. However, CoppeliSim Robotics does not provide a proper way to perform highband PD control, so we have to run PD controller in coppeliasim custom client, which control frequency is lower than 200Hz.

  We still don't know whether this method work well under this situation. Module testing is necessary but we havn't finished.



## **Dependencies**

**Running Enviroment :**

- Ubuntu 18.04 LTS (Recommended)

- CMake (version 3.12.0 +) 

- Python 2 : Numpy,  Matplotlib

  ```bash
  ~$ sudo apt-get install python-matplotlib python-numpy python2.7-dev
  ```

- GTK+3 (Not used for now)

  ```bash
  ~$ sudo apt-get install libgtk-3-dev
  ```

- Eigen3

  ```bash
  ~$ sudo apt-get install libeigen3-dev
  ```

**Sumulation Enviroment :**

- Coppeliasim 4.0.1 (Recommended)

- Qt ([version 5.12.0 +](http://download.qt.io/archive/qt/5.12/5.12.9/))

