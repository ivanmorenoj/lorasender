#ifndef UTILITIES_H_
#define UTILITIES_H_

void makeLoRaPayload(struct gas_values *_gv, lora_payload *_lp);
void preparePayload(lora_payload *_lp,char *_buff,unsigned int len);

#endif //UTILITIES_H_
