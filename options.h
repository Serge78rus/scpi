/*
 * options.h
 *
 *  Created on: 13 дек. 2021 г.
 *      Author: serge78rus
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdbool.h>

//type definitions
struct Command {
	char *command;
	struct Command *next;
};

struct Options {
	bool help_flag;
	bool version_flag;
	bool verbose_flag;
	char *serial_device;
	unsigned long baudrate;
	unsigned long timeout_ms;
	struct Command *commands;
};

//public functions
struct Options* options_parse(int argc, char **argv);
void options_print(void);
void options_help(void);
void options_version(void);
void options_clean(void);

#endif /* OPTIONS_H_ */
