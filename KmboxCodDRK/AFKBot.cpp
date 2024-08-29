#define NOMINMAX
#include <algorithm>
#include "AFKBot.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <Windows.h>

AFKBot::AFKBot(Kmbox* kmbox)
    : kmbox(kmbox),
    distX(0, 1)
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
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    SetConsoleCursorPosition(hConsole, statusPos);
    std::cout << "                    ";

    SetConsoleCursorPosition(hConsole, { 0, 12 });
    std::cout << "AFK Bot stopped. Returning to the main menu... ";
}

void AFKBot::moveMouseRandomly() {
    POINT cursorPos;
    if (!GetCursorPos(&cursorPos)) {
        std::cerr << "Failed to get cursor position!\n";
        return;
    }

    int moveAmount = distX(rng);

    bool moveRight = rng() % 2 == 0;

    int newX = cursorPos.x + (moveRight ? moveAmount : -moveAmount);
    int newY = cursorPos.y;

    newX = std::max(0, std::min(newX, 1920));

    const int steps = 20;
    int stepX = (newX - cursorPos.x) / steps;

    for (int i = 0; i < steps; ++i) {
        int intermediateX = cursorPos.x + stepX * i;
        int intermediateY = cursorPos.y;

        std::string command = "km.move(" + std::to_string(intermediateX) + "," + std::to_string(intermediateY) + ")\r\n";
        kmbox->send_command(command);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::string command = "km.move(" + std::to_string(newX) + "," + std::to_string(newY) + ")\r\n";
    kmbox->send_command(command);

    std::cout << "Moved mouse to (" << newX << ", " << newY << ")\n";
}
