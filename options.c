/*
 * options.c
 *
 *  Created on: 13 дек. 2021 г.
 *      Author: serge78rus
 */

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "options.h"
#include "errmsg.h"
#include "def.h"

/*
 * local variables
 */

static struct option long_options[] = {
	    {.name = "help", 			.has_arg = no_argument, 		.flag = 0, .val = 'h'},
	    {.name = "version",			.has_arg = no_argument, 		.flag = 0, .val = 'V'},
	    {.name = "verbose", 		.has_arg = no_argument, 		.flag = 0, .val = 'v'},
	    {.name = "device", 			.has_arg = required_argument,	.flag = 0, .val = 'd'},
	    {.name = "baudrate",		.has_arg = required_argument,	.flag = 0, .val = 'b'},
	    {.name = "timeout",		    .has_arg = required_argument,	.flag = 0, .val = 't'},
	    {.name = "command",		    .has_arg = required_argument,	.flag = 0, .val = 'c'},

		{0, 0, 0, 0}
};

static struct Options options = {
		.help_flag = false,
		.version_flag = false,
		.verbose_flag = false,
		.serial_device = 0,
		.baudrate = 0,
		.timeout_ms = DEFAULT_TIMEOUT_MS,
		.commands = 0
};

/*
 * public functions
 */

struct Options* options_parse(int argc, char **argv)
{
	for (;;) {
		//int option_index = -1;

		int c = getopt_long(argc, argv, "hVvd:b:t:c:", long_options, 0/*&option_index*/);

		if (c == -1) {
			break;
		}

		switch (c) {
		case 0:
			ERR_MSG("getopt_long() return 0");
			return 0;

		case 'h':
			options.help_flag = true;
			break;
		case 'V':
			options.version_flag = true;
			break;
		case 'v':
			options.verbose_flag = true;
			break;
		case 'd':
			options.serial_device = optarg;
			break;
		case 'b': {
			char *end_ptr = optarg;
			options.baudrate = strtoul(optarg, &end_ptr, 10);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid unsigned integer value \"%s\" in command line", optarg);
				return 0;
			}
			break;
		}
		case 't': {
			char *end_ptr = optarg;
			options.timeout_ms = strtoul(optarg, &end_ptr, 10);
			if (end_ptr == optarg || *end_ptr) {
				ERR_MSG_F("Invalid unsigned integer value \"%s\" in command line", optarg);
				return 0;
			}
			break;
		}
		case 'c': {
			struct Command **ptr = &(options.commands);
			while (*ptr) {
				ptr = &((*ptr)->next);
			}
			*ptr = malloc(sizeof(struct Command));
			(*ptr)->command = optarg;
			(*ptr)->next = 0;
			break;
		}

		default:
			ERR_MSG("Unknown getopt_long() error");
			return 0;
		}
	}

	if (optind < argc) {
		ERR_MSG_F("Unknown command line argument%s", argc - optind > 1 ? "s" : "");
		while (optind < argc) {
			ERR_MSG_F(" %s", argv[optind++]);
		}
		return 0;
	}

	return &options;
}

void options_print(void)
{
	printf("Options:\n"
			"help:     %s\n"
			"version:  %s\n"
			"verbose:  %s\n"
			"device:   %s\n"
			"baudrate: %lu%s\n",
			options.help_flag ? "true" : "false",
			options.version_flag ? "true" : "false",
			options.verbose_flag ? "true" : "false",
			options.serial_device,
			options.baudrate, options.baudrate ? "" : " (not specified)"
	);

	printf("commands:\n");
	for (struct Command *cmd = options.commands; cmd; cmd = cmd->next) {
		printf("\t%s\n", cmd->command);
	}

	printf("\n");
}

void options_help(void)
{
	printf( "\n"
			"Usage: "APP_NAME" options\n"
			"\n"
			"options:\n"
			"\n"
			"-h, --help                       show this help screen and exit\n"
			"-V, --version                    show version information and exit\n"
			"-v, --verbose          optional  show verbose information\n"
			"                                 (default: false)\n"
			"-d, --device=NAME      required  serial communication device\n"
			"-b, --baudrate=VALUE   optional  communication baudrate\n"
			"                                 (default: current setting for device)\n"
			"-t, --timeout=VALUE    optional  communication timeout in milliseconds\n"
			"                                 (default: %i ms)\n"
			"-c, --command=COMMAND  required  SCPI command\n"
			"                                 If COMMAND contain spaces it must be quoted\n"
			"                                 May use more than one command, for examle:\n"
			"                                 -c COMMAND1 -c COMMAND2 ... etc\n"
			"\n",

			DEFAULT_TIMEOUT_MS
	);
}

void options_version(void)
{
	printf(APP_NAME" version "VERSION" by Serge L. Ryadkow (aka Serge78rus), GPL-3.0 License\n");
}

void options_clean(void)
{
	struct Command *cmd = options.commands;
	while (cmd) {
		struct Command *next = cmd->next;
		free(cmd);
		cmd = next;
	}
}

