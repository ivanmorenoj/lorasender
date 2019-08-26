/**
 *  Author: Ivan Moreno <ivan1697@gmail.com>
 *      August 2019
 */
#include "plog/Log.h"
#include "sqlConnector.h"
#include "dbStruct.h"
#include "cfgSettings.h"
#include "clientModel.h"

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

    /* structs */
    cfg_settings _mainCfg;
    gas_values _gv;
    lora_payload _lp;

    /* objets */
    sqlConnector _sql;
    clientModel _client;

    plog::init(plog::debug, LOG_PATH, 1000000, 3); // Initialize the logger. 1MB
    PLOG_INFO << ">>>>>>>>>>>>>>>Init Program<<<<<<<<<<<<<<<";

    /* Pring log path and cfg path */
    printf("Logs saved in: %s\nConfig file loaded from: %s\n",LOG_PATH,CFG_PATH);

    /* General Config */
    if (getSettings(&_mainCfg,CFG_PATH)){
        PLOG_INFO << "Load config from: " << CFG_PATH;
        //printSettings(&_mainCfg);
    }
    else {
        PLOG_FATAL << "Couldn't get config";
        return EXIT_FAILURE;
    }

    /* Connect with lora stick */
    _client.setConfig(&_mainCfg._lora);

    /*Open serial port loaded from cfg file */
    if (!_client.openSerial()) {
        PLOG_FATAL << "Cannot connect to serial port";
        return EXIT_FAILURE;
    }
    
    /*Send config Values to usb stick */
    if ( _client.sendTP() || _client.sendAM() || 
         _client.sendDR() || _client.sendCH() || 
         _client.sendNK() || _client.sendAK() || 
         _client.sendDA() || _client.sendFC() ) {
        std::cout << "Problem when sending config to usb stick\n"; 
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

            _client.sendSP(&_lp,&_gv);
        }
        sleep(30);
    }
    
    return EXIT_SUCCESS;
}
