#include "ruby.h"
#include "edlib.h"

VALUE mEdlib;
VALUE cAligner;

static void config_free(void *ptr);
static size_t config_memsize(const void *ptr);

static const rb_data_type_t config_type = {
    .wrap_struct_name = "EdlibAlignConfig",
    .function = {
        .dfree = config_free,
        .dsize = config_memsize,
    },
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
config_allocate(VALUE klass)
{
  struct EdlibAlignConfig *config;
  VALUE obj = TypedData_Make_Struct(klass, EdlibAlignConfig,
                                    &config_type, config);
  return obj;
}

static void
config_free(void *ptr)
{
  // EdlibAlignConfig *config = (EdlibAlignConfig *)ptr;
  // free(config->additionalEqualities);
  xfree(ptr);
}

static size_t
config_memsize(const void *ptr)
{
  const EdlibAlignConfig *config = ptr;
  return sizeof(ptr) + 2 * sizeof(char) * (config->additionalEqualitiesLength);
}

static VALUE
aligner_init0(VALUE self)
{
  EdlibAlignConfig config = edlibDefaultAlignConfig();
  DATA_PTR(self) = &config;
  return self;
}

static VALUE

void Init_edlib(void)
{
  mEdlib = rb_define_module("Edlib");
  cAligner = rb_define_class_under(mEdlib, "Aligner", rb_cObject);
  rb_define_alloc_func(cAligner, config_allocate);
  rb_define_method(cAligner, "initialize_with_defaults", aligner_init0, 0);
  rb_deflie_method(cAligner, "align_raw", aligner_align, 4);
}
