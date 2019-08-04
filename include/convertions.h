#ifndef CONVERTIONS_H_
#define CONVERTIONS_H_

#include "dbStruct.h"

void makeLoRaPayload(struct gas_values *_gv, lora_payload *_lp);
void preparePayload(lora_payload *_lp,char *_buff,unsigned int len);

#endif // CONVERTIONS_H_