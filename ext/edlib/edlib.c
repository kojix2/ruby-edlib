#include "ruby.h"
#include "edlib.h"

VALUE mEdlib;
VALUE cAligner;

static const rb_data_type_t config_type = {
	.wrap_struct_name = "EdlibAlignConfig",
	.function = {
		.dfree = RUBY_DEFAULT_FREE,
		//.dsize = config_memsize,
	},
	.flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
config_allocate(VALUE klass)
{
	EdlibAlignConfig *config;
	VALUE obj = TypedData_Make_Struct(klass, EdlibAlignConfig,
									  &config_type, config);
	return obj;
}

static size_t
config_memsize(const void *ptr)
{
	const EdlibAlignConfig *config = ptr;
	return sizeof(ptr) + 2 * sizeof(char) * (config->additionalEqualitiesLength);
}

static EdlibAlignConfig *get_config(VALUE self)
{
	EdlibAlignConfig *ptr = NULL;
	TypedData_Get_Struct(self, EdlibAlignConfig, &config_type, ptr);

	return ptr;
}

static VALUE
aligner_initialize(VALUE self, VALUE k, VALUE mode, VALUE task)
{
	EdlibAlignConfig *config = get_config(self);
	config->k = NUM2INT(k);
	config->mode = NUM2INT(mode);
	config->task = NUM2INT(task);
	config->additionalEqualities = NULL;
	config->additionalEqualitiesLength = 0;

	return self;
}

static VALUE
aligner_config_hash(VALUE self)
{
	EdlibAlignConfig *config = get_config(self);
	VALUE hash = rb_hash_new();
	rb_hash_aset(hash, ID2SYM(rb_intern("k")), INT2NUM(config->k));
	rb_hash_aset(hash, ID2SYM(rb_intern("mode")), INT2NUM(config->mode));
	rb_hash_aset(hash, ID2SYM(rb_intern("task")), INT2NUM(config->task));
	// rb_hash_aset(hash, ID2SYM(rb_intern("additionalEqualities")), rb_str_new(config->additionalEqualities, config->additionalEqualitiesLength));
	rb_hash_aset(hash, ID2SYM(rb_intern("additionalEqualitiesLength")), INT2NUM(config->additionalEqualitiesLength));
	return hash;
}

static VALUE
get_k(VALUE self)
{
	const EdlibAlignConfig *config = DATA_PTR(self);
	return INT2NUM(config->k);
}

static VALUE
get_mode(VALUE self)
{
	const EdlibAlignConfig *config = get_config(self);
	switch (config->mode)
	{
	case 0:
		return rb_str_new2("HW");
	case 1:
		return rb_str_new2("SHW");
	case 2:
		return rb_str_new2("NW");
	default:
		return Qnil;
	}
}

static VALUE
get_task(VALUE self)
{
	const EdlibAlignConfig *config = get_config(self);
	switch (config->task)
	{
	case 0:
		return rb_str_new2("DISTANCE");
	case 1:
		return rb_str_new2("LOC");
	case 2:
		return rb_str_new2("PATH");
	default:
		return Qnil;
	}
}

static VALUE
aligner_align(VALUE self, VALUE query, VALUE target)
{

	EdlibAlignConfig *config = get_config(self);
	if (!config)
	{
		rb_raise(rb_eRuntimeError, "config is NULL");
	}

	EdlibAlignResult result = edlibAlign(
		StringValueCStr(query),
		RSTRING_LEN(query),
		StringValueCStr(target),
		RSTRING_LEN(target),
		edlibDefaultAlignConfig());

	VALUE editDistance = INT2NUM(result.editDistance);
	VALUE numLocations = INT2NUM(result.numLocations);
	VALUE endLocations = rb_ary_new();
	VALUE startLocations = rb_ary_new();
	for (int i = 0; i < result.numLocations; i++)
	{
		rb_ary_push(endLocations, INT2NUM(result.endLocations[i]));
		// rb_ary_push(startLocations, INT2NUM(result.startLocations[i]));
	}
	VALUE alignmentLength = INT2NUM(result.alignmentLength);
	VALUE alphabetLength = INT2NUM(result.alphabetLength);

	VALUE hash = rb_hash_new();
	rb_hash_aset(hash, ID2SYM(rb_intern("editDistance")), editDistance);
	rb_hash_aset(hash, ID2SYM(rb_intern("numLocations")), numLocations);
	rb_hash_aset(hash, ID2SYM(rb_intern("endLocations")), endLocations);
	// rb_hash_aset(hash, ID2SYM(rb_intern("startLocations")), startLocations);

	rb_hash_aset(hash, ID2SYM(rb_intern("alignment")), rb_str_new(result.alignment, result.alignmentLength));
	rb_hash_aset(hash, ID2SYM(rb_intern("alignmentLength")), alignmentLength);
	rb_hash_aset(hash, ID2SYM(rb_intern("alphabetLength")), alphabetLength);
	// edlibFreeAlignResult(result);

	return hash;
}

void Init_edlib(void)
{
	mEdlib = rb_define_module("Edlib");
	cAligner = rb_define_class_under(mEdlib, "Aligner", rb_cObject);
	rb_define_alloc_func(cAligner, config_allocate);
	rb_define_method(cAligner, "initialize", aligner_initialize, 3);
	rb_define_method(cAligner, "k", get_k, 0);
	rb_define_method(cAligner, "mode", get_mode, 0);
	rb_define_method(cAligner, "task", get_task, 0);
	rb_define_method(cAligner, "config", aligner_config_hash, 0);
	rb_define_method(cAligner, "align", aligner_align, 2);
}
