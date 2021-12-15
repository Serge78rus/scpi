/*
 * errmsg.h
 *
 *  Created on: 13 дек. 2021 г.
 *      Author: serge78rus
 */

#ifndef ERRMSG_H_
#define ERRMSG_H_

#include <stdio.h>
#include <errno.h>

#define ERR_MSG(message) fprintf(stderr, "ERROR [%s:%d]\t"message"\n", __FILE__, __LINE__)
#define ERR_MSG_F(message, ...) fprintf(stderr, "ERROR [%s:%d]\t"message"\n", __FILE__, __LINE__, __VA_ARGS__)

#define SYS_ERR(message) fprintf(stderr, "ERROR [%s:%d]\t"message" : %s\n", __FILE__, __LINE__, errno ? strerror(errno) : "?")
#define SYS_ERR_F(message, ...) fprintf(stderr, "ERROR [%s:%d]\t"message" : %s\n", __FILE__, __LINE__, __VA_ARGS__, errno ? strerror(errno) : "?")

#endif /* ERRMSG_H_ */
