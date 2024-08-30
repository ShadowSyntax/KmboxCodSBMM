#define NOMINMAX
#include "AFKBot.h"
#include "Kmbox_interface.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include <cstdio>  

AFKBot::AFKBot(Kmbox* kmbox)
    : kmbox(kmbox),
    distX(300, 1500),
    distDelay(1, 2),
    distHold(2, 6),  
    distLeftClickDuration(500, 1000) 
{
    std::random_device rd;
    rng.seed(rd());
}

void AFKBot::run() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD statusPos = { 0, 10 };

    SetConsoleCursorPosition(hConsole, statusPos);
    std::cout << "Running AFK press Esc to stop...";

    bool running = true;
    while (running) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {
                running = false;
                break;
            }
        }
        moveMouseRandomly();
        performRandomKeyPresses();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    SetConsoleCursorPosition(hConsole, statusPos);
    std::cout << "                    ";

    system("cls");
    SetConsoleCursorPosition(hConsole, { 0, 12 });
    std::cout << "AFK Bot stopped. Returning to the main menu... ";
}

void AFKBot::moveMouseRandomly() {
    int moveAmount = distX(rng);

    bool moveRight = rng() % 2 == 0;

    if (!moveRight) {
        moveAmount = -moveAmount;
    }

    const int steps = 20;
    int stepAmount = moveAmount / steps;
    int delayMs = 10;

    for (int i = 0; i < steps; ++i) {
        std::string command = "km.move(" + std::to_string(stepAmount) + ",0)\r\n";
        kmbox->send_command(command);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    if (rng() % 2 == 0) {
        pressRightMouseButton();
        std::this_thread::sleep_for(std::chrono::seconds(distHold(rng))); 
        releaseRightMouseButton();
    }

    std::this_thread::sleep_for(std::chrono::seconds(distDelay(rng)));

    std::cout << "Moved mouse by " << moveAmount << " units to the " << (moveRight ? "right" : "left") << "\n";
}

void AFKBot::performRandomKeyPresses() {
    const char keys[] = { 'W', 'A', 'S', 'D' };
    char key = keys[rng() % (sizeof(keys) / sizeof(keys[0]))];

    std::string command = "km.press('" + std::string(1, key) + "')\r\n";
    kmbox->send_command(command);

    int delay = distDelay(rng) * 1000; 
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    std::cout << "Pressed key: " << key << "\n";
}

void AFKBot::pressRightMouseButton() {
    std::string command = "km.right(" + std::to_string(1) + ")\r\n"; 
    kmbox->send_command(command);
    std::cout << "Right mouse button pressed.\n";

    if (rng() % 2 == 0) {
        pressLeftMouseButton();
        std::this_thread::sleep_for(std::chrono::milliseconds(distLeftClickDuration(rng))); 
        releaseLeftMouseButton();
    }
}

void AFKBot::releaseRightMouseButton() {
    std::string command = "km.right(" + std::to_string(0) + ")\r\n"; 
    kmbox->send_command(command);
    std::cout << "Right mouse button released.\n";
}

void AFKBot::pressLeftMouseButton() {
    std::string command = "km.left(" + std::to_string(1) + ")\r\n";
    kmbox->send_command(command);
    std::cout << "Left mouse button pressed.\n";
}

void AFKBot::releaseLeftMouseButton() {
    std::string command = "km.left(" + std::to_string(0) + ")\r\n"; 
    kmbox->send_command(command);
    std::cout << "Left mouse button released.\n";
}
