#include <wiringSerial.h>
#include <string.h>
#include <iostream>

#include "plog/Log.h"
#include "LoraTransceiver.h"

LoraTransceiver::LoraTransceiver() {
    _fd = -1;
    for (uint8_t i; i < sizeof(_dev); i++)
        _dev[i] = 0;
}

LoraTransceiver::LoraTransceiver(const char *_device,const int _bauRate) {
    setConfigValues(_device,_bauRate);
}

LoraTransceiver::~LoraTransceiver() {
    if (_fd > 0) {
        serialClose(_fd);
    }
}

void LoraTransceiver::setConfigValues(const char *_device,const int _bauRate) {
    for (uint8_t i = 0 ; i < strlen(_device); i++)
        _dev[i] = _device[i];
    _baud = _bauRate;
}

uint8_t LoraTransceiver::openSerial() {
    _fd = serialOpen(_dev,_baud);
    if (_fd < 0) {
        return 0;
    }
    return 1;
}

void LoraTransceiver::closeSerial() {
    serialClose(_fd);
}

void LoraTransceiver::sendPayload(const char *_payload){
    serialPuts(_fd,_payload);
    serialFlush(_fd);
}

std::string LoraTransceiver::read() {
    std::string _readBuff = "";
    while(serialDataAvail(_fd) > 0)
        _readBuff += (char)serialGetchar(_fd);
    return _readBuff;
}    

