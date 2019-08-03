#ifndef CFGSETTING_H_
#define CFGSETTING_H_

#include "dbStruct.h"

/**
 *  @return 1 success 0 of incomplete 
 *  @param _con  - pointer to sql_cfg struct
 *  @param _path - path to cfg file
 */
extern int getSettings(struct sql_cfg *_con,const char *_path);
/**
 *  @param _con - pointer to sql_cfg struct
 */
extern void printSettings(struct sql_cfg *_con);

#endif //CFGSETTING_H_