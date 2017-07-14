#ifndef CAFEBABE_UTIL_H
#define CAFEBABE_UTIL_H

#include <stdio.h>
#include "util.h"
#include "cafebabetypes.h"

/* reading bytes can probably moved somewhere else */

u1 read_u1(FILE *f) {
    // TODO: It may make sense to change this to an int to handle EOF.
    u1 tmp = 0;

    // XXX: this can give EOF
    // which is an error because it's -1, but tmp can't hold that.
    // let's just assume no EOF!
    tmp = fgetc(f);

    return tmp;
}

u2 read_u2(FILE *f) {
    u2 tmp = 0;
    u2 i = 0;

    while (i < sizeof(u2)) {
        // XXX: this can give EOF
        // which is an error because it's -1, but tmp can't hold that.
        // let's just assume no EOF!
        tmp |= fgetc(f);
        if (i + 1 < sizeof(u2)) {
            tmp <<= 8;
        }
        i++;
    }

    return tmp;
}

u4 read_u4(FILE *f) {
    u4 tmp = 0;
    u4 i = 0;

    while (i < sizeof(u4)) {
        // XXX: this can give EOF
        // which is an error because it's -1, but tmp can't hold that.
        // let's just assume no EOF!
        tmp |= fgetc(f);
        if (i + 1 < sizeof(u4)) {
            tmp <<= 8;
        }
        i++;
    }

    return tmp;
}

/* END reading bytes */

#endif
