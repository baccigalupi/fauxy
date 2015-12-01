#include "helpers.h"

char *test_interpet_literal_true() {
  spec_describe("literal evaluation: 'true' returns true object");
  setup_interpreter();

  FxP_Literal *literal = FxP_Literal_create(NULL, TOKEN_TRUE);
  FxN_Object *object = fxi_evaluate(interpreter, literal);

  assert_truthy(fxn_boolean_value(object) == true,  "returned object is true");
  /*assert_truthy(,  "returned object is true");*/
  // assert that object has the right class

  FxN_Object *stored_object = fxi_literal_get(interpreter, TRUE_KEY);
  assert_truthy(fxn_boolean_value(object) == true, "literal is set in the interpreter");

  assert_equal(object, stored_object, "literal returned is same as one stored in the interpreter");

  fxi_interpreter_free(interpreter);

  return NULL;
}

char *test_interpet_literal_false() {
  spec_describe("literal evaluation: 'false' returns false object");
  setup_interpreter();

  FxP_Literal *literal = FxP_Literal_create(NULL, TOKEN_FALSE);
  FxN_Object *object = fxi_evaluate(interpreter, literal);

  assert_truthy(fxn_boolean_value(object) == false,  "returned object is false");
  // assert that object has the right class

  FxN_Object *stored_object = fxi_literal_get(interpreter, FALSE_KEY);
  assert_truthy(fxn_boolean_value(object) == false, "literal is set in the interpreter");

  assert_equal(object, stored_object, "literal returned is same as one stored in the interpreter");

  fxi_interpreter_free(interpreter);

  return NULL;
}

char *test_interpet_literal_integer() {
  spec_describe("literal evaluation: integer literal return an object");
  setup_interpreter();

  FxP_Bit *bit = FxP_Bit_integer_create("12");
  FxP_Literal *literal = FxP_Literal_create(bit, TOKEN_INTEGER);
  FxN_Object *object = fxi_evaluate(interpreter, literal);

  assert_equal(fxi_object_value_short(object), (short)12, "returned the right number");
  // assert that object has the right class

  FxN_Object *stored_object = fxi_literal_get(interpreter, fxi_literal_key(literal));
  assert_equal(object, stored_object, "literal returned is same as one stored in the interpreter");

  fxi_interpreter_free(interpreter);

  return NULL;
}

char *test_interpet_literal_decimal() {
  spec_describe("literal evaluation: decimal literal returns an object");
  setup_interpreter();

  FxP_Bit *bit = FxP_Bit_decimal_create("1.2");
  FxP_Literal *literal = FxP_Literal_create(bit, TOKEN_FLOAT);
  FxN_Object *object = fxi_evaluate(interpreter, literal);

  assert_equal(fxi_object_value_double(object), (double)1.2, "returned the right number");
  // assert that object has the right class

  assert_ints_equal(fxi_interpreter_literal_length(interpreter), 2, "decimal was not added to the interpreter pool");

  fxi_interpreter_free(interpreter);

  return NULL;
}

char *test_interpet_literal_string() {
  spec_describe("literal evaluation: string literal returns an object");
  setup_interpreter();

  FxP_Bit *bit = FxP_Bit_string_create("hello world");
  FxP_Literal *literal = FxP_Literal_create(bit, TOKEN_STRING);
  FxN_Object *object = fxi_evaluate(interpreter, literal);

  assert_strings_equal(fxb_string_value(fxi_object_value_string(object)), "hello world", "returned the right string");
  // assert that object has the right class

  FxN_Object *stored_object = fxi_literal_get(interpreter, fxi_literal_key(literal));
  assert_equal(object, stored_object, "literal returned is same as one stored in the interpreter");

  fxi_interpreter_free(interpreter);

  return NULL;
}

char *test_global_assignment_of_literal() {
  spec_describe("literal lookup evaluation: assigning literal to the top context and then retrieving it");
  setup_interpreter();

  FxP_Bit     *value_bit =      FxP_Bit_string_create("Hello Fauxy world!");
  FxP_Literal *value =          FxP_Literal_create(value_bit, TOKEN_STRING);

  FxN_Object  *value_object =   fxi_evaluate(interpreter, value);

  FxP_Bit     *lookup_bit =     FxP_Bit_string_create("greeting");
  FxP_Lookup  *lookup =         FxP_Lookup_create(lookup_bit, TOKEN_ID);

  FxP_Expression *assignment =  FxP_ColonExpression_create(lookup, value);

  FxN_Object   *value_dup =     fxi_evaluate(interpreter, assignment);

  assert_equal(value_dup, value_object, "evaluation of assignment returns what is assigned to it");
  FxN_Object *evaluation = fxi_evaluate(interpreter, lookup);
  assert_equal(evaluation, value_object, "lookup of the id returns the object");

  return NULL;
}

char *test_context_assignment_of_literal() {
  spec_describe("lookup evaluation: literal in current context (non-global)");
  setup_interpreter();

  FxN_Object *object = FxN_Object_create(interpreter, NULL);
  fxi_interpreter_push_context(interpreter, object);

  FxP_Bit     *value_bit =      FxP_Bit_string_create("Hello Fauxy world!");
  FxP_Literal *value =          FxP_Literal_create(value_bit, TOKEN_STRING);

  FxN_Object  *value_object =   fxi_evaluate(interpreter, value);

  FxP_Bit     *lookup_bit =     FxP_Bit_string_create("greeting");
  FxP_Lookup  *lookup =         FxP_Lookup_create(lookup_bit, TOKEN_ID);

  FxP_Expression *assignment =  FxP_ColonExpression_create(lookup, value);

  FxN_Object   *assign_return_value = fxi_evaluate(interpreter, assignment);

  assert_equal(assign_return_value, value_object, "evaluation of assignment returns what is assigned to it");

  FxN_Object *attr = fxn_object_get_attribute(object, "greeting");
  assert_equal(attr, value_object, "literal is stored in context");

  FxN_Object *evaluation = fxi_evaluate(interpreter, lookup);
  assert_equal(evaluation, value_object, "lookup of the id in context returns the object");

  return NULL;
}

char *test_global_assignment_but_lookup_from_context() {
  spec_describe("lookup evaluation: global level literal from context");
  setup_interpreter();

  FxN_Object *object = FxN_Object_create(interpreter, NULL);

  FxP_Bit     *value_bit =      FxP_Bit_string_create("Hello Fauxy world!");
  FxP_Literal *value =          FxP_Literal_create(value_bit, TOKEN_STRING);

  FxN_Object  *value_object =   fxi_evaluate(interpreter, value);

  FxP_Bit     *lookup_bit =     FxP_Bit_string_create("greeting");
  FxP_Lookup  *lookup =         FxP_Lookup_create(lookup_bit, TOKEN_ID);

  FxP_Expression *assignment =  FxP_ColonExpression_create(lookup, value);

  FxN_Object   *assign_return_value = fxi_evaluate(interpreter, assignment);

  fxi_interpreter_push_context(interpreter, object);

  FxN_Object *evaluation = fxi_evaluate(interpreter, lookup);
  assert_equal(evaluation, value_object, "lookup of the id bypasses context to find it higher up");

  return NULL;
}

char *test_function_declaration() {
  spec_describe("function definition evaluation: returns function definition");
  setup_interpreter();

  FxP_Literal *literal = FxP_Literal_create(NULL, TOKEN_TRUE);

  // expressions
  FxP_Expressions *expressions = FxP_Expressions_create();
  fxp_expression_push(expressions, literal);
  // function definition setup
  FxP_FunctionDefinition *function_definition = FxP_FunctionDefinition_create_no_args();
  fxp_function_definition_set_expressions(function_definition, expressions);

  FxN_Object *evaluation = fxi_evaluate(interpreter, function_definition);
  assert_equal(fxn_object_value(evaluation), function_definition, "returns an object with the expression stored in the value");
  // TODO: assert about the class/type

  return NULL;
}

char *test_import_expression_on_global_space() {
  spec_describe("import expression adds to global namespace");
  setup_interpreter();

  // currently path is from root run location, not relative to her or file require
  FxP_Bit *bit                              = FxP_Bit_string_create("spec/c-unit/interpreter/fixtures/import-test.fx");
  FxP_Literal *literal                      = FxP_Literal_create(bit, TOKEN_STRING);
  FxP_ImportExpression *import_expression   = FxP_ImportExpression_create(literal);

  FxN_Object *object = fxi_evaluate(interpreter, import_expression);
  assert_equal(fxn_boolean_value(object), true, "returns the last expression return value from the import");

  bit = FxP_Bit_string_create("it-worked?");
  FxP_Lookup *lookup = FxP_Lookup_create(bit, TOKEN_ID);
  object = fxi_evaluate(interpreter, lookup);

  assert_equal(fxn_boolean_value(object), true, "import code run against current interpreter context");

  return NULL;
}

char *all_specs() {
  spec_setup("Interpreter");

  run_spec(test_interpet_literal_true);
  run_spec(test_interpet_literal_false);
  run_spec(test_interpet_literal_integer);
  run_spec(test_interpet_literal_decimal);
  run_spec(test_interpet_literal_string);

  run_spec(test_global_assignment_of_literal);
  run_spec(test_context_assignment_of_literal);
  run_spec(test_global_assignment_but_lookup_from_context);

  run_spec(test_function_declaration);

  // can't run this until instance assignment is defined
  /*run_spec(test_import_expression_on_global_space);*/

  spec_teardown();

  return NULL;
}

run_all_specs(all_specs);
