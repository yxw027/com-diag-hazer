/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * EXAMPLE
 *
 * gpstool [ -d ] [ -v ]
 */

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "com/diag/hazer/hazer.h"

static int send_sentence(FILE * fp, const char * data)
{
    int rc = -1;
    uint8_t cs = 0;
    char msn = '\0';
    char lsn = '\0';

    do {
        cs = hazer_checksum(data, strlen(data));
        if (hazer_checksum2characters(cs, &msn, &lsn) < 0) { break; }
        if (fprintf(fp, "%s%c%c%c\r\n", data, HAZER_STIMULUS_CHECKSUM, msn, lsn) < 0) { break; }
        if (fflush(fp) == EOF) { break; }
        rc = 0;

    } while (0);

    return rc;
}

static void print_solution(FILE * fp, const char * name, const hazer_constellation_t * cp)
{
    static const int SATELLITES = sizeof(cp->id) / sizeof(cp->id[0]);
    int satellite = 0;
    int satellites = 0;
    int limit = 0;

    satellites = cp->satellites;
    limit = (satellites > SATELLITES) ? SATELLITES : satellites;

    fprintf(fp, "%s {", name);
    for (satellite = 0; satellite < limit; ++satellite) {
        if (cp->id[satellite] != 0) {
            fprintf(fp, " %2u", cp->id[satellite]);
        }
    }
    fprintf(fp, " } [%02d/%02d] pdop %.2lf hdop %.2lf vdop %.2lf\n", satellites, SATELLITES, cp->pdop, cp->hdop, cp->vdop);
}

static void print_constellation(FILE *fp, const char * name, const hazer_constellation_t * cp)
{
    static const int CHANNELS = sizeof(cp->sat) / sizeof(cp->sat[0]);
    int channel = 0;
    int channels = 0;
    int limit = 0;

    channels = cp->channels;
    limit = (channels > CHANNELS) ? CHANNELS : channels;

    for (channel = 0; channel < limit; ++channel) {
        if (cp->sat[channel].id != 0) {
            fprintf(fp, "%s [%02d/%02d/%02d] sat %2u elv %2u azm %3u snr %2udBHz\n", name, channel + 1, channels, CHANNELS, cp->sat[channel].id, cp->sat[channel].elv_degrees, cp->sat[channel].azm_degrees, cp->sat[channel].snr_dbhz);
        }
    }
}

static void print_position(FILE * fp, const char * name, const hazer_position_t * pp)
{
    uint64_t nanoseconds = 0;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int degrees = 0;
    int minutes = 0;
    int seconds = 0;
    int hundredths = 0;
    int direction = 0;
    const char * compass = (const char *)0;
    double decimal = 0.0;

    nanoseconds = pp->dmy_nanoseconds;
    if (nanoseconds == 0) { return; }

    fputs(name, fp);

    nanoseconds += pp->utc_nanoseconds;
    hazer_format_nanoseconds2timestamp(nanoseconds, &year, &month, &day, &hour, &minute, &second, &nanoseconds);
    assert((1 <= month) && (month <= 12));
    assert((1 <= day) && (day <= 31));
    assert((0 <= hour) && (hour <= 23));
    assert((0 <= minute) && (minute <= 59));
    assert((0 <= second) && (second <= 59));
    assert((0 <= nanoseconds) && (nanoseconds < 1000000000ULL));
    fprintf(fp, " %04d-%02d-%02dT%02d:%02d:%02dZ", year, month, day, hour, minute, second);

    /* Latitude and longitude are printed in a format maps.google.com kinda likes. */

    hazer_format_nanodegrees2position(pp->lat_nanodegrees, &degrees, &minutes, &seconds, &hundredths, &direction);
    assert((0 <= degrees) && (degrees <= 90));
    assert((0 <= minutes) && (minutes <= 59));
    assert((0 <= seconds) && (seconds <= 59));
    assert((0 <= hundredths) && (hundredths <= 99));
    fprintf(fp, " { %d %02d' %02d.%02d\"%c", degrees, minutes, seconds, hundredths, direction < 0 ? 'S' : 'N');

    hazer_format_nanodegrees2position(pp->lon_nanodegrees, &degrees, &minutes, &seconds, &hundredths, &direction);
    assert((0 <= degrees) && (degrees <= 180));
    assert((0 <= minutes) && (minutes <= 59));
    assert((0 <= seconds) && (seconds <= 59));
    assert((0 <= hundredths) && (hundredths <= 99));
    fprintf(fp, " %d %02d' %02d.%02d\"%c }", degrees, minutes, seconds, hundredths, direction < 0 ? 'W' : 'E');

    fprintf(fp, " %.2lf'", pp->alt_millimeters * 3.2808 / 1000.0);

    assert((0LL <= pp->cog_nanodegrees) && (pp->cog_nanodegrees <= 360000000000LL));
    compass = hazer_format_nanodegrees2compass(pp->cog_nanodegrees);
    assert(compass != (const char *)0);
    assert(strlen(compass) <= 4);
    fprintf(fp, " %s", compass);

    fprintf(fp, " %.2lfmph", pp->sog_microknots * 1.150779 / 1000000.0);

    fputc('\n', fp);

    fputs("MAP", fp);

    decimal = pp->lat_nanodegrees;
    decimal /= 1000000000.0;
    fprintf(fp, " { %.6lf", decimal);

    decimal = pp->lon_nanodegrees;
    decimal /= 1000000000.0;
    fprintf(fp, ",%.6lf }", decimal);

    decimal = pp->alt_millimeters;
    decimal /= 1000.0;
    fprintf(fp, " %.3lf", decimal);

    decimal = pp->cog_nanodegrees;
    decimal /= 1000000000.0;
    fprintf(fp, " %.3lf", decimal);

    decimal = pp->sog_microknots;
    decimal /= 1000000.0;
    fprintf(fp, " %.3lf", decimal);

    fputc('\n', fp);
}

int main(int argc, char * argv[])
{
    const char * program = (const char *)0;
    int debug = 0;
    int verbose = 0;
    int escape = 0;
    hazer_state_t state = HAZER_STATE_EOF;
    hazer_state_t prior = HAZER_STATE_START;
    hazer_buffer_t buffer = { 0 };
    hazer_vector_t vector = { 0 };
    hazer_position_t position = { 0 };
    hazer_constellation_t constellation = { 0 };
    FILE * infp = stdin;
    FILE * outfp = stdout;
    FILE * errfp = stderr;
    int rc = 0;
    int ch = EOF;
    char * bb = (char *)0;
    ssize_t size = 0;
    ssize_t ss = 0;
    size_t current = 0;
    int end = 0;
    ssize_t check = 0;
    ssize_t count = 0;
    char ** vv = (char **)0;
    int tt = 0;
    uint8_t cs = 0;
    uint8_t ck = 0;
    char msn = '\0';
    char lsn = '\0';
    int opt = -1;
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    program = ((program = strrchr(argv[0], '/')) == (char *)0) ? argv[0] : program + 1;

    while ((opt = getopt(argc, argv, "dervw:?")) >= 0) {
        switch (opt) {
        case 'd':
            debug = !0;
            break;
        case 'e':
            escape = !0;
            break;
        case 'r':
            outfp = stderr;
            errfp = stdout;
            break;
        case 'v':
            verbose = !0;
            break;
        case 'w':
            return send_sentence(outfp, optarg) < 0 ? 1 : 0;
            break;
        case '?':
            fprintf(stderr, "usage: %s [ -d ] [ -v ] [ -e ] [ -w S ]\n", program);
            fprintf(stderr, "       -d      Display debug output to standard error.\n");
            fprintf(stderr, "       -e      Use ANSI escape sequences to control display.\n");
            fprintf(stderr, "       -r      Reverse use of standard output and error.\n");
            fprintf(stderr, "       -v      Display verbose output to standard error.\n");
            fprintf(stderr, "       -w S    Append * and checksum to S and write to standard output.\n");
            return 1;
            break;
        }
    }

    if (debug) {
        hazer_debug(stderr);
    }

    rc = hazer_initialize();
    assert(rc == 0);

    if (escape) { fputs("\033[1;1H\033[0J", outfp); }

    while (!0) {

        state = HAZER_STATE_START;

        while (!0) {
            ch = fgetc(infp);
            prior = state;
            state = hazer_machine(state, ch, buffer, sizeof(buffer), &bb, &ss);
            if (state == HAZER_STATE_END) {
                break;
            } else if  (state == HAZER_STATE_EOF) {
                break;
            } else if ((prior != HAZER_STATE_START) && (state == HAZER_STATE_START)) {
                /* State machine restarted. */
                fprintf(stderr, "%s: ERR\n", program);
            } else {
                /* Do nothing. */
            }
        }

        if (state == HAZER_STATE_EOF) {
            fprintf(stderr, "%s: EOF\n", program);
            break;
        }

        assert(state == HAZER_STATE_END);

        size = ss;
        assert(size > 0);

        assert(buffer[0] == '$');
        assert(buffer[size - 1] == '\0');
        assert(buffer[size - 2] == '\n');
        assert(buffer[size - 3] == '\r');
        assert(buffer[size - 6] == '*');

        cs = hazer_checksum(buffer, size);

        rc = hazer_characters2checksum(buffer[size - 5], buffer[size - 4], &ck);
        assert(rc >= 0);

        if (ck != cs) {
            /* Checksum failed. */
            fprintf(stderr, "%s: SUM 0x%02x 0x%02x\n", program, cs, ck);
            continue;
        }

        if (verbose) {
            fputs(buffer, errfp);
            fflush(errfp);
        }

        count = hazer_tokenize(vector, sizeof(vector) / sizeof(vector[0]),  buffer, size);
        assert(count >= 0);
        assert(vector[count] == (char *)0);
        assert(count < (sizeof(vector) / sizeof(vector[0])));

        if (escape) { fputs("\033[1;1H\033[0K", outfp); }
        for (vv = vector, tt = 1; *vv != (char *)0; ++vv, ++tt) {
            fputs(*vv, outfp);
            fputc((tt == count) ? '\n' : ',', outfp);
        }

        if (hazer_parse_gga(&position, vector, count) == 0) {
            if (escape) { fputs("\033[2;1H\033[0K", outfp); }
            print_position(outfp, "GGA",  &position);
        } else if (hazer_parse_rmc(&position, vector, count) == 0) {
            if (escape) { fputs("\033[2;1H\033[0K", outfp); }
            print_position(outfp, "RMC", &position);
        } else if (hazer_parse_gsa(&constellation, vector, count) == 0) {
            if (escape) { fputs("\033[4;1H\033[0K", outfp); }
            print_solution(outfp, "GSA", &constellation);
        } else if (hazer_parse_gsv(&constellation, vector, count) == 0) {
            if (escape) { fputs("\033[5;1H\033[0J", outfp); }
            print_constellation(outfp, "GSV", &constellation);
        } else {
            /* Do nothing. */
        }

        fflush(outfp);

    }

    rc = hazer_finalize();
    assert(rc == 0);

    return 0;
}
