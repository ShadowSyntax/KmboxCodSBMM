#include "Kmbox.hpp"
#include <iostream>
#include <setupapi.h>
#include <devguid.h>
#include <thread>

#pragma comment(lib, "setupapi.lib")

Kmbox::Kmbox() : hSerial(INVALID_HANDLE_VALUE) {}

Kmbox::~Kmbox() {
    closePort();
}

bool Kmbox::initialize(const std::string& targetDescription, DWORD baudRate) {
    std::string portName = findPort(targetDescription);
    if (portName.empty()) {
        std::cerr << "Failed to find port." << std::endl;
        return false;
    }

    return openPort(portName.c_str(), baudRate);
}

void Kmbox::sendCommand(const std::string& command) {
    if (hSerial != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        if (!WriteFile(hSerial, command.c_str(), command.length(), &bytesWritten, NULL)) {
            std::cerr << "Failed to write to serial port!" << std::endl;
        }
    }
    else {
        std::cerr << "Serial port is not open!" << std::endl;
    }
}

void Kmbox::closePort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

std::string Kmbox::findPort(const std::string& targetDescription) {
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

bool Kmbox::openPort(const char* portName, DWORD baudRate) {
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
