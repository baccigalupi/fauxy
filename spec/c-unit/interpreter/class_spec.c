#include "helpers.h"

char *test_class_name() {
  spec_describe("class name");
  setup_interpreter();
  char *name = "MyClass";
  void *superclass = NULL;

  FxI_Class *klass = FxI_Class_create(interpreter, name, superclass);

  assert_strings_equal(fxi_class_name(klass), name, "is equal to init value");
  return NULL;
}

char *set_method_with_no_name_match() {
  spec_describe("set method with no group yet created");

  setup_interpreter();

  FxI_Object *klass = FxI_Object_create(interpreter, NULL);
  FxI_FunctionDefinition *function = fx_alloc(FxI_FunctionDefinition);
  fxi_class_set_method(klass, "my-call", function);

  assert_ints_equal(fxi_class_method_count(klass), 1, "has the right number of methods");
  FxI_MethodGroup *method_group = fxi_class_get_method_group(klass, "my-call");
  assert_ints_equal(
    fxi_method_group_length(method_group), 1, "method group has only one function"
  );

  return NULL;
}

char *set_method_with_name_match() {
  spec_describe("set method with group already created");

  setup_interpreter();
  FxI_Object *klass = FxI_Object_create(interpreter, NULL);

  FxI_FunctionDefinition *function_1 = fx_alloc(FxI_FunctionDefinition);
  FxI_FunctionDefinition *function_2 = fx_alloc(FxI_FunctionDefinition);

  fxi_class_set_method(klass, "my-call", function_1);
  fxi_class_set_method(klass, "my-call", function_2);

  assert_ints_equal(fxi_class_method_count(klass), 1, "has the right number of method groups");

  FxI_MethodGroup *method_group = fxi_class_get_method_group(klass, "my-call");

  assert_ints_equal(
    fxi_method_group_length(method_group), 2, "method group has the right number of functions"
  );

  return NULL;
}

char *all_specs() {
  spec_setup("Class");

  run_spec(test_class_name);
  run_spec(set_method_with_no_name_match);
  run_spec(set_method_with_name_match);

  spec_teardown();

  return NULL;
}

run_all_specs(all_specs);
