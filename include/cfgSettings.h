#ifndef CFGSETTING_H_
#define CFGSETTING_H_

#include "dbStruct.h"

/**
 *  @return 1 success 0 of incomplete 
 *  @param _con  - pointer to cfg_settings struct
 *  @param _path - path to cfg file
 */
extern int getSettings(struct cfg_settings *_con,const char *_path);
/**
 *  @param _con - pointer to cfg_settings struct
 */
extern void printSettings(struct cfg_settings *_con);
/**
 *  Write write FrameCounter from cfg_settings._lora.frameCounter 
 */
extern void writeFrameCounter(struct cfg_settings *_con,const char *_path);

#endif //CFGSETTING_H_