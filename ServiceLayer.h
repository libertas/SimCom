#ifndef _SERVICELAYER_H
#define _SERVICELAYER_H

#include <stdbool.h>

#include "SimCom.h"

#define SL_CALLBACK_NUM 2
#define SL_BUF_LEN 200

bool sl_init();
bool sl_config(char port, void (*callback)(char*, SIMCOM_LENGTH_TYPE));
bool sl_send(char from_port, char to_port, char *data, SIMCOM_LENGTH_TYPE length);

#endif
