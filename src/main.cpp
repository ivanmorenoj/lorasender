#include <iostream>
#include "plog/Log.h"
#include "sqlConnector.h"
#include "dbStruct.h"
#include "cfgSettings.h"
#include "convertions.h"

//#define INSTALL
#ifdef INSTALL
    #define CFG_PATH    "/etc/lorasender/config.cfg"
    #define LOG_PATH    "/var/log/lorasender/lorasender.log"
#else
    #define CFG_PATH    "mainConfig.cfg"
    #define LOG_PATH    "lorasender.log"
#endif 

using ::std::cout;
using ::std::endl;

int main(int argc, char const *argv[])
{
    plog::init(plog::debug, LOG_PATH, 1000000, 3); // Initialize the logger. 1MB
    PLOG_INFO << ">>>>>>>>>>>>>>>Init Program<<<<<<<<<<<<<<<";

    /* General Config */
    struct sql_cfg _sqlCfg = {0};
    if (getSettings(&_sqlCfg,CFG_PATH)){
        PLOG_INFO << "Load config from: " << CFG_PATH;
        //printSettings(&_sqlCfg);
    }
    else {
        PLOG_ERROR << "Culdn't get config";
        return EXIT_FAILURE;
    }

    /* sql objet */
    sqlConnector _sql;

    /* configure db */
    _sql.setUser(_sqlCfg.user);
    _sql.setPassword(_sqlCfg.pass);
    _sql.setHost(_sqlCfg.host);
    _sql.setSchema(_sqlCfg.schema);
    
    /* connect to db*/
    _sql.connect();

    /* structs */
    gas_values _gv;
    lora_payload _lp;

    cout << "Latest id: " << _sql.getLatestId() << endl;

    _sql.fetch_gas_values(&_gv);
    _gv.amb._tem = -10;

    cout << "ID:  \t" << _gv._id << endl
         << "TEMP:  \t" << _gv.amb._tem << endl
         << "PRE:  \t" << _gv.amb._pre << endl
         << "HUM:  \t" << _gv.amb._hum << endl
         << "CO:  \t" << _gv._co * 1000 << endl
         << "O3:  \t" << _gv._o3 * 1000 << endl
         << "SO2:  \t" << _gv._so2 * 1000 << endl
         << "NO2:  \t" << _gv._no2 * 1000 << endl
         << "PM1:  \t" << _gv._pm1 * 1000 << endl
         << "PM10:  \t" << _gv._pm10 * 1000 << endl
         << "PM25:  \t" << _gv._pm25 * 1000 << endl;

    makeLoRaPayload(&_gv, &_lp);

    cout << "LoRa RAW Payload: \n";
    for (uint8_t i = 0; i < sizeof(_lp); ++i) {
        printf("0x%02X ",_lp._raw[i]);
    }
    cout << endl;

    printf("LoRa bytes Payload: \n");
    printf("0x%02X ",(uint8_t)_lp._bytes._tem);
    printf("0x%02X ",(uint8_t)_lp._bytes._hum);
    printf("0x%02X ",(uint8_t)(_lp._bytes._pre & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._pre >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._co & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._co >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._o3 & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._o3 >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._so2 & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._so2 >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._no2 & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._no2 >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._pm1 & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._pm1 >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._pm10 & 0xFF));
    printf("0x%02X ",(uint8_t)((_lp._bytes._pm10 >> 8) & 0xFF));
    printf("0x%02X ",(uint8_t)(_lp._bytes._pm25 & 0xFF));
    printf("0x%02X \n",(uint8_t)((_lp._bytes._pm25 >> 8) & 0xFF));

    
    return EXIT_SUCCESS;
}
