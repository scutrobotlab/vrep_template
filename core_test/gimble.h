#pragma once
        enum 
        {
                yaw = 0,
                pitch,
                roll
        };
        typedef struct GimbleJoint
        {
                /*---command---*/
                float pos_d;
                float vel_d;
                float acc_d;
                float tau_d;

                 /*---feedback---*/
                float pos_fb;
                float vel_fb;
                float acc_fb;
                float tau_fb;
        }GimbleJointDataTypedef;

        typedef struct GimbleBody
        {
                /*---status---*/
                float pitch;
                float yaw;
                float roll;
        }GimbleBodyDataTypedef;

        class Gimble
        {
        private:
                /* data */
        public:
                Gimble(/* args */);
                ~Gimble();
                void init();
                void task();
                GimbleJointDataTypedef gimble_joint;
                GimbleBodyDataTypedef  gimble_body;
        };
        
        

