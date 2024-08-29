// AFKBot.cpp

#include "AFKBot.h"
#include <iostream>
#include <windows.h>  // Include the Windows API header for console manipulation

void AFKBot::run() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD saved_attributes = consoleInfo.wAttributes;

    // Set text color to green (or any color you like)
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    // Main loop
    bool running = true;
    while (running) {
        // Set cursor position for dynamic content
        COORD pos = { 0, 8 }; // Same line number used in main.cpp to keep content below the ASCII art
        SetConsoleCursorPosition(hConsole, pos);

        // Display a message indicating that "Cod AFK" is running
        std::cout << "Cod AFK is now running press Esc to stop...\n";
        // Check if the 'Esc' key is pressed
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = false;  // Exit the loop if 'Esc' is pressed
        }

        Sleep(100);  
    }

    // Restore the original console color
    SetConsoleTextAttribute(hConsole, saved_attributes);
}
