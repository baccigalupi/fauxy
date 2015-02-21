#include "helpers.h"

char *set_method_with_no_name_match() {
  spec_describe("set method with no group yet created");

  FxN_Class *klass = FxN_Class_create("MyClass", NULL);
  FxN_Function *function = fx_alloc(FxN_Function);
  fxn_class_set_method(klass, "my-call", function);

  assert_ints_equal(fxn_class_method_count(klass), 1, "has the right number of methods");
  FxN_MethodGroup *method_group = fxn_class_get_method_group(klass, "my-call");
  assert_ints_equal(
    fxn_method_group_length(method_group), 1, "method group has only one function"
  );

  return NULL;
}

char *set_method_with_name_match() {
  spec_describe("set method with group already created");

  FxN_Class *klass = FxN_Class_create("MyClass", NULL);

  FxN_Function *function_1 = fx_alloc(FxN_Function);
  FxN_Function *function_2 = fx_alloc(FxN_Function);

  fxn_class_set_method(klass, "my-call", function_1);
  fxn_class_set_method(klass, "my-call", function_2);

  assert_ints_equal(fxn_class_method_count(klass), 1, "has the right number of method groups");

  FxN_MethodGroup *method_group = fxn_class_get_method_group(klass, "my-call");

  assert_ints_equal(
    fxn_method_group_length(method_group), 2, "method group has the right number of functions"
  );

  return NULL;
}

char *all_specs() {
  spec_setup("Class");

  run_spec(set_method_with_no_name_match);
  run_spec(set_method_with_name_match);

  spec_teardown();

  return NULL;
}

run_all_specs(all_specs);
