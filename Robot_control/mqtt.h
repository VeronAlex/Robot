#pragma once
#include "robot.h"
#define MSG_MAX_SIZE 512

class Robot_mqtt :public Robot {

    int port;
    int rv;

    void sendtohost(const char data[MSG_MAX_SIZE]);
public:
    Robot_mqtt(int _port);
    void left();
    void right();
    void forward();
};