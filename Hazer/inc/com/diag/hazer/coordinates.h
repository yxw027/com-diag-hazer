/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_HAZER_COORDINATES_
#define _H_COM_DIAG_HAZER_COORDINATES_

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

/*
 * POS 39°47'39.216"N, 105°09'12.106"W    39.7942268, -105.1533628        GNSS
 * HPP   39.794226865, -105.153362915 ±     1.1993m                       GNSS
 * NGS  39 47 39.21671(N) 105 09 12.10649(W)                              GNSS
 */

/*
 * These are #define symbols and not static const variables for a reason.
 */

#define COORDINATES_FORMAT_HPP "%lf"

#define COORDINATES_FORMAT_NGS "%u %u %lf(%c)"

#define COORDINATES_FORMAT_POS "%u\u00B0%u'%lf\"%c"

int coordinates_parse(const char * string, double * latitudep, double * longitudep);

#endif
