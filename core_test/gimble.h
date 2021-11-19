#pragma once
#include "controller/controller.h""
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
        public:
                Gimble(/* args */);
                ~Gimble();
                void init();
                void task();
                JointDataTypedef gimble_joint;
                BodyDataTypedef  gimble_body;
        };
        
        

