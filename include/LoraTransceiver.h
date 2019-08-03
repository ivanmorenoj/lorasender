#ifndef LORATRANSCEIVER_H_
#define LORATRANSCEIVER_H_

#include <stdint.h>
#include <iostream>


class LoraTransceiver
{
private:
    int _fd;
    int _baud;
    char _dev[40]; // /dev/ttyACM0

public:
    LoraTransceiver();
    LoraTransceiver(const char *_device,const int _bauRate);
    ~LoraTransceiver();
    void setConfigValues(const char *_device,const int _bauRate);
    uint8_t openSerial();
    void closeSerial();
    void sendPayload(const char *_payload);
    std::string read();
};


#endif //LORATRANSCEIVER_H_