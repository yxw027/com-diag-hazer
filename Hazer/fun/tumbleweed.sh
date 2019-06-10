#!/bin/bash
# Copyright 2019 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer

# This script is specific to the Ardusimple SimpleRTK2B.

PROGRAM=$(basename ${0})
DATAGRAM=${1:-"localhost:5555"}
SURVEYOR=${2:-"localhost:2101"}
DEVICE=${3:-"/dev/ttyACM0"}
RATE=${4:-115200}

. $(readlink -e $(dirname ${0})/../bin)/setup

DIR=$(readlink -e $(dirname ${0})/..)/log
mkdir -p ${DIR}

exec coreable gpstool -D ${DEVICE} -b ${RATE} -8 -n -1 -G ${DATAGRAM} -g 4 -Y ${SURVEYOR} -y 10 -E -F -t 10 2> ${DIR}/${PROGRAM}.log
