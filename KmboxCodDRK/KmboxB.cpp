// Kmbox_interface.cpp

#include "Kmbox_interface.hpp"
#include <setupapi.h>
#include <devguid.h>
#include <iostream>
#include <thread>
#include <conio.h>

#pragma comment(lib, "setupapi.lib")

Kmbox::Kmbox() : hSerial(INVALID_HANDLE_VALUE), connected(false) {}

Kmbox::~Kmbox() {
    if (connected) {
        close();
    }
}

bool Kmbox::initialize(const std::string& description, DWORD baudRate) {
    std::string port = find_port(description);
    if (port.empty()) {
        std::cerr << "No port found for description: " << description << std::endl;
        return false;
    }

    if (!open_port(port.c_str(), baudRate)) {
        std::cerr << "Failed to open port: " << port << std::endl;
        return false;
    }

    connected = true;
    std::cout << "Connected to " << description << " on port " << port << std::endl;
    return true;
}

void Kmbox::close() {
    if (connected) {
        CloseHandle(hSerial);
        connected = false;
    }
}

bool Kmbox::isConnected() const {
    return connected;
}

std::string Kmbox::find_port(const std::string& targetDescription) {
    HDEVINFO hDevInfo = SetupDiGetClassDevsA(&GUID_DEVCLASS_PORTS, 0, 0, DIGCF_PRESENT);
    if (hDevInfo == INVALID_HANDLE_VALUE) return "";

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); ++i) {
        char buf[512];
        DWORD nSize = 0;

        if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)buf, sizeof(buf), &nSize) && nSize > 0) {
            buf[nSize] = '\0';
            std::string deviceDescription = buf;

            size_t comPos = deviceDescription.find("COM");
            size_t endPos = deviceDescription.find(")", comPos);

            if (comPos != std::string::npos && endPos != std::string::npos && deviceDescription.find(targetDescription) != std::string::npos) {
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return deviceDescription.substr(comPos, endPos - comPos);
            }
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return "";
}

bool Kmbox::open_port(const char* portName, DWORD baudRate) {
    hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) return false;

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 5;
    timeouts.ReadTotalTimeoutConstant = 5;
    timeouts.ReadTotalTimeoutMultiplier = 5;
    timeouts.WriteTotalTimeoutConstant = 5;
    timeouts.WriteTotalTimeoutMultiplier = 5;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting timeouts!" << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    return true;
}

void Kmbox::send_command(const std::string& command) {
    DWORD bytesWritten;
    if (!WriteFile(hSerial, command.c_str(), command.length(), &bytesWritten, NULL)) {
        std::cerr << "Failed to write to serial port!" << std::endl;
    }
}
