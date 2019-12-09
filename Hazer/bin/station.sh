#!/bin/bash
# Copyright 2019 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer
# Run the U-blox ZED-UBX-F9P with no additional configuration.

PROGRAM=$(basename ${0})
DEVICE=${1:-"/dev/tumbleweed"}
RATE=${2:-230400}
ERRFIL=${3-"./${PROGRAM}.err"}
OUTFIL=${4-"./${PROGRAM}.out"}

cp /dev/null ${ERRFIL}
exec 2>>${ERRFIL}

. $(readlink -e $(dirname ${0})/../bin)/setup

export COM_DIAG_DIMINUTO_LOG_MASK=0xfe

exec coreable gpstool \
    -F -H ${OUTFIL} -t 10 \
    -D ${DEVICE} -b ${RATE} -8 -n -1 \
    < /dev/null 1> /dev/null
