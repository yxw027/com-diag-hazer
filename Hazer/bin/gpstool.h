/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAZER_GPSTOOL_
#define _H_COM_DIAG_HAZER_GPSTOOL_

/**
 * @file
 *
 * Copyright 2019 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * https://github.com/coverclock/com-diag-hazer<BR>
 *
 * This file is part of the Digital Aggregates Corporation Hazer package.
 */

#include "com/diag/hazer/hazer.h"
#include "com/diag/hazer/yodel.h"
#include "com/diag/diminuto/diminuto_list.h"

/*******************************************************************************
 * HIGH PRECISION SOLUTION
 ******************************************************************************/

/**
 * Structure combining both a UBX-NAV-HPPOSLLH payload and its expiry time in
 * ticks.
 */
typedef struct YodelSolution {
	yodel_ubx_nav_hpposllh_t payload;   /* Payload from UBX-NAV-HPPOSLLH message. */
    expiry_t ticks;                     /* Lifetime in application-defined ticks. */
    uint8_t unused[3];
} yodel_solution_t;

/**
 * @define YODEL_SOLUTION_INITIALIZER
 * Initialize a YodelSolution structure.
 */
#define YODEL_SOLUTION_INITIALIZER \
    { \
	    YODEL_UBX_NAV_HPPOSLLH_INITIALIZER, \
		0, \
		{ 0, } \
    }

/*******************************************************************************
 * HARDWARE MONITOR
 ******************************************************************************/

/**
 * Structure combining both a UBX-MON-HW payload and its expiry time in ticks.
 */
typedef struct YodelHardware {
    yodel_ubx_mon_hw_t payload;	/* Payload from UBX-MON-HW message. */
    expiry_t ticks;				/* Lifetime in application-defined ticks. */
    uint8_t unused[3];
} yodel_hardware_t;

/**
 * @define YODEL_HARDWARE_INITIALIZER
 * Initialize a YodelHardware structure.
 */
#define YODEL_HARDWARE_INITIALIZER \
    { \
	    YODEL_UBX_MON_HW_INITIALIZER, \
		0, \
		{ 0, } \
    }

/*******************************************************************************
 * NAVIGATION STATUS
 ******************************************************************************/

/**
 * Structure combining both a UBX-NAV-STATUS payload and its expiry time in ticks.
 */
typedef struct YodelStatus {
    yodel_ubx_nav_status_t payload;	/* Payload from UBX-NAV-STATUS message. */
    expiry_t ticks;					/* Lifetime in application-defined ticks. */
    uint8_t unused[3];
} yodel_status_t;

/**
 * @define YODEL_STATUS_INITIALIZER
 * Initialize a YodelStatus structure.
 */
#define YODEL_STATUS_INITIALIZER \
    { \
	    YODEL_UBX_NAV_STATUS_INITIALIZER, \
		0, \
		{ 0, } \
    }

/*******************************************************************************
 * BASE STATUS
 ******************************************************************************/

/**
 * Structure combining both a UBX-NAV-SVIN payload and its expiry time in ticks.
 */
typedef struct YodelBase {
    yodel_ubx_nav_svin_t payload;	/* Payload from UBX-NAV-SVIN message. */
    expiry_t ticks;					/* Lifetime in application-defined ticks. */
    uint8_t unused[7];
} yodel_base_t;

/**
 * @define YODEL_BASE_INITIALIZER
 * Initialize a YodelBase structure.
 */
#define YODEL_BASE_INITIALIZER \
    { \
	    YODEL_UBX_NAV_SVIN_INITIALIZER, \
		0, \
		{ 0, }, \
    }

/*******************************************************************************
 * ROVER STATUS
 ******************************************************************************/

/**
 * Structure combining both a UBX-RXM-RTCM payload and its expiry time in ticks.
 */
typedef struct YodelRover {
    yodel_ubx_rxm_rtcm_t payload;	/* Payload from UBX-RXM-RTCM message. */
    expiry_t ticks;					/* Lifetime in application-defined ticks. */
    uint8_t unused[7];
} yodel_rover_t;

/**
 * @define YODEL_ROVER_INITIALIZER
 * Initialize a YodelRover structure.
 */
#define YODEL_ROVER_INITIALIZER \
    { \
	    YODEL_UBX_RXM_RTCM_INITIALIZER, \
		0, \
		{ 0, }, \
    }

/*******************************************************************************
 * STRUCTURES
 ******************************************************************************/

/**
 * The Poller structure is used by periodic DCD or GPIO poller threads to
 * communicate with the main program about the assertion of the 1Hz 1PPS
 * signal from certain GPS receivers which are so-equipped. The volatile
 * declaration is used to suggest to the compiler that it doesn't optimize
 * use of these variables out since they can be altered by other threads.
 */
struct Poller {
    FILE * ppsfp;
    FILE * strobefp;
    volatile int onepps;
    volatile int done;
};

/**
 * The Command structure contains a linked list node whose data pointer
 * points to the command we want to send, and the acknak field indicates
 * whether this command expects an UBX CFG ACK or a NAK from the device.
 */
struct Command {
	diminuto_list_t link;
	int acknak;
};

#endif