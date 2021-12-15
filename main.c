/*
 * main.c
 *
 *  Created on: 13 дек. 2021 г.
 *      Author: serge78rus
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#include "errmsg.h"
#include "options.h"
#include "scpi.h"

int main(int argc, char **argv)
{
	//printf("begin\n");

	struct Options *options = options_parse(argc, argv);
	if (!options) {
		ERR_MSG("Parsing command line error");
		options_help();
		options_clean();
		return 1;
	}
	if (options->help_flag) {
		options_help();
		options_clean();
		return 0;
	}
	if (options->version_flag) {
		options_version();
		options_clean();
		return 0;
	}
	if (options->verbose_flag) {
		options_print();
	}

	//TODO check options

	if (scpi_open(options->serial_device, options->baudrate)) {

		for (struct Command *cmd = options->commands; cmd; cmd = cmd->next) {

			if (options->verbose_flag) {
				printf("execute command: \"%s\"\n", cmd->command);
			}

			if (scpi_send(cmd->command)) {
				if (options->timeout_ms) {

					char *resp = scpi_receive(options->timeout_ms);
					if (resp) {
						printf("%s\n", resp);
					} else {
						ERR_MSG_F("scpi_receive() timeout, command: \"%s\"", cmd->command);
					}
				}
			}
		}
	}


	//printf("end\n");
	scpi_close();
	options_clean();
	return 0;
}

