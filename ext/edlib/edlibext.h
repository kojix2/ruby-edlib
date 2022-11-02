#ifndef EDLIBEXT_H
#define EDLIBEXT_H

#include "edlib.h"

typedef struct
{
    EdlibAlignConfig *config;
    EdlibEqualityPair *equalityPairs;
} RbAlignConfig;

static VALUE aligner_config_allocate(VALUE klass);
static size_t aligner_config_memsize(const void *ptr);
static void aligner_config_free(void *ptr);

static EdlibAlignConfig *aligner_get_config(VALUE self);
static EdlibEqualityPair *aligner_get_equalityPairs(VALUE self);

static VALUE get_k(EdlibAlignConfig *config);
static VALUE aligner_get_k(VALUE self);
static VALUE set_k(EdlibAlignConfig *config, VALUE k);
static VALUE aligner_set_k(VALUE self, VALUE k);
static VALUE get_mode(EdlibAlignConfig *config);
static VALUE aligner_get_mode(VALUE self);
static VALUE set_mode(EdlibAlignConfig *config, VALUE mode);
static VALUE aligner_set_mode(VALUE self, VALUE mode);
static VALUE get_task(EdlibAlignConfig *config);
static VALUE aligner_get_task(VALUE self);
static VALUE set_task(EdlibAlignConfig *config, VALUE task);
static VALUE aligner_set_task(VALUE self, VALUE task);
static VALUE get_additional_equalities(EdlibAlignConfig *config);
static VALUE aligner_get_additional_equalities(VALUE self);
static VALUE set_additional_equalities(EdlibAlignConfig *config, EdlibEqualityPair *eqpairs, VALUE equalities);
static VALUE aligner_set_additional_equalities(VALUE self, VALUE equalities);

static VALUE aligner_config_hash(VALUE self);

static VALUE aligner_initialize_raw(VALUE self, VALUE k, VALUE mode, VALUE task, VALUE additional_equalities);
static VALUE aligner_align(VALUE self, VALUE query, VALUE target);
void Init_edlibext(void);

#endif // EDLIBEXT_H