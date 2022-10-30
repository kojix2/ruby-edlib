#include "ruby.h"
#include "edlib.h"

VALUE mEdlib;
VALUE cAligner;

static size_t config_memsize(const void *ptr);

static const rb_data_type_t config_type = {
	.wrap_struct_name = "EdlibAlignConfig",
	.function = {
		.dfree = RUBY_DEFAULT_FREE,
		.dsize = config_memsize,
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
get_k(EdlibAlignConfig *config)
{
	return INT2NUM(config->k);
}

static VALUE
aligner_get_k(VALUE self)
{
	EdlibAlignConfig *config = get_config(self);
	return get_k(config);
}

static VALUE
set_k(EdlibAlignConfig *config, VALUE k)
{
	config->k = NUM2INT(k);
	return k;
}

static VALUE
aligner_set_k(VALUE self, VALUE k)
{
	EdlibAlignConfig *config = get_config(self);
	return set_k(config, k);
}

static VALUE
get_mode(EdlibAlignConfig *config)
{
	switch (config->mode)
	{
	case 0:
		return rb_str_new2("NW");
	case 1:
		return rb_str_new2("SHW");
	case 2:
		return rb_str_new2("HW");
	default:
		return Qnil;
	}
}

static VALUE
aligner_get_mode(VALUE self)
{
	EdlibAlignConfig *config = get_config(self);
	return get_mode(config);
}

static VALUE
set_mode(EdlibAlignConfig *config, VALUE mode)
{
	switch (TYPE(mode))
	{
	case T_STRING:
		if (strcmp(RSTRING_PTR(mode), "NW") == 0)
		{
			config->mode = 0;
		}
		else if (strcmp(RSTRING_PTR(mode), "SHW") == 0)
		{
			config->mode = 1;
		}
		else if (strcmp(RSTRING_PTR(mode), "HW") == 0)
		{
			config->mode = 2;
		}
		else
		{
			rb_raise(rb_eArgError, "Invalid mode");
		}
		break;
	case T_FIXNUM:
		int m = NUM2INT(mode);
		if (m < 0 || m > 2)
		{
			rb_raise(rb_eArgError, "Invalid mode");
		}
		config->mode = NUM2INT(mode);
		break;
	default:
		rb_raise(rb_eArgError, "Invalid mode");
	}
	return mode;
}

static VALUE
aligner_set_mode(VALUE self, VALUE mode)
{
	EdlibAlignConfig *config = get_config(self);
	return set_mode(config, mode);
}

static VALUE
get_task(EdlibAlignConfig *config)
{
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
aligner_get_task(VALUE self)
{
	EdlibAlignConfig *config = get_config(self);
	return get_task(config);
}

static VALUE
set_task(EdlibAlignConfig *config, VALUE task)
{
	switch (TYPE(task))
	{
	case T_STRING:
		if (strcmp(RSTRING_PTR(task), "DISTANCE") == 0)
		{
			config->task = 0;
		}
		else if (strcmp(RSTRING_PTR(task), "LOC") == 0)
		{
			config->task = 1;
		}
		else if (strcmp(RSTRING_PTR(task), "PATH") == 0)
		{
			config->task = 2;
		}
		else
		{
			rb_raise(rb_eArgError, "Invalid task");
		}
		break;
	case T_FIXNUM:
		int t = NUM2INT(task);
		if (t < 0 || t > 2)
		{
			rb_raise(rb_eArgError, "Invalid task");
		}
		config->task = NUM2INT(task);
		break;
	default:
		rb_raise(rb_eArgError, "Invalid task");
	}
	return task;
}

static VALUE
aligner_set_task(VALUE self, VALUE task)
{
	EdlibAlignConfig *config = get_config(self);
	return set_task(config, task);
}

static VALUE
get_additional_equalities(EdlibAlignConfig *config)
{
	VALUE equalities = rb_ary_new();

	for (int i = 0; i < config->additionalEqualitiesLength; i++)
	{
		EdlibEqualityPair pair = config->additionalEqualities[i];
		VALUE pair_ary = rb_ary_new();
		rb_ary_push(pair_ary, rb_str_new(&pair.first, 1));
		rb_ary_push(pair_ary, rb_str_new(&pair.second, 1));
		rb_ary_push(equalities, pair_ary);
	}

	return equalities;
}

static VALUE
aligner_get_additional_equalities(VALUE self)
{
	EdlibAlignConfig *config = get_config(self);
	return get_additional_equalities(config);
}

static VALUE
set_additional_equalities(EdlibAlignConfig *config, VALUE equalities)
{
	Check_Type(equalities, T_ARRAY);
	EdlibEqualityPair *pairs = (EdlibEqualityPair *)malloc(sizeof(EdlibEqualityPair) * RARRAY_LEN(equalities));

	for (int i = 0; i < RARRAY_LEN(equalities); i++)
	{
		VALUE pair = rb_ary_entry(equalities, i);
		Check_Type(pair, T_ARRAY);
		if (RARRAY_LEN(pair) != 2)
		{
			rb_raise(rb_eArgError, "Invalid equality pair");
		}
		VALUE s1 = rb_ary_entry(pair, 0);
		VALUE s2 = rb_ary_entry(pair, 1);
		Check_Type(s1, T_STRING);
		Check_Type(s2, T_STRING);
		if (RSTRING_LEN(s1) != 1 || RSTRING_LEN(s2) != 1)
		{
			rb_raise(rb_eArgError, "String length must be 1");
		}
		char c1 = RSTRING_PTR(s1)[0];
		char c2 = RSTRING_PTR(s2)[0];

		pairs[i].first = c1;
		pairs[i].second = c2;
	}

	config->additionalEqualities = pairs;
	config->additionalEqualitiesLength = RARRAY_LEN(equalities);

	return equalities;
}

static VALUE
aligner_set_additional_equalities(VALUE self, VALUE equalities)
{
	EdlibAlignConfig *config = get_config(self);
	return set_additional_equalities(config, equalities);
}

static VALUE
aligner_config_hash(VALUE self)
{
	EdlibAlignConfig *config = get_config(self);

	VALUE hash = rb_hash_new();

	rb_hash_aset(hash, ID2SYM(rb_intern("k")), get_k(config));
	rb_hash_aset(hash, ID2SYM(rb_intern("mode")), get_mode(config));
	rb_hash_aset(hash, ID2SYM(rb_intern("task")), get_task(config));
	rb_hash_aset(hash, ID2SYM(rb_intern("additional_equalities")), get_additional_equalities(config));

	return hash;
}

static VALUE
aligner_initialize(VALUE self, VALUE k, VALUE mode, VALUE task, VALUE additional_equalities)
{
	EdlibAlignConfig *config = get_config(self);

	config->k = NUM2INT(k);
	set_mode(config, mode);
	set_task(config, task);
	if (additional_equalities != Qnil)
	{
		set_additional_equalities(config, additional_equalities);
	}
	else
	{
		config->additionalEqualities = NULL;
		config->additionalEqualitiesLength = 0;
	}

	return self;
}

static VALUE
aligner_align(VALUE self, VALUE query, VALUE target)
{
	EdlibAlignConfig *config = get_config(self);
	if (!config)
	{
		rb_raise(rb_eRuntimeError, "config is NULL");
	}
	EdlibAlignConfig cfg = edlibNewAlignConfig(
		config->k,
		config->mode,
		config->task,
		config->additionalEqualities,
		config->additionalEqualitiesLength);

	EdlibAlignResult result = edlibAlign(
		StringValueCStr(query),
		RSTRING_LEN(query),
		StringValueCStr(target),
		RSTRING_LEN(target),
		cfg);

	if (result.status != 0)
	{
		rb_raise(rb_eRuntimeError, "edlibAlign failed");
	}

	VALUE edit_distance = INT2NUM(result.editDistance);
	VALUE alphabet_length = INT2NUM(result.alphabetLength);
	VALUE locations = rb_ary_new();
	VALUE alignment = rb_ary_new();
	VALUE cigar;

	int *el = result.endLocations;
	int *sl = result.startLocations;
	for (int i = 0; i < result.numLocations; i++)
	{
		VALUE ary = rb_ary_new();
		if (sl)
		{
			rb_ary_push(ary, INT2NUM(sl[i]));
		}
		else
		{
			rb_ary_push(ary, Qnil);
		}
		if (el)
		{
			rb_ary_push(ary, INT2NUM(el[i]));
		}
		else
		{
			rb_ary_push(ary, Qnil);
		}
		rb_ary_push(locations, ary);
	}

	for (int i = 0; i < result.alignmentLength; i++)
	{
		rb_ary_push(alignment, UINT2NUM(result.alignment[i]));
	}

	char *ccigar = edlibAlignmentToCigar(result.alignment, result.alignmentLength, 1); // EDLIB_CIGAR_EXTENDED
	cigar = rb_str_new2(ccigar);

	VALUE hash = rb_hash_new();
	rb_hash_aset(hash, ID2SYM(rb_intern("edit_distance")), edit_distance);
	rb_hash_aset(hash, ID2SYM(rb_intern("alphabet_length")), alphabet_length);
	rb_hash_aset(hash, ID2SYM(rb_intern("locations")), locations);
	rb_hash_aset(hash, ID2SYM(rb_intern("alignment")), alignment);
	rb_hash_aset(hash, ID2SYM(rb_intern("cigar")), cigar);

	edlibFreeAlignResult(result);

	return hash;
}

void Init_edlib(void)
{
	mEdlib = rb_define_module("Edlib");
	cAligner = rb_define_class_under(mEdlib, "Aligner", rb_cObject);
	rb_define_alloc_func(cAligner, config_allocate);
	rb_define_private_method(cAligner, "initialize_raw", aligner_initialize, 4);
	rb_define_method(cAligner, "k", aligner_get_k, 0);
	rb_define_method(cAligner, "k=", aligner_set_k, 1);
	rb_define_method(cAligner, "mode", aligner_get_mode, 0);
	rb_define_method(cAligner, "mode=", aligner_set_mode, 1);
	rb_define_method(cAligner, "task", aligner_get_task, 0);
	rb_define_method(cAligner, "task=", aligner_set_task, 1);
	rb_define_method(cAligner, "additional_equalities", aligner_get_additional_equalities, 0);
	rb_define_method(cAligner, "additional_equalities=", aligner_set_additional_equalities, 1);
	rb_define_method(cAligner, "config", aligner_config_hash, 0);
	rb_define_method(cAligner, "align", aligner_align, 2);
}
