#ifndef CLIENT_MODEL_H_
#define CLIENT_MODEL_H_

#include "dbStruct.h"
#include "serial.h"

extern Serial _ser;

/**
 *  All functions return 0 on success and non zero in otherwise 
 */

/* Chech comminication */
uint8_t clientSendCC();
/* Send Payload */
uint8_t clientSendSP(lora_payload *_loraPayload,gas_values *_gasvalues);
/* send Tx Power */
uint8_t clientSendTP(lora *_lora);
/* send Activation method */
uint8_t clientSendAM(lora *_lora);
/* send Data Rate */
uint8_t clientSendDR(lora *_lora);
/* send Channel */
uint8_t clientSendCH(lora *_lora);
/* send Network key */
uint8_t clientSendNK(lora *_lora);
/* send App key */
uint8_t clientSendAK(lora *_lora);
/* send Device Address */
uint8_t clientSendDA(lora *_lora);
/* send Frame counter */
uint8_t clientSendFC(lora *_lora);

#endif //CLIENT_MODEL_H_