#ifndef _DATALINKLAYER_H
#define _DATALINKLAYER_H

#include "CharQueue.h"
#include "SimCom.h"

#define DL_BUF_LEN 200
#define DL_RETRY_TIMES 10

void dl_daemon();
bool dl_init(char_queue *send_queue, char_queue *receive_queue);
bool dl_receive(char *data, SIMCOM_LENGTH_TYPE length);
bool dl_send(char *data, SIMCOM_LENGTH_TYPE length);

#endif