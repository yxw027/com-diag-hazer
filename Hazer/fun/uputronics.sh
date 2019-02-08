#!/bin/bash
# Copyright 2019 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer

PROGRAM=$(basename ${0})
DEVICE=${1:-"/dev/ttyAMA0"}
RATE=${2:-9600}

. $(readlink -e $(dirname ${0})/../bin)/setup

. $(readlink -e $(dirname ${0})/../fun)/ubx8

OPTIONS=""
for OPTION in ${COMMANDS}; do
    OPTIONS="${OPTIONS} -W ${OPTION}"
done

eval coreable gpstool -D ${DEVICE} -b ${RATE} -8 -n -1 -E -t 10 ${OPTIONS} 2> >(log -S -N ${PROGRAM})