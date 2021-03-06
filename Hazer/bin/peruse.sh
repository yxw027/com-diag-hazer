#!/bin/bash
# Copyright 2019 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer
# 
# ABSTRACT
#
# Helper script used to follow the log or the screens from the Tumbleweed
# scripts.
#
# USAGE
#
#    peruse TASK FILE
#
# EXAMPLES
#
#    peruse base out
#    peruse base err
#    peruse rover out
#    peruse rover err
#    peruse router err

SAVDIR=${COM_DIAG_HAZER_SAVE_DIR:-"./sav"}

PROGRAM=$(basename ${0})

TASK=${1}
FILE=${2}
DIRECTORY=${3:-${SAVDIR}}
LIMIT=${4:-$(($(stty size | cut -d ' ' -f 1) - 2))}

. $(readlink -e $(dirname ${0})/../bin)/setup

if [[ "${FILE}" == "err" ]]; then
    CMD="tail -n ${LIMIT} -f"
elif [[ "${FILE}" == "out" ]]; then
    CMD="headless"
elif [[ "${FILE}" == "csv" ]]; then
    CMD="tail -n ${LIMIT} -f"
else
    CMD="cat"
fi

exec ${CMD} ${DIRECTORY}/${TASK}.${FILE}
