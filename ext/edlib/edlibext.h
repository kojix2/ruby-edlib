#ifndef EDLIBEXT_H
#define EDLIBEXT_H

#include "edlib.h"

typedef struct
{
    EdlibAlignConfig *config;
    EdlibEqualityPair *equalityPairs;
} RbAlignConfig;

static EdlibAlignConfig *aligner_get_config(VALUE self);
static EdlibEqualityPair *aligner_get_equalityPairs(VALUE self);

#endif // EDLIBEXT_H