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

    // Send Command
    _ser.write((const unsigned char *)"[CC]\n",5);
    _ser.flush();
    sleep(2);
    
    if (_ser.read().find(std::string("OK")) != std::string::npos)
        return 0;

    return 1;
}

/* Command: [SP] send payload*/
uint8_t clientModel::sendSP(lora_payload *_loraPayload,gas_values *_gasValues) {
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
    int txPw = std::stoi(_loraCfg->txPower);
    memset(_buff,0,100);
    sprintf(_buff,"[TP]%X\n", (uint8_t)(txPw > 0x0F ? 0x0F : txPw));

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [TP]";
        return 1;
    }
    return 0;
}

/* Command: [AM] send Activation method */
uint8_t clientModel::sendAM() {
    memset(_buff,0,100);
    sprintf(_buff,"[AM]%s\n",_loraCfg->activationMethod.c_str());

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [AM]";
        return 1;
    }
    return 0;
}

/* Command: [DR] send Data Rate */
uint8_t clientModel::sendDR() {
    memset(_buff,0,100);
    sprintf(_buff,"[DR]%s\n",_loraCfg->dataRate.c_str());

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [DR]";
        return 1;
    }
    return 0;
}

/* Command: [CH] send Channel */
uint8_t clientModel::sendCH() {
    memset(_buff,0,100);
    sprintf(_buff,"[CH]%s\n",_loraCfg->channel.c_str());

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [CH]";
        return 1;
    }
    return 0;
}

/* Command: [NK] send Network key */
uint8_t clientModel::sendNK() {
    memset(_buff,0,100);
    sprintf(_buff,"[NK]%s\n",_loraCfg->NwkSKey.c_str());

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [NK]";
        return 1;
    }
    return 0;
}

/* Command: [AK] send App key */
uint8_t clientModel::sendAK() {
    memset(_buff,0,100);
    sprintf(_buff,"[AK]%s\n",_loraCfg->AppSKey.c_str());

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [AK]";
        return 1;
    }
    return 0;
}

/* Command: [DA] send Device Address */
uint8_t clientModel::sendDA() {
    memset(_buff,0,100);
    sprintf(_buff,"[DA]%s\n",_loraCfg->DevAddr.c_str());

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [DA]";
        return 1;
    }
    return 0;
}

/* Command: [FC] send Frame counter */
uint8_t clientModel::sendFC() {
    memset(_buff,0,100);
    sprintf(_buff,"[FC]%08X\n",_loraCfg->frameCounter);

    _ser.write((unsigned char *)_buff,strlen(_buff));
    _ser.flush();
    sleep(2);
    if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
        PLOG_ERROR << "No response when send [FC]";
        return 1;
    }
    return 0;
}
