#ifndef UTILITIES_H_
#define UTILITIES_H_

void makeLoRaPayload(struct gas_values *_gv, lora_payload *_lp);
void hex2str(lora_payload *_lp,char *str);

#endif //UTILITIES_H_
