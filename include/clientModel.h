#ifndef CLIENT_MODEL_H_
#define CLIENT_MODEL_H_

#include "dbStruct.h"

/* Chech comminication */
uint8_t clientSendCC(lora *_lora);
/* Send Payload */
uint8_t clientSendSP(lora *_lora);
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