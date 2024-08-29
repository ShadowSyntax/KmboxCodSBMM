#ifndef KBOX_INTERFACE_HPP
#define KBOX_INTERFACE_HPP

#include <Windows.h>
#include <string>
#include <iostream>

class Kmbox {
public:
    Kmbox();
    ~Kmbox();
    bool initialize(const std::string& description, DWORD baudRate);
    void close();
    bool isConnected() const;
    void send_command(const std::string& command); 

private:
    HANDLE hSerial;
    bool connected;
    std::string find_port(const std::string& targetDescription);
    bool open_port(const char* portName, DWORD baudRate);
};

#endif 
