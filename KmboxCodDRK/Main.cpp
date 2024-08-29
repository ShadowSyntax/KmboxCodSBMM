#include <iostream>
#include <windows.h>
#include "AFKBot.h"
#include "Kmbox_interface.hpp"

// Function to display ASCII art
void displayASCIIArt() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    // Save the current console attributes
    WORD saved_attributes = consoleInfo.wAttributes;

    // Set text color to purple (combination of red and blue)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);

    // Display ASCII art in purple
    std::cout << "   _____ _               _                _____             _             \n";
    std::cout << "  / ____| |             | |              / ____|           | |            \n";
    std::cout << " | (___ | |__   __ _  __| | _____      _| (___  _   _ _ __ | |_ __ ___   \n";
    std::cout << "  \\___ \\| '_ \\ / _` |/ _` |/ _ \\ \\ /\\ / /\\___ \\| | | | '_ \\| __/ _` \\ \\ / /\n";
    std::cout << "  ____) | | | | (_| | (_| | (_) \\ V  V / ____) | |_| | | | | || (_| |>  < \n";
    std::cout << " |_____/|_| |_|\\__,_|\\__,_|\\___/ \\_/\\_/ |_____/ \\__, |_| |_|\\__\\__,_/_/\\_\\ \n";
    std::cout << "                                                 __/ |                    \n";
    std::cout << "                                                |___/                     \n";

    // Restore the original console color
    SetConsoleTextAttribute(hConsole, saved_attributes);

    // Move cursor to a new line for dynamic content
    std::cout << "\n\n";
}

int main() {
    int choice;
    Kmbox kmbox; // Create a single Kmbox instance to be used throughout the program

    do {
        // Display the ASCII art
        displayASCIIArt();

        // Display the options
        std::cout << "1. Cod AFK\n";
        std::cout << "2. Kmbox B+ connect\n";
        std::cout << "0. Exit\n"; // Option to exit the program

        // Prompt user to pick an option
        std::cout << "\nPick an option: ";
        std::cin >> choice;

        // Clear the console after user input
        system("cls");

        // Redisplay ASCII art
        displayASCIIArt();

        // Move cursor to a position where dynamic content will be displayed
        COORD pos = { 0, 8 }; // Change this value to a line number that ensures dynamic content is below the ASCII art
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

        // Check the user's choice
        if (choice == 1) {
            // Create an instance of AFKBot
            AFKBot afkBot;
            // Run the AFKBot output
            afkBot.run();
        }
        else if (choice == 2) {
            if (kmbox.initialize("USB-SERIAL CH340", CBR_115200)) { // Ensure the target description is correct
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
            std::cin.ignore();  // Clear the input buffer
            std::cin.get();     // Wait for Enter key press
        }

        // Clear the console screen for the next menu display
        system("cls");

    } while (choice != 0);

    return 0;
}
