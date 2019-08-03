#include <iostream>
#include "plog/Log.h"
#include "sqlConnector.h"
#include "dbStruct.h"
#include "cfgSettings.h"

//#define INSTALL
#ifdef INSTALL
    #define CFG_PATH    "/etc/lorasender/config.cfg"
    #define LOG_PATH    "/var/log/lorasender/sqlAVG.log"
#else
    #define CFG_PATH    "mainConfig.cfg"
    #define LOG_PATH    "sqlAVG.log"
#endif 

using ::std::cout;
using ::std::endl;

int main(int argc, char const *argv[])
{
    plog::init(plog::debug, LOG_PATH, 1000000, 3); // Initialize the logger. 1MB
    PLOG_INFO << "init program";

    /* General Config */
    struct sql_cfg _sqlCfg = {0};
    if (getSettings(&_sqlCfg,CFG_PATH)){
        PLOG_INFO << "Get config";
        printSettings(&_sqlCfg);
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

    /* db variables */
    gas_values _tmp_gv;

    cout << "Latest id: " << _sql.getLatestId();

    _tmp_gv = _sql.fetch_gas_values();

    cout << "ID:  \t" << _tmp_gv._id << endl
         << "TEMP:  \t" << _tmp_gv.amb._tem << endl
         << "PRE:  \t" << _tmp_gv.amb._pre << endl
         << "HUM:  \t" << _tmp_gv.amb._hum << endl
         << "CO:  \t" << _tmp_gv._co * 1000 << endl
         << "O3:  \t" << _tmp_gv._o3 * 1000 << endl
         << "SO2:  \t" << _tmp_gv._so2 * 1000 << endl
         << "NO2:  \t" << _tmp_gv._no2 * 1000 << endl
         << "PM1:  \t" << _tmp_gv._pm1 * 1000 << endl
         << "PM10:  \t" << _tmp_gv._pm10 * 1000 << endl
         << "PM25:  \t" << _tmp_gv._pm25 * 1000 << endl;

    return EXIT_SUCCESS;
}
