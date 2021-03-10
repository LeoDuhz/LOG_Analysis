#ifndef DEMO_H
#define DEMO_H
#include "zsplugin.hpp"
class A : public ZSPlugin {
public:
    A(): ZSPlugin("A"){
        declare_publish("mes");
    }
    void run() {
        int count = 0;
        while(true) {
            auto &&str = std::to_string(count++);
            publish("mes",str.c_str(),str.size());
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
private:
};

class B : public ZSPlugin {
public:
    B(): ZSPlugin("B"){
        declare_receive("mes");
    }
    void run() {
        std::string str;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        ZSData data;
        std::cout<<"run"<<std::endl;
        while(true) {
            receive("mes",data);
            std::cout << std::string((const char *)data.data(),data.size()) << std::endl;
        }
    }
};

#endif // DEMO_H
