#pragma once
#include "common/common.h"
#include "controller/pid_ctrl.h"
#include "controller/lqr_ctrl.h"

        enum 
        {
                yaw = 0,
                pitch,
                roll
        };

        typedef struct GimbleBodyData
        {
                /*---status---*/
                float pitch;
                float yaw;
                float roll;
        }BodyDataTypedef;

        class Gimble
        {
        private:
                /* data */
                PIDParamTypedef _pos, _vel, _tau;
                pid_ctrl pid;
        public:
                Gimble(/* args */);
                ~Gimble();
                void init();
                void task();
                void adjust();
                JointDataTypedef gimble_joint;
                BodyDataTypedef  gimble_body;
        };
        
        

