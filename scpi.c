/*
 * scpi.c
 *
 *  Created on: 14 дек. 2021 г.
 *      Author: serge78rus
 */

#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>

#include "scpi.h"
#include "errmsg.h"

//#define BAUDRATE B115200
#define BUFF_SIZE 1024 /*TODO...*/
//#define TIMEOUT_MS 1000

static int serial_fd = -1;
static struct termios old_serial_termios;
static struct termios new_serial_termios;
static char rx_buffer[BUFF_SIZE];

//private functions
static speed_t get_baudrate_const(unsigned long baudrate);

bool scpi_open(const char *serial, unsigned long baudrate)
{
	serial_fd = open(serial, O_RDWR | O_NOCTTY);
	if (serial_fd < 0) {
		SYS_ERR_F("open(), device: %s", serial);
		return false;
	}

	if (tcgetattr(serial_fd, &old_serial_termios)) {
		SYS_ERR_F("tcgetattr(), device: %s", serial);
		return false;
	}
	memcpy(&new_serial_termios, &old_serial_termios, sizeof(struct termios));

	cfmakeraw(&new_serial_termios);

	if (baudrate) {
		speed_t brc = get_baudrate_const(baudrate);
		if (brc) {
			cfsetspeed(&new_serial_termios, brc);
		}
	}

	if (tcsetattr(serial_fd, TCSANOW, &new_serial_termios)) {
		SYS_ERR_F("tcsetattr(), device: %s", serial);
		return false;
	}

	if (fcntl(serial_fd, F_SETFL, O_NONBLOCK)) {
		SYS_ERR_F("tcsetattr(), device: %s", serial);
		return false;
	}

	return true;
}

void scpi_close(void)
{
	if (serial_fd >= 0) {
		tcsetattr(serial_fd, TCSANOW, &old_serial_termios);
		close(serial_fd);
	}
}

bool scpi_send(const char *command)
{
	tcflush(serial_fd, TCIFLUSH);

	int tx_len = strlen(command);

	if (write(serial_fd, command, tx_len) != tx_len) {
		SYS_ERR_F("write() command: %s", command);
		return false;
	}

	if (write(serial_fd, "\x0A", 1) != 1) {
		SYS_ERR("write() LF");
		return false;
	}

	return true;
}

char* scpi_receive(int timeout_ms)
{
	int rx_len = 0;

	struct pollfd fds;
    fds.fd = serial_fd;
    fds.events = POLLIN;

    for (;;) {
        int res = poll(&fds, 1, timeout_ms);
        if (res < 0) {
        	SYS_ERR("pool()");
        	return 0;
        }

        if (res && (fds.revents & POLLIN)) {
        	int len = read(serial_fd, rx_buffer + rx_len, BUFF_SIZE - 1 - rx_len);
        	if (len < 0) {
            	SYS_ERR("read()");
            	return 0;
        	}

        	if (len) {
            	rx_len += len;
            	if (rx_buffer[rx_len - 1] == '\x0A') {
            		--rx_len; //remove '\x0A' terminator (no check length - its always > 0 here)

        			if (rx_len) {
                    	if (rx_buffer[rx_len - 1] == '\x0D') {
                			--rx_len; //remove '\x0D' terminator
                    	}
        			}

            		break;
            	}

        	}

        } else {
        	break;
        }
    }

    if (rx_len) {
    	rx_buffer[rx_len] = 0;
    	return rx_buffer;
    } else {
    	return 0;
    }
}

/*
 * private functions
 */

static speed_t get_baudrate_const(unsigned long baudrate)
{
	switch (baudrate) {
	case 50:
		return B50;
	case 75:
		return B75;
	case 110:
		return B110;
	case 134:
		return B134;
	case 150:
		return B150;
	case 200:
		return B200;
	case 300:
		return B300;
	case 600:
		return B600;
	case 1200:
		return B1200;
	case 1800:
		return B1800;
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	case 500000:
		return B500000;
	case 576000:
		return B576000;
	case 921600:
		return B921600;
	case 1000000:
		return B1000000;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	case 2000000:
		return B2000000;
	case 2500000:
		return B2500000;
	case 3000000:
		return B3000000;
	case 3500000:
		return B3500000;
	case 4000000:
		return B4000000;
	default:
		ERR_MSG_F("Invalid (non standard) baudrate: %lu", baudrate);
		return 0;
	}
}
