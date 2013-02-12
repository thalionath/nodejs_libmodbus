/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _MODBUS_RTU_PRIVATE_H_
#define _MODBUS_RTU_PRIVATE_H_

#ifndef _MSC_VER
#include <stdint.h>
#else
#include "stdint.h"
#endif

#if defined(_WIN32)
#include <windows.h>
#else
#include <termios.h>
#endif

#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2

/* Time waited beetween the RTS switch before transmit data or after transmit
   data before to read */
#define _MODBUS_RTU_TIME_BETWEEN_RTS_SWITCH 10000

#if defined(_WIN32)
#if !defined(ENOTSUP)
#define ENOTSUP WSAEOPNOTSUPP
#endif

/* WIN32: struct containing serial handle and a receive buffer */
#define PY_BUF_SIZE 512
struct win32_ser {
	/* File handle */
	HANDLE fd;
	/* Receive buffer */
	uint8_t buf[PY_BUF_SIZE];
	/* Received chars */
	DWORD n_bytes;
};
#endif /* _WIN32 */

typedef struct _modbus_rtu {
    /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*" on Mac OS X. */
    char *device;
    /* Bauds: 9600, 19200, 57600, 115200, etc */
    int baud;
    /* Data bit */
    uint8_t data_bit;
    /* Stop bit */
    uint8_t stop_bit;
    /* Parity: 'N', 'O', 'E' */
    char parity;
#if defined(_WIN32)
    struct win32_ser w_ser;
    DCB old_dcb;
#else
    /* Save old termios settings */
    struct termios old_tios;
#endif
#if HAVE_DECL_TIOCSRS485
    int serial_mode;
#endif
#if HAVE_DECL_TIOCM_RTS
    int rts;
    int onebyte_time;
#endif
    /* To handle many slaves on the same link */
    int confirmation_to_ignore;
} modbus_rtu_t;

typedef struct _modbus_rtu_mt {
	/* Pipes for control in multithread */
    int mtp_r;
    int mtp_w;
} modbus_rtu_mt_t;

#endif /* _MODBUS_RTU_PRIVATE_H_ */
