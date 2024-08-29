#ifndef KIMBOX_HPP
#define KIMBOX_HPP

#include <Windows.h>
#include <string>

class Kmbox {
public:
    Kmbox();
    ~Kmbox();

    bool initialize(const std::string& targetDescription, DWORD baudRate);
    void sendCommand(const std::string& command);
    void closePort();

private:
    HANDLE hSerial;
    std::string findPort(const std::string& targetDescription);
    bool openPort(const char* portName, DWORD baudRate);
};

#endif // KIMBOX_H
