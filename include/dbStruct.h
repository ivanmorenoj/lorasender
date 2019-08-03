#ifndef DBSTRUCT_H_
#define DBSTRUCT_H_

#include <stdint.h>

struct sql_cfg {
    char user[50]; 
    char pass[50];
    char host[80];
    char schema[30];
};

struct ambiental{
    float   _tem;
    float   _hum;
    float   _pre;
};

struct gas_values{
    uint64_t    _id;
    ambiental   amb;
    float       _co;    // in ppm 
    float       _o3;    // in ppm 
    float       _so2;   // in ppm 
    float       _no2;   // in ppm 
    float       _pm1;   // in ppm 
    float       _pm10;  // in ppm 
    float       _pm25;  // in ppm 
};

struct byte_payload{
    // ambiental variables
    int8_t      _tem;   // -127 - 127 in °C
    uint8_t     _hum;   // 0 - 100 in %
    uint16_t    _pre;   // 0 - 65535 in hPa

    // gas variables
    uint16_t    _co;    // 0 - 65535 in ppb 
    uint16_t    _o3;    // 0 - 65535 in ppb 
    uint16_t    _so2;   // 0 - 65535 in ppb 
    uint16_t    _no2;   // 0 - 65535 in ppb 
    uint16_t    _pm1;   // 0 - 65535 in ppb 
    uint16_t    _pm10;  // 0 - 65535 in ppb 
    uint16_t    _pm25;  // 0 - 65535 in ppb
};

union lora_payload{
    byte_payload _bytes;
    uint8_t _raw[19];
};

#endif