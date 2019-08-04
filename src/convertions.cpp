#include "convertions.h"
#include <string.h>
#include <stdio.h>

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