#include <iostream>
#include <libconfig.h++>
#include "plog/Log.h"
#include "cfgSettings.h"

int getSettings(struct cfg_settings *_con,const char *_path) {
    int success = 0;

    libconfig::Config cfg;
    
    try {
        cfg.readFile(_path);
    } catch (const libconfig::FileIOException &fioex) {
        PLOG_FATAL << "I/O error while reading file." << std::endl;
        exit(EXIT_FAILURE);
    } catch (const libconfig::ParseException &pex) {
        PLOG_FATAL << "Parse error at " << pex.getFile() << ":" << pex.getLine()
             << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
    }
    const libconfig::Setting &root = cfg.getRoot();
    try {
        /* lookup database config*/
        string tmp[4];
        const libconfig::Setting &_db = root["sqlDataBase"];
        _db.lookupValue("user",     tmp[0]);
        _db.lookupValue("password", tmp[1]);
        _db.lookupValue("host",     tmp[2]);
        _db.lookupValue("schema",   tmp[3]);
        /* copy values */
        strcpy(_con->_sql.user,  tmp[0].c_str());
        strcpy(_con->_sql.pass,  tmp[1].c_str());
        strcpy(_con->_sql.host,  tmp[2].c_str());
        strcpy(_con->_sql.schema,tmp[3].c_str());

        /* lookup LoRa settings */
        const libconfig::Setting &_LoRa = root["LoRa"];
        _LoRa.lookupValue("port"        ,_con->_lora.port);
        _LoRa.lookupValue("txPower"     ,_con->_lora.txPower);
        _LoRa.lookupValue("actMethod"   ,_con->_lora.activationMethod);
        _LoRa.lookupValue("dataRate"    ,_con->_lora.dataRate);
        _LoRa.lookupValue("channel"     ,_con->_lora.channel);
        _LoRa.lookupValue("NwkSKey"     ,_con->_lora.NwkSKey);
        _LoRa.lookupValue("AppSKey"     ,_con->_lora.AppSKey);
        _LoRa.lookupValue("DevAddr"     ,_con->_lora.DevAddr);
        _LoRa.lookupValue("frameCounter",_con->_lora.frameCounter);

        success = 1;
    } catch (const libconfig::SettingNotFoundException &nfex) {
        PLOG_FATAL << "Settings not found!";
        exit(EXIT_FAILURE);
    }catch (const libconfig::SettingTypeException &ste) {
        PLOG_FATAL << "Settings Type exception on " << ste.what();
        exit(EXIT_FAILURE);        
    }
    return success;
}
void printSettings(struct cfg_settings *_con) {
    std::cout << "\nData Base"
              << "\n\tDB user:\t" << string(_con->_sql.user)
              << "\n\tDB pass:\t" << string(_con->_sql.pass)
              << "\n\tDB host:\t" << string(_con->_sql.host)
              << "\n\tDB schema:\t" << string(_con->_sql.schema) << std::endl;

    std::cout << "\nLoRa"
              << "\n\tport:\t\t"      << _con->_lora.port
              << "\n\ttxPower:\t"   << _con->_lora.txPower
              << "\n\tactMethod:\t" << _con->_lora.activationMethod
              << "\n\tdataRate:\t"  << _con->_lora.dataRate
              << "\n\tchannel:\t"   << _con->_lora.channel
              << "\n\tNwkSKey:\t"   << _con->_lora.NwkSKey
              << "\n\tAppSKey:\t"   << _con->_lora.AppSKey
              << "\n\tDevAddr:\t"   << _con->_lora.DevAddr
              << "\n\tFramecounter:\t"   << _con->_lora.frameCounter << std::endl;

}

void writeFrameCounter(struct cfg_settings *_con,const char *_path) {
    libconfig::Config cfg;

    try {
        cfg.readFile(_path);

        libconfig::Setting &frameCounter = cfg.lookup("LoRa.frameCounter");
        frameCounter = (int)_con->_lora.frameCounter;
        cfg.writeFile(_path);

    } catch (const libconfig::FileIOException &fioex) {
        PLOG_ERROR << "I/O error while reading file." << std::endl;
    } catch (const libconfig::ParseException &pex) {
        PLOG_ERROR << "Parse error at " << pex.getFile() << ":" << pex.getLine()
             << " - " << pex.getError() << std::endl;
    }
}