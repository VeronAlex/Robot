#pragma once
class Order_control {
public:

    static bool order_exist_bool;
    static bool glass_is_given_bool;

    bool order_exist();
    void send_message();
    bool glass_is_given();
    void order_end();
};