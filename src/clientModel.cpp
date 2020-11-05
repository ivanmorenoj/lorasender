#include <string.h>
#include <stdio.h>
#include "plog/Log.h"
#include "clientModel.h"
#include "utilities.h"

clientModel::clientModel() {
    _loraCfg = NULL;
    _status = 0;
    memset(_buff,0,100);
}

clientModel::clientModel(lora *_lora_cfg) {
    this->_loraCfg = _lora_cfg;
    _status = 0;
    memset(_buff,0,100);
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
    memset(_buff,0,100);

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
    memset(_buff,0,100);

    sprintf(_buff,"[CC]\n");

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [SP] send payload*/
uint8_t clientModel::sendSP(lora_payload *_loraPayload,gas_values *_gasValues) {
    memset(_buff,0,100);

    makeLoRaPayload(_gasValues,_loraPayload);
    strcpy(_buff,"[SP]");
    hex2str(_loraPayload,_buff + 4);
    strcat(_buff,"\n");

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [TP] send Tx Power */
uint8_t clientModel::sendTP() {
    int txPw = std::stoi(_loraCfg->txPower);
    memset(_buff,0,100);
    sprintf(_buff,"[TP]%X\n", (uint8_t)(txPw & 0x0F));

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [AM] send Activation method */
uint8_t clientModel::sendAM() {
    memset(_buff,0,100);
    sprintf(_buff,"[AM]%s\n",_loraCfg->activationMethod.c_str());

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [DR] send Data Rate */
uint8_t clientModel::sendDR() {
    memset(_buff,0,100);
    sprintf(_buff,"[DR]%s\n",_loraCfg->dataRate.c_str());

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [CH] send Channel */
uint8_t clientModel::sendCH() {
    memset(_buff,0,100);
    sprintf(_buff,"[CH]%s\n",_loraCfg->channel.c_str());

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [NK] send Network key */
uint8_t clientModel::sendNK() {
    memset(_buff,0,100);
    sprintf(_buff,"[NK]%s\n",_loraCfg->NwkSKey.c_str());

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [AK] send App key */
uint8_t clientModel::sendAK() {
    memset(_buff,0,100);
    sprintf(_buff,"[AK]%s\n",_loraCfg->AppSKey.c_str());

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [DA] send Device Address */
uint8_t clientModel::sendDA() {
    memset(_buff,0,100);
    sprintf(_buff,"[DA]%s\n",_loraCfg->DevAddr.c_str());

    return sendAndWaitOK(strlen(_buff));
}

/* Command: [FC] send Frame counter */
uint8_t clientModel::sendFC() {
    memset(_buff,0,100);
    sprintf(_buff,"[FC]%08X\n",_loraCfg->frameCounter);
    return sendAndWaitOK(strlen(_buff));
}

uint8_t clientModel::sendAndWaitOK(uint8_t _len) {
    /* DEBUG */
    //std::cout << _buff << std::endl;
    
    _ser.write((unsigned char *)_buff,_len);
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response found";
        exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}