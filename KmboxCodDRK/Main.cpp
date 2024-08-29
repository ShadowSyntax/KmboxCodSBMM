#include <iostream>
#include <windows.h>
#include "AFKBot.h"
#include "Kmbox_interface.hpp"

void displayASCIIArt() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    WORD saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);

    std::cout << "   _____ _               _                _____             _             \n";
    std::cout << "  / ____| |             | |              / ____|           | |            \n";
    std::cout << " | (___ | |__   __ _  __| | _____      _| (___  _   _ _ __ | |_ __ ___   \n";
    std::cout << "  \\___ \\| '_ \\ / _` |/ _` |/ _ \\ \\ /\\ / /\\___ \\| | | | '_ \\| __/ _` \\ \\ / /\n";
    std::cout << "  ____) | | | | (_| | (_| | (_) \\ V  V / ____) | |_| | | | | || (_| |>  < \n";
    std::cout << " |_____/|_| |_|\\__,_|\\__,_|\\___/ \\_/\\_/ |_____/ \\__, |_| |_|\\__\\__,_/_/\\_\\ \n";
    std::cout << "                                                 __/ |                    \n";
    std::cout << "                                                |___/                     \n";

    SetConsoleTextAttribute(hConsole, saved_attributes);

    std::cout << "\n\n";
}

int main() {
    int choice;
    Kmbox kmbox; 

    do {
        displayASCIIArt();

        std::cout << "1. Cod AFK\n";
        std::cout << "2. Kmbox B+ connect\n";
        std::cout << "0. Exit\n";

        std::cout << "\nPick an option: ";
        std::cin >> choice;

        system("cls");

        displayASCIIArt();

        COORD pos = { 0, 8 }; 
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

        if (choice == 1) {
            if (!kmbox.isConnected()) {
                COORD statusPos = { 0, 10 };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), statusPos);
                system("cls");
                std::cout << "Kmbox is not connected. Please connect first.\n";
                continue;
            }
            AFKBot afkBot(&kmbox);
            afkBot.run();
        }
        else if (choice == 2) {
            if (kmbox.initialize("USB-SERIAL CH340", CBR_115200)) { 
                std::cout << "\nKmbox B+ connected successfully.\n";
            }
            else {
                std::cout << "\nFailed to connect to Kmbox B+.\n";
            }
        }
        else if (choice == 0) {
            std::cout << "\nExiting...\n";
        }
        else {
            std::cout << "\nInvalid choice. Please select a valid option.\n";
        }

        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get(); 
        }

        system("cls");

    } while (choice != 0);

    return 0;
}
