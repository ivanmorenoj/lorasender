#include <iostream>
#include <wiringSerial.h>
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
    #define LOG_PATH    "logger.log"
#endif 

using ::std::cout;
using ::std::endl;

int main(int argc, char const *argv[])
{
    /* Variables */
    uint64_t _latestId;
    char _device[20];
    int _serialFd;

    /* structs */
    gas_values _gv;
    lora_payload _lp;

    /* objets */
    sqlConnector _sql;

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

    /* Open Serial port */
    _serialFd = serialOpen("/dev/ttyACM0",115200);

    if (_serialFd < 0) {
        cout << "cannot open serial port";
        return EXIT_FAILURE;
    } else {
        cout << "serial port open";
    }

    /* check protocol */
    serialPuts(_serialFd,"[CC]");
    sleep(2);

    if (serialDataAvail(_serialFd)) {
        printf("Rcv: ");
        while (serialDataAvail()) {
            putchar((char)serialGetchar(_serialFd));
        }
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
