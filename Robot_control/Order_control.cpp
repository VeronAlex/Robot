#include <iostream>
#include "order_control.h"

using namespace std;

bool Order_control::order_exist() {
    return order_exist_bool;
}

void Order_control::send_message() {
    cout << "Order is ready!" << "/n";
    glass_is_given_bool = false;
}

bool Order_control::glass_is_given() {
    return glass_is_given_bool;
}

void Order_control::order_end() {
    order_exist_bool = false;
}