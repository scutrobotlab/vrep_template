#pragma once
#include <iostream>

        enum 
        {
                yaw = 0,
                pitch,
                roll
        };
        enum 
        {
                lf_wheel = 0,
                lh_wheel,
                rf_wheel,
                rh_wheel,
                servo
        };
        typedef struct JointData
        {
                float curr_vel[5];
                float curr_pos[5];
                float curr_torque[5];
                float tar_vel[5];
                float tar_pos[5];
                float tar_torque[5];

                float out[5];
        }JointDataTypedef;
        

        typedef struct CarData
        {
                float roll, pitch, yaw;
                float ox,oy,oz;   
                JointDataTypedef joint_data;
        }CarDataTypedef;
        
        class mycar
        {
        private:
                /* data */
        public:
                mycar(/* args */);
                ~mycar();
                void car_spin();
                CarDataTypedef mycar_data;
        };
        
        

