/*
 *      sio.h
 *
 *      v 0.00 1999.10.8 by Reed Lai
 *
 *      header of sio.c
 *
 *      History
 *
 *              0.00    create
 */

#ifndef _SIO_H
#define _SIO_H

#include <stdio.h>				/* Standard input/output definitions */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>				/* UNIX standard function definitions */
#include <fcntl.h>				/* File control definitions */
#include <errno.h>				/* Error number definitions */
#include <termios.h>			/* POSIX terminal control definitions */

#include "codes.h"
#include "common.h"
#include "debug.h"

/* Error Codes */
#define OK_PORT_CLOSE   0
#define ERR_PORT_CLOSE  -1
#define ERR_PORT_OPEN   0		/* DO NOT CHANGE THIS!! */

/* sio devices */
#define SIO_DEV0     "/dev/ttyS0"
#define SIO_DEV1     "/dev/ttyS1"
#define SIO_DEV2     "/dev/ttyS2"
#define SIO_DEV3     "/dev/ttyS3"
#define SIO_DEV4     "/dev/ttyS4"

/* CAN devices */
/*
#define USB_DEV0     "/dev/ttyUSB0"
#define USB_DEV1     "/dev/ttyUSB1"
#define USB_DEV2     "/dev/ttyUSB2"
#define USB_DEV3     "/dev/ttyUSB3"
*/
/* data bits */
#define DATA_BITS_5  CS5
#define DATA_BITS_6  CS6
#define DATA_BITS_7  CS7
#define DATA_BITS_8  CS8

/* parity check */
#define NO_PARITY    0
#define ODD_PARITY   1
#define EVEN_PARITY  2

/* stop bit(s) */
#define ONE_STOP_BIT    0
#define ONE_5_STOP_BIT  1		/* but there is no 1.5 stop bit to be */
				/* defined in Linux POSIX control mode flags */
#define ONE_P5_STOP_BIT 1		/* but there is no 1.5 stop bit defined */
				/* in Linux POSIX control mode flags */
#define TWO_STOP_BIT    2
#define TWO_STOP_BITS   2

struct sio {
	struct sio *next;			/* next opened sio device */
	int fd;						/* file descriptor of this port */
	const char *name;			/* device name */
	struct termios *options;	/* current options of this port */
	struct termios *old_options;	/* original options */
};

extern void sio_set_noncan(int fd);
extern void sio_set_canon(int fd);
extern void sio_set_timer(int fd, cc_t vmin, cc_t vtime);
extern void sio_poptions(int fd);
extern int sio_set_baud(int fd, speed_t baud);
extern int sio_open(const char *port, speed_t baud, tcflag_t data,
					tcflag_t parity, tcflag_t stop);
extern int sio_close(int fd);

#endif							/* _SIO_H */
