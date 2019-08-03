#include <iostream>
#include "plog/Log.h"
#include "sqlConnector.h"
#include "dbStruct.h"
#include "cfgSettings.h"
#include "convertions.h"
#include "LoraTransceiver.h"

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
    /* Variables */
    uint64_t _latestId;
    char _device[20];

    /* structs */
    gas_values _gv;
    lora_payload _lp;

    /* objets */
    sqlConnector _sql;
    LoraTransceiver _lora;

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

    /* Connect to usb stick */
    uint8_t _status;
    for (uint8_t _devNumber = 0; _devNumber < 5; _devNumber++) {
        std::string dev = "/dev/ttyACM" + std::to_string(_devNumber);
        strcpy(_device,dev.c_str());

        _lora.setConfigValues(_device,115200);
        _status = _lora.openSerial();
        
        if (_status){
            PLOG_INFO << "Check communication to: " << _device;
            _lora.sendPayload("[CC]");
            sleep(2);
        
            std::string readStr = _lora.read();
            if (readStr.find(std::string("OK")) != std::string::npos) {
                PLOG_INFO << "Match protocol with: " << _device;
                //cout << "connected";
                break;
            }
            _lora.closeSerial();
        }
        sleep(1);
    }

    if (!_status) {
        PLOG_ERROR << "Can't open serial port";
        return EXIT_FAILURE;
    }

    /* configure db */
    _sql.setUser(_sqlCfg.user);
    _sql.setPassword(_sqlCfg.pass);
    _sql.setHost(_sqlCfg.host);
    _sql.setSchema(_sqlCfg.schema);
    
    /* connect to db*/
    _sql.connect();

    /*for (;;) {

    }*/
    
    return EXIT_SUCCESS;
}
