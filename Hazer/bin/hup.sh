#!/bin/bash
# Copyright 2020 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in LICENSE.txt
# Chip Overclock <coverclock@diag.com>
# https://github.com/coverclock/com-diag-hazer
# USAGE hup [ SIGNAL [ PROGRAM ] ]
# EXAMPLE hup
# EXAMPLE hup INT
# EXAMPLE hup INT footool
# Send a SIGHUP to all gpstool instances if any.

exec pkill -${1:-"HUP"} ${2:-"gpstool"}
