#include "ruby.h"
#include "edlibext.h"

#define ALIGNER_GET_(name)                               \
	static VALUE                                           \
			aligner_get_##name(VALUE self)                     \
	{                                                      \
		EdlibAlignConfig *config = aligner_get_config(self); \
		return get_##name(config);                           \
	}

#define ALIGNER_SET_(name)                               \
	static VALUE                                           \
			aligner_set_##name(VALUE self, VALUE value)        \
	{                                                      \
		EdlibAlignConfig *config = aligner_get_config(self); \
		return set_##name(config, value);                    \
	}

VALUE mEdlib;
VALUE cAligner;

// Aligner class

static size_t aligner_config_memsize(const void *ptr);
static void aligner_config_free(void *ptr);

static const rb_data_type_t config_type = {
		.wrap_struct_name = "RbAlignConfig",
		.function = {
				.dfree = aligner_config_free,
				.dsize = aligner_config_memsize,
		},
		.flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
aligner_config_allocate(VALUE klass)
{
	RbAlignConfig *aligner_config;

	VALUE obj = TypedData_Make_Struct(klass, RbAlignConfig, &config_type, aligner_config);
	aligner_config->config = (EdlibAlignConfig *)malloc(sizeof(EdlibAlignConfig));
	aligner_config->equalityPairs = NULL;
	return obj;
}

static void
aligner_config_free(void *ptr)
{
	RbAlignConfig *aligner_config = ptr;
	if (aligner_config->config != NULL)
	{
		free(aligner_config->config);
	}
	if (aligner_config->equalityPairs != NULL)
	{
		free(aligner_config->equalityPairs);
	}

	free(ptr);
}

static size_t
aligner_config_memsize(const void *ptr)
{
	const RbAlignConfig *aligner_config = ptr;
	return sizeof(ptr) + sizeof(aligner_config->config) + 2 * sizeof(char) * aligner_config->config->additionalEqualitiesLength;
}

static EdlibAlignConfig *
aligner_get_config(VALUE self)
{
	RbAlignConfig *aligner_config = NULL;
	TypedData_Get_Struct(self, RbAlignConfig, &config_type, aligner_config);
	return aligner_config->config;
}

static EdlibEqualityPair *
aligner_get_equalityPairs(VALUE self)
{
	RbAlignConfig *aligner_config = NULL;
	TypedData_Get_Struct(self, RbAlignConfig, &config_type, aligner_config);
	return aligner_config->equalityPairs;
}

// Config

static VALUE
get_k(EdlibAlignConfig *config)
{
	return INT2NUM(config->k);
}

ALIGNER_GET_(k)

static VALUE
set_k(EdlibAlignConfig *config, VALUE k)
{
	config->k = NUM2INT(k);
	return k;
}

ALIGNER_SET_(k)

static VALUE
get_mode(EdlibAlignConfig *config)
{
	switch (config->mode)
	{
	case EDLIB_MODE_NW:
		return rb_str_new2("NW");
	case EDLIB_MODE_SHW:
		return rb_str_new2("SHW");
	case EDLIB_MODE_HW:
		return rb_str_new2("HW");
	default:
		return Qnil;
	}
}

ALIGNER_GET_(mode)

static VALUE
set_mode(EdlibAlignConfig *config, VALUE mode)
{
	if (TYPE(mode) == T_SYMBOL)
	{
		mode = rb_String(mode);
	}
	switch (TYPE(mode))
	{
	case T_STRING:;
		VALUE mode_str = rb_funcall(mode, rb_intern("upcase"), 0);
		char *mode_s = RSTRING_PTR(mode_str);
		if (strcmp(mode_s, "NW") == 0)
		{
			config->mode = EDLIB_MODE_NW;
		}
		else if (strcmp(mode_s, "SHW") == 0)
		{
			config->mode = EDLIB_MODE_SHW;
		}
		else if (strcmp(mode_s, "HW") == 0)
		{
			config->mode = EDLIB_MODE_HW;
		}
		else
		{
			rb_raise(rb_eArgError, "Invalid mode");
		}
		break;
	case T_FIXNUM:;
		int m = NUM2INT(mode);
		if (m < 0 || m > 2)
		{
			rb_raise(rb_eArgError, "Invalid mode");
		}
		config->mode = m;
		break;
	default:
		rb_raise(rb_eArgError, "Invalid mode");
	}
	return mode;
}

ALIGNER_SET_(mode)

static VALUE
get_task(EdlibAlignConfig *config)
{
	switch (config->task)
	{
	case EDLIB_TASK_DISTANCE:
		return rb_str_new2("DISTANCE");
	case EDLIB_TASK_LOC:
		return rb_str_new2("LOC");
	case EDLIB_TASK_PATH:
		return rb_str_new2("PATH");
	default:
		return Qnil;
	}
}

ALIGNER_GET_(task)

static VALUE
set_task(EdlibAlignConfig *config, VALUE task)
{
	if (TYPE(task) == T_SYMBOL)
	{
		task = rb_String(task);
	}
	switch (TYPE(task))
	{
	case T_STRING:;
		VALUE task_str = rb_funcall(task, rb_intern("upcase"), 0);
		char *task_s = RSTRING_PTR(task_str);
		if (strcmp(task_s, "DISTANCE") == 0)
		{
			config->task = EDLIB_TASK_DISTANCE;
		}
		else if (strcmp(task_s, "LOC") == 0)
		{
			config->task = EDLIB_TASK_LOC;
		}
		else if (strcmp(task_s, "PATH") == 0)
		{
			config->task = EDLIB_TASK_PATH;
		}
		else
		{
			rb_raise(rb_eArgError, "Invalid task");
		}
		break;
	case T_FIXNUM:;
		int t = NUM2INT(task);
		if (t < 0 || t > 2)
		{
			rb_raise(rb_eArgError, "Invalid task");
		}
		config->task = t;
		break;
	default:
		rb_raise(rb_eArgError, "Invalid task");
	}
	return task;
}

ALIGNER_SET_(task)

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

ALIGNER_GET_(additional_equalities)

static VALUE
set_additional_equalities(EdlibAlignConfig *config, EdlibEqualityPair *eqpairs, VALUE equalities)
{
	Check_Type(equalities, T_ARRAY);
	size_t len = RARRAY_LEN(equalities);
	if (len == 0)
	{
		if (eqpairs != NULL)
		{
			free(eqpairs);
			eqpairs = NULL;
		}
		config->additionalEqualities = NULL;
		config->additionalEqualitiesLength = 0;
		return equalities;
	}

	// Check if len is too large
	if (len > SIZE_MAX / sizeof(EdlibEqualityPair))
	{
		rb_raise(rb_eArgError, "Requested array is too large");
	}

	char *first_arr = malloc(len * sizeof(char));
	char *second_arr = malloc(len * sizeof(char));
	if (first_arr == NULL || second_arr == NULL)
	{
		if (first_arr != NULL)
			free(first_arr);
		if (second_arr != NULL)
			free(second_arr);
		rb_raise(rb_eNoMemError, "Failed to allocate memory for equality pairs");
	}

	for (size_t i = 0; i < len; i++)
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
		first_arr[i] = c1;
		second_arr[i] = c2;
	}

	if (eqpairs != NULL)
	{
		free(eqpairs);
	}

	eqpairs = (EdlibEqualityPair *)malloc(sizeof(EdlibEqualityPair) * len);

	for (size_t i = 0; i < len; i++)
	{
		eqpairs[i].first = first_arr[i];
		eqpairs[i].second = second_arr[i];
	}

	config->additionalEqualities = eqpairs;
	config->additionalEqualitiesLength = len;

	free(first_arr);
	free(second_arr);

	return equalities;
}

static VALUE
aligner_set_additional_equalities(VALUE self, VALUE equalities)
{
	EdlibAlignConfig *config = aligner_get_config(self);
	EdlibEqualityPair *eqpairs = aligner_get_equalityPairs(self);
	return set_additional_equalities(config, eqpairs, equalities);
}

static VALUE
aligner_config_hash(VALUE self)
{
	EdlibAlignConfig *config = aligner_get_config(self);

	VALUE hash = rb_hash_new();

	rb_hash_aset(hash, ID2SYM(rb_intern("k")), get_k(config));
	rb_hash_aset(hash, ID2SYM(rb_intern("mode")), get_mode(config));
	rb_hash_aset(hash, ID2SYM(rb_intern("task")), get_task(config));
	rb_hash_aset(hash, ID2SYM(rb_intern("additional_equalities")), get_additional_equalities(config));

	return hash;
}

static VALUE
aligner_initialize_raw(VALUE self, VALUE k, VALUE mode, VALUE task, VALUE additional_equalities)
{
	EdlibAlignConfig *config = aligner_get_config(self);
	EdlibEqualityPair *eqpairs = aligner_get_equalityPairs(self);
	config->k = NUM2INT(k);
	set_mode(config, mode);
	set_task(config, task);

	if (additional_equalities != Qnil)
	{
		set_additional_equalities(config, eqpairs, additional_equalities);
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
	EdlibAlignConfig *config = aligner_get_config(self);
	if (!config)
	{
		rb_raise(rb_eRuntimeError, "config is NULL");
	}

	EdlibAlignResult result = edlibAlign(
			StringValueCStr(query),
			RSTRING_LEN(query),
			StringValueCStr(target),
			RSTRING_LEN(target),
			*config);

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

void Init_edlibext(void)
{
	mEdlib = rb_define_module("Edlib");
	cAligner = rb_define_class_under(mEdlib, "Aligner", rb_cObject);
	rb_define_alloc_func(cAligner, aligner_config_allocate);
	rb_define_private_method(cAligner, "initialize_raw", aligner_initialize_raw, 4);
	rb_define_method(cAligner, "k", aligner_get_k, 0);
	rb_define_method(cAligner, "k=", aligner_set_k, 1);
	rb_define_method(cAligner, "mode", aligner_get_mode, 0);
	rb_define_method(cAligner, "mode=", aligner_set_mode, 1);
	rb_define_method(cAligner, "task", aligner_get_task, 0);
	rb_define_method(cAligner, "task=", aligner_set_task, 1);
	rb_define_method(cAligner, "additional_equalities", aligner_get_additional_equalities, 0);
	rb_define_method(cAligner, "additional_equalities=", aligner_set_additional_equalities, 1);
	rb_define_method(cAligner, "config", aligner_config_hash, 0);
	rb_define_method(cAligner, "align_raw", aligner_align, 2);
}
