#!/bin/bash
# Copyright 2018 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer

PROGRAM=$(basename ${0})
DEVICE=${1:-"/dev/ttyACM0"}
RATE=${2:-9600}
FIFO=${3:-${TMPDIR:="/tmp"}"/"${PROGRAM}".fifo"}
BYTES=${4:-"512"}

. $(readlink -e $(dirname ${0})/../bin)/setup

. $(readlink -e $(dirname ${0})/../fun)/ubx8

OPTIONS=""
for OPTION in ${COMMANDS}; do
    OPTIONS="${OPTIONS} -W ${OPTION}"
done

eval gpstool -D ${DEVICE} -b ${RATE} -8 -n -1 -x ${OPTIONS} 2> >(log -S -N ${PROGRAM})

mkfifo ${FIFO} 

socat -u -b ${BYTES} OPEN:${DEVICE},b${RATE},cs8,rawer PIPE:${FIFO} & PID=$!

trap "kill -0 ${PID} && kill ${PID}; rm -f ${FIFO}" INT TERM

gpstool -S ${FIFO} -t 10 -E 2> >(log -S -N ${PROGRAM})