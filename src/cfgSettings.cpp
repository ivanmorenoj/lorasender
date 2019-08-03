#include <iostream>
#include <string.h>
#include <libconfig.h++>
#include "plog/Log.h"
#include "cfgSettings.h"

int getSettings(struct sql_cfg *_con,const char *_path) {
    int success = 0;

    libconfig::Config cfg;
    
    try {
        cfg.readFile(_path);
    } catch (const libconfig::FileIOException &fioex) {
        PLOG_ERROR << "I/O error while reading file." << std::endl;
        exit(EXIT_FAILURE);
    } catch (const libconfig::ParseException &pex) {
        PLOG_ERROR << "Parse error at " << pex.getFile() << ":" << pex.getLine()
             << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
    }
    const libconfig::Setting &root = cfg.getRoot();
    try {
        /* lookup database config*/
        std::string tmp[4];
        const libconfig::Setting &_db = root["sqlDataBase"];
        _db.lookupValue("user",     tmp[0]);
        _db.lookupValue("password", tmp[1]);
        _db.lookupValue("host",     tmp[2]);
        _db.lookupValue("schema",   tmp[3]);
        /* copy values */
        strcpy(_con->user,  tmp[0].c_str());
        strcpy(_con->pass,  tmp[1].c_str());
        strcpy(_con->host,  tmp[2].c_str());
        strcpy(_con->schema,tmp[3].c_str());

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
void printSettings(struct sql_cfg *_con) {
    std::cout << "\nData Base"
              << "\n\tDB user:\t" << std::string(_con->user)
              << "\n\tDB pass:\t" << std::string(_con->pass)
              << "\n\tDB host:\t" << std::string(_con->host)
              << "\n\tDB schema:\t" << std::string(_con->schema) << std::endl;
}