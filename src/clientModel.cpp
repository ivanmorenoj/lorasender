#include <string.h>
#include <stdio.h>
#include "plog/Log.h"
#include "clientModel.h"

static void hex2str(lora_payload *_lp,char *str) {
    char * tmp = str;
    for (uint8_t i = 0; i < 18; i++) {
        sprintf(tmp,"%02X",_lp->_raw[i]);
        tmp += 2;
    }    
}

void makeLoRaPayload(struct gas_values *_gv, lora_payload *_lp) {
    memset(_lp,0,sizeof(*_lp));

    // ambiental variables
    _lp->_bytes._tem = (int8_t)_gv->amb._tem;
    _lp->_bytes._hum = (uint8_t)_gv->amb._hum;
    _lp->_bytes._pre = (uint16_t)(_gv->amb._pre / 100.0);

    // gas variables
    _lp->_bytes._co    = (uint16_t)(_gv->_co   * 1000.0);
    _lp->_bytes._o3    = (uint16_t)(_gv->_o3   * 1000.0);
    _lp->_bytes._so2   = (uint16_t)(_gv->_so2  * 1000.0);
    _lp->_bytes._no2   = (uint16_t)(_gv->_no2  * 1000.0);
    _lp->_bytes._pm1   = (uint16_t)(_gv->_pm1  * 100.0);
    _lp->_bytes._pm10  = (uint16_t)(_gv->_pm10 * 100.0);
    _lp->_bytes._pm25  = (uint16_t)(_gv->_pm25 * 100.0);
}

void preparePayload(lora_payload *_lp,char *_buff,unsigned int len) {
    memset(_buff,0,len);

    strcpy(_buff,"[SP]");

    hex2str(_lp,_buff + 4);

    strcat(_buff,"\n");
}

/* Command: [CC] Check Communication */
uint8_t clientSendCC() {

    // Send Command
    _ser.write((const unsigned char *)"[CC]\n",5);
    sleep(2);
    
    if (_ser.read().find(std::string("OK")) != std::string::npos)
        return 0;

    return 1;
}

/* Command: [SP] send payload*/
uint8_t clientSendSP(lora_payload *_loraPayload,gas_values *_gasValues) {

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
uint8_t clientSendTP(lora *_lora) {

    return 0;
}

/* Command: [AM] send Activation method */
uint8_t clientSendAM(lora *_lora) {

    return 0;
}

/* Command: [DR] send Data Rate */
uint8_t clientSendDR(lora *_lora) {

    return 0;
}

/* Command: [CH] send Channel */
uint8_t clientSendCH(lora *_lora) {

    return 0;
}

/* Command: [NK] send Network key */
uint8_t clientSendNK(lora *_lora) {

    return 0;
}

/* Command: [AK] send App key */
uint8_t clientSendAK(lora *_lora) {

    return 0;
}

/* Command: [DA] send Device Address */
uint8_t clientSendDA(lora *_lora) {

    return 0;
}

/* Command: [FC] send Frame counter */
uint8_t clientSendFC(lora *_lora) {

    return 0;
}
