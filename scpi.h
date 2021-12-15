/*
 * scpi.h
 *
 *  Created on: 14 дек. 2021 г.
 *      Author: serge78rus
 */

#ifndef SCPI_H_
#define SCPI_H_

#include <stdbool.h>

//public functions
bool scpi_open(const char *serial, unsigned long baudrate);
void scpi_close(void);
bool scpi_send(const char *command);
char* scpi_receive(int timeout_ms);

#endif /* SCPI_H_ */
