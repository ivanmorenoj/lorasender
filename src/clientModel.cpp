#include <string.h>
#include <stdio.h>
#include "plog/Log.h"
#include "clientModel.h"
#include "utilities.h"

clientModel::clientModel() {
    _loraCfg = NULL;
    _status = 0;
}

clientModel::clientModel(lora *_lora_cfg) {
    this->_loraCfg = _lora_cfg;
}

clientModel::~clientModel() {
    if (_status)
        _ser.close();
}

void clientModel::setConfig(lora *_lora_cfg) {
    this->_loraCfg = _lora_cfg;
}

Serial *clientModel::getSerial() {
    return &_ser;
}

uint8_t clientModel::openSerial() {
    char _buff[50];
    memset(_buff,0,50);

    strcpy(_buff, _loraCfg->port.c_str());
    _status = _ser.open(_buff,115200); 
    if (_status) {
        PLOG_INFO << "Check communication on: " << _buff;
        if (sendCC()) {
            _ser.close();
            _status = 0;
            PLOG_ERROR << "Doesn't match protocol in " << _buff;
        }
    }
    return _status;
}

void clientModel::closeSerial() {
    _ser.close();
}

uint8_t clientModel::getStatus() {
    return _status;
}

/* Command: [CC] Check Communication */
uint8_t clientModel::sendCC() {

    // Send Command
    _ser.write((const unsigned char *)"[CC]\n",5);
    sleep(2);
    
    if (_ser.read().find(std::string("OK")) != std::string::npos)
        return 0;

    return 1;
}

/* Command: [SP] send payload*/
uint8_t clientModel::sendSP(lora_payload *_loraPayload,gas_values *_gasValues) {

    char _buff[100];
    memset(_buff,0,100);

    makeLoRaPayload(_gasValues,_loraPayload);
    preparePayload(_loraPayload,_buff,100);

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    
    sleep(3);
    
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [SP]";
        return 1;
    }

    return 0;
}

/* Command: [TP] send Tx Power */
uint8_t clientModel::sendTP() {

    return 0;
}

/* Command: [AM] send Activation method */
uint8_t clientModel::sendAM() {

    return 0;
}

/* Command: [DR] send Data Rate */
uint8_t clientModel::sendDR() {

    return 0;
}

/* Command: [CH] send Channel */
uint8_t clientModel::sendCH() {

    return 0;
}

/* Command: [NK] send Network key */
uint8_t clientModel::sendNK() {

    return 0;
}

/* Command: [AK] send App key */
uint8_t clientModel::sendAK() {

    return 0;
}

/* Command: [DA] send Device Address */
uint8_t clientModel::sendDA() {

    return 0;
}

/* Command: [FC] send Frame counter */
uint8_t clientModel::sendFC() {

    return 0;
}
