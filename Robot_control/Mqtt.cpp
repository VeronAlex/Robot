#include <iostream>
#include "mqtt.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>
#include <errno.h>
#include "robot.h"
#define HOST "192.168.1.4"
#define NUM_MESSAGES 30
#define THREAD 10
#define KEEP_ALIVE 60
#define MQTT_PUB_TOPIC "abot/command/alex"
#define MQTT_QOS_LEVEL 2
#define MSG_MAX_SIZE 512

using namespace std;

Robot_mqtt::Robot_mqtt(int _port) {
    this->port = port;
}

void Robot_mqtt::sendtohost(const char data[MSG_MAX_SIZE]) {
    struct mosquitto *mosq = NULL;
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        cout << "Error: Out of memory." << "/n";
        return 1;
    }
    rv=mosquitto_connect(mosq, HOST, port, KEEP_ALIVE);
    if (rv) {
        cout<<"Connect server error:"<< strerror(errno)<<"/n";
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }
    mosquitto_publish(mosq, NULL, MQTT_PUB_TOPIC, strlen(data), data, 0, false);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    cout << "End!" << "/n";
}

void Robot_mqtt::left() {
    sendtohost("{ \"cmd\":\"left\", \"val\": 0.1, \"spd\": 0.3}");
}
void Robot_mqtt::right() {
    sendtohost("{ \"cmd\":\"right\", \"val\": 0.1, \"spd\": 0.3}");
}
void Robot_mqtt::forward() {
    sendtohost("{ \"cmd\":\"forward\", \"val\": 0.1, \"spd\": 0.5}");
}