#include <iostream>
#include "plog/Log.h"
#include "sqlConnector.h"
#include "dbStruct.h"
#include "cfgSettings.h"
#include "convertions.h"
#include "serial.h"

//#define INSTALL
#ifdef INSTALL
    #define CFG_PATH    "/etc/lorasender/config.cfg"
    #define LOG_PATH    "/var/log/lorasender/lorasender.log"
#else
    #define CFG_PATH    "mainConfig.cfg"
    #define LOG_PATH    "logger.log"
#endif 

using ::std::cout;
using ::std::endl;

int main(int argc, char const *argv[])
{
    /* Variables */
    uint64_t _latestId;
    char _device[30];
    uint8_t _status;

    /* structs */
    gas_values _gv;
    lora_payload _lp;

    /* objets */
    sqlConnector _sql;
    Serial _ser;

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


    for (uint8_t i = 0; i < 5; i++) {
        std::string tmpDev = "/dev/ttyACM" + std::to_string(i);
        memset(_device,0,30);
        strcpy(_device, tmpDev.c_str());

        /* open serial Port */
        _status = _ser.open(_device,115200); 

        if (_status) {
            PLOG_INFO << "Check communication on: " << _device;

            _ser.puts("[CC]");
            sleep(2);
            std::string tmp = _ser.read();
            
            if (tmp.find(std::string("OK")) != std::string::npos) {
                PLOG_INFO << "Match protocol in: " << _device;
                break;
            }
        }
    }

    /* check if the serial port is open */
    if (!_status) {
        cout << "Cannot connect to serial port\n";
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
