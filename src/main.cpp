/**
 *  Author: Ivan Moreno <ivan1697@gmail.com>
 *      August 2019
 */
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

int main(int argc, char const *argv[])
{
    /* Variables */
    uint64_t _latestId = 0;
    char _buff[100];
    uint8_t _status;

    /* structs */
    gas_values _gv;
    lora_payload _lp;

    /* objets */
    sqlConnector _sql;
    Serial _ser;

    plog::init(plog::debug, LOG_PATH, 1000000, 3); // Initialize the logger. 1MB
    PLOG_INFO << ">>>>>>>>>>>>>>>Init Program<<<<<<<<<<<<<<<";

    /* Pring log path and cfg path */
    printf("Logs saved in: %s\nConfig file loaded from: %s\n",LOG_PATH,CFG_PATH);

    /* General Config */
    cfg_settings _mainCfg;
    if (getSettings(&_mainCfg,CFG_PATH)){
        PLOG_INFO << "Load config from: " << CFG_PATH;
        //printSettings(&_mainCfg);
    }
    else {
        PLOG_FATAL << "Couldn't get config";
        return EXIT_FAILURE;
    }

    for (uint8_t i = 0; i < 5; i++) {
        std::string tmpDev = "/dev/ttyACM" + std::to_string(i);
        memset(_buff,0,100);
        strcpy(_buff, tmpDev.c_str());

        /* open serial Port */
        _status = _ser.open(_buff,115200); 

        if (_status) {
            PLOG_INFO << "Check communication on: " << _buff;

            _ser.write((const unsigned char *)"[CC]\n",5);
            sleep(2);
            
            if (_ser.read().find(std::string("OK")) != std::string::npos) {
                PLOG_INFO << "Match protocol in: " << _buff;
                break;
            }
            _ser.close();
        }
    }

    /* check if the serial port is open */
    if (!_status) {
        PLOG_FATAL << "Cannot connect to serial port\n";
        return EXIT_FAILURE;
    }

    /* configure db */
    _sql.setUser(_mainCfg._sql.user);
    _sql.setPassword(_mainCfg._sql.pass);
    _sql.setHost(_mainCfg._sql.host);
    _sql.setSchema(_mainCfg._sql.schema);
    
    /* connect to db*/
    _sql.connect();
    
    for (;;) {
        if (_sql.getLatestId() > _latestId){
            _sql.fetch_gas_values(&_gv);
            _latestId = _gv._id;

            makeLoRaPayload(&_gv,&_lp);

            preparePayload(&_lp,_buff,100);

            _ser.write((unsigned char *)_buff,strlen(_buff));
            _ser.flush();
            sleep(3);

            if (!(_ser.read().find(std::string("OK")) != std::string::npos)) {
                PLOG_ERROR << "No response when send [SP]";
            }
        }
        sleep(30);
    }
    
    return EXIT_SUCCESS;
}
