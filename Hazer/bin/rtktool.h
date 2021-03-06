/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAZER_RTKTOOL_
#define _H_COM_DIAG_HAZER_RTKTOOL_

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

#include "com/diag/hazer/datagram.h"
#include "com/diag/diminuto/diminuto_tree.h"
#include "com/diag/diminuto/diminuto_ipc6.h"
#include "com/diag/diminuto/diminuto_time.h"

/**
 * Clients of the router can be in one of two classes: a stationary base
 * station sending RTK updates, or a mobile (maybe) rover sending keepalives
 * and receiving RTK updates.
 */
typedef enum Class { CLASS = '?', BASE = 'B', ROVER = 'R', } class_t;

/**
 * This structure describes the state we have to maintain about clients.
 */
typedef struct Client {
    diminuto_tree_t node;
    long last;
    datagram_sequence_t sequence;
    class_t classification;
    diminuto_ipv6_t address;
    diminuto_port_t port;
} client_t;

/**
 * @define CLIENT_INITIALIZER
 * This is how we can statically initialize the client structure.
 */
#define CLIENT_INITIALIZER { DIMINUTO_TREE_NULLINIT, 0, 0, CLASSIFICATION, { 0, }, 0, }

#endif
