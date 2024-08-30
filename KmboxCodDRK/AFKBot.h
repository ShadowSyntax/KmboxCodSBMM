#ifndef AFKBOT_H
#define AFKBOT_H

#include "Kmbox_interface.hpp"
#include <random>

class AFKBot {
public:
    AFKBot(Kmbox* kmbox);
    void run();

private:
    void moveMouseRandomly();
    void performRandomKeyPresses();
    void pressRightMouseButton();
    void releaseRightMouseButton();
    void pressLeftMouseButton();
    void releaseLeftMouseButton();

    Kmbox* kmbox;
    std::mt19937 rng;
    std::uniform_int_distribution<> distX;
    std::uniform_int_distribution<> distDelay;
    std::uniform_int_distribution<> distHold;
    std::uniform_int_distribution<> distLeftClickDuration; 
};

#endif 
