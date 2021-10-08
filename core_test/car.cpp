#include "car.h"
void mycar::car_spin()
{
        uint8_t dir = 1;
        mycar_data.joint_data.tar_pos[servo] = 45*3.14/180 * dir;

        for (size_t i = 0; i < 2; i++)
                mycar_data.joint_data.tar_vel[i] = -10;
        for (size_t i = 2; i < 4; i++)
                mycar_data.joint_data.tar_vel[i] = 10;

        for (size_t i = 0; i < 4; i++){
                if(i == servo){
                        mycar_data.joint_data.out[i] = mycar_data.joint_data.tar_pos[i];
                        return;
                }
                mycar_data.joint_data.out[i] =  mycar_data.joint_data.tar_vel[i];
        }
}

mycar::mycar(/* args */){}
mycar::~mycar(){}
