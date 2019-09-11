#!/bin/bash
# Copyright 2019 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer
# Configure and run the Ardusimple SimpleRTK2B as a fixed Base.

PROGRAM=$(basename ${0})
LOG=$(readlink -e $(dirname ${0})/..)/log
mkdir -p ${LOG}

ROUTER=${1:-"tumbleweed:tumbleweed"}
DEVICE=${2:-"/dev/tumbleweed"}
FILE=${3:-${LOG}/base.fix}
RATE=${4:-230400}

. $(readlink -e $(dirname ${0})/../bin)/setup

export COM_DIAG_DIMINUTO_LOG_MASK=0xfe

cp /dev/null ${LOG}/${PROGRAM}.err

test -f ${FILE} || exit 1

exec 2>>${LOG}/${PROGRAM}.err
exec 0<${FILE}

read -r CFG_TMODE_LAT
read -r CFG_TMODE_LATHP
read -r CFG_TMODE_LON
read -r CFG_TMODE_LONHP
read -r CFG_TMODE_HEIGHT
read -r CFG_TMODE_HEIGHTHP

log -N ${PROGRAM} -i CFG_TMODE_LAT="\"${CFG_TMODE_LAT}\""
log -N ${PROGRAM} -i CFG_TMODE_LATHP="\"${CFG_TMODE_LATHP}\""
log -N ${PROGRAM} -i CFG_TMODE_LON="\"${CFG_TMODE_LON}\""
log -N ${PROGRAM} -i CFG_TMODE_LONHP="\"${CFG_TMODE_LONHP}\""
log -N ${PROGRAM} -i CFG_TMODE_HEIGHT="\"${CFG_TMODE_HEIGHT}\""
log -N ${PROGRAM} -i CFG_TMODE_HEIGHTHP="\"${CFG_TMODE_HEIGHTHP}\""

test -z "${CFG_TMODE_LAT}" && exit 2
test -z "${CFG_TMODE_LATHP}" && exit 2
test -z "${CFG_TMODE_LON}" && exit 2
test -z "${CFG_TMODE_LONHP}" && exit 2
test -z "${CFG_TMODE_HEIGHT}" && exit 2
test -z "${CFG_TMODE_HEIGHTHP}" && exit 2

# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-TMODE-MODE FIXED
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-TMODE-POS_TYPE LLH
# UBX-CFG-VALSET [12] V0 RAM 0 0 CFG-TMODE-CFG_TMODE_LAT (read)
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-TMODE-CFG_TMODE_LATHP (read)
# UBX-CFG-VALSET [12] V0 RAM 0 0 CFG-TMODE-CFG_TMODE_LON (read)
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-TMODE-CFG_TMODE_LONHP (read)
# UBX-CFG-VALSET [12] V0 RAM 0 0 CFG-TMODE-CFG_TMODE_HEIGHT (read)
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-TMODE-CFG_TMODE_HEIGHTHP (read)
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-RTCM_3X_TYPE1005_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-RTCM_3X_TYPE1074_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-RTCM_3X_TYPE1084_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-RTCM_3X_TYPE1094_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-RTCM_3X_TYPE1124_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-RTCM_3X_TYPE1230_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-USBINPROT-RTCM3X 0
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-USBOUTPROT-RTCM3X 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-MSGOUT-UBX_NAV_SVIN_USB 1
# UBX-CFG-VALSET [9] V0 RAM 0 0 CFG-UART2-ENABLED 0
# UBX-CFG-MSG [3] UBX-NAV-HPPOSLLH 1

exec coreable gpstool -D ${DEVICE} -b ${RATE} -8 -n -1 \
    -G ${ROUTER} -g 4 \
    -F -H ${LOG}/${PROGRAM}.out -t 10 \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x01\x00\x03\x20\x02' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x02\x00\x03\x20\x01' \
    -U "${CFG_TMODE_LAT}" \
    -U "${CFG_TMODE_LATHP}" \
    -U "${CFG_TMODE_LON}" \
    -U "${CFG_TMODE_LONHP}" \
    -U "${CFG_TMODE_HEIGHT}" \
    -U "${CFG_TMODE_HEIGHTHP}" \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\xc0\x02\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x61\x03\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x66\x03\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x6b\x03\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x70\x03\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x06\x03\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x04\x00\x77\x10\x00' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x04\x00\x78\x10\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x8b\x00\x91\x20\x01' \
    -U '\xb5\x62\x06\x8a\x09\x00\x00\x01\x00\x00\x05\x00\x53\x10\x00' \
    -U '\xb5\x62\x06\x01\x03\x00\x01\x14\x01' \
    < /dev/null 1> /dev/null