
#ifndef AFKBOT_H
#define AFKBOT_H

#include "Kmbox_interface.hpp"
#include <chrono>
#include <random>

class AFKBot {
public:
    AFKBot(Kmbox* kmbox);
    void run();

private:
    Kmbox* kmbox;
    std::mt19937 rng;
    std::uniform_int_distribution<int> distX;
    std::uniform_int_distribution<int> distY;

    void moveMouseRandomly(); 
};

#endif 
