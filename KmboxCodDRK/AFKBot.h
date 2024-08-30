#ifndef AFKBOT_H
#define AFKBOT_H

#include "Kmbox_interface.hpp"
#include <random>
#include <string>

class AFKBot {
public:
    AFKBot(Kmbox* kmbox);
    void run();

private:
    void moveMouseRandomly();
    void performRandomKeyPresses();
    void pressRightMouseButton(); // Declare method to press right mouse button
    void releaseRightMouseButton(); // Declaration of the key press method

    Kmbox* kmbox;
    std::mt19937 rng;
    std::uniform_int_distribution<> distX;
    std::uniform_int_distribution<> distDelay;
    std::uniform_int_distribution<> distHold;
};

#endif // AFKBOT_H
