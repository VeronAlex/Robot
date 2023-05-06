#include <iostream>
#include "robot_control.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Robot_control::Robot_control(Definer *d, Robot *robot, Order_control *order, Drink_control *dr) {
    this->df = d;
    this->bot = robot;
    this->ord_control = order;
    this->dr_control = dr;
    this->state = EWait;
}

void Robot_control::start() {
    for(;;){
        this->df->robot_define();
        this->df->show_inf();
        cout << this->state << "\n";

        this->ControlBot();

        if (waitKey(5) >= 0)
            break;
    }
}
void Robot_control::ControlBot(){
    switch (this->state) {
    case EWait:
        if (ord_control->order_exist()) {
            this->state = EFillingGlass;
        }
        break;

    case EWaitCustomer:
        if (ord_control->glass_is_given()) {
            MoveToHome = true;
            this->state = EMove;
        }
        break;

    case EFillingGlass:
        if (GlassFull) {
            if (dr_control->drink_ready()){
                GlassFull= false;
                this->state = EMove;
            }
        }
        else {
            dr_control->open();
            GlassFull = true;
        }
        break;
    case EMove:
        if (MoveToHome = false) {
            if (df->get_angle_to_target() > 0)
                this->state = ERotateLeft;
            else
                this->state = ERotateRight;
            if (df->get_distance_to_target() > 100)
                this->state = EMoveForward;
            else
                this->state = ENotifyTheCustomer;
            break;
        }
        else
        if (df->get_angle_to_home() > 0)
            this->state = ERotateLeft;
        else
            this->state = ERotateRight;
        if (df->get_distance_to_home() > 100)
            this->state = EMoveForward;
        else {
            this->state = EWait;
            ord_control->order_end();
            MoveToHome = false;
        }
    case ERotateLeft:
        bot->left();
        this->state = EMove;
        break;
    case ERotateRight:
        bot->right();
        this->state = EMove;
        break;
    case EMoveForward:
        bot->forward();
        this->state = EMove;
        break;
    case ENotifyTheCustomer:
        ord_control->send_message();
        this->state = EWaitCustomer;
        break;
    }
}