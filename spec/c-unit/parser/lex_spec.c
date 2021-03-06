#include "../../../src/parser/_parser.h"
#include "../../../src/parser/parse.tab.h"
#include "../../../src/parser/lex.yy.h"
#include "../lib/spec.h"

char *multiline_comment() {
  spec_describe("muliline comment");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "/*** and some more \n ** stuff ***/";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "no tokens found");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *trailing_comment() {
  spec_describe("muliline comment");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "//and some more \n123";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_LINE_END, "ignores comment and returns new line");
  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_INTEGER, "returns integer next line");
  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "total number of tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_operator_ids() {
  spec_describe("operators ids");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "* << - + ^ @ / \\";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "*");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "<<");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "-");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "+");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "^");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "@");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "/");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "\\");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_numeric_start() {
  spec_describe("ids with numbers");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "1* 123foo 1*1 foo1";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "1*");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "123foo");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "1*1");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "foo1");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_dashes_and_underscores() {
  spec_describe("ids with dashes and underscores");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "hello-world hello_world _foo -foo";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "hello-world");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "hello_world");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "_foo");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "-foo");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_ids_with_equals_signs() {
  spec_describe("ids with =");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "=3x make= foo=bar";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "=hello");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "make=");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "foo=bar");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_equality_operator() {
  spec_describe("= operator");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "3 = 3";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EQUALITY, "= token");

  token_type = yylex(bit, location, state.scanner);

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_ending_with_a_bang() {
  spec_describe("ids ending with !");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "do-it!";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "do-it!");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_ending_with_a_question_mark() {
  spec_describe("ids followed a ?");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "valid?";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "valid?");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_question_mark_standalone() {
  spec_describe("ids followed a space and ?");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "thing ?";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "thing");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "?");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_question_mark_assignment() {
  spec_describe("assignment to a ?");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "?:";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_ID, "?");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_COLON, ":");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *test_ids_with_caps_fail() {
  spec_describe("ids with capital letters anywhere are illegal");

  FxP_LexWrapper state;
  yylex_init(&state.scanner);
  char *operators = "helloWorld 123W #";
  YY_BUFFER_STATE buffer = yy_scan_string(operators, state.scanner);
  void *bit = fx_alloc(FxP_Bit);
  YYLTYPE *location = fx_alloc(YYLTYPE);

  int token_type;

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_LEX_ERROR_ILLEGAL_VARIABLE, "lex error: helloWorld");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_LEX_ERROR_ILLEGAL_VARIABLE, "lex error: 123W");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_LEX_ERROR_UNKNOWN_TOKEN, "lex error: #");

  token_type = yylex(bit, location, state.scanner);
  assert_ints_equal(token_type, TOKEN_EOF, "right number of total tokens");

  yylex_destroy(state.scanner);
  fx_pfree(location);
  return NULL;
}

char *all_specs() {
  spec_setup("Lexing");

  run_spec(multiline_comment);
  run_spec(trailing_comment);

  run_spec(test_operator_ids);
  run_spec(test_numeric_start);
  run_spec(test_dashes_and_underscores);
  run_spec(test_ids_with_equals_signs);
  run_spec(test_equality_operator);
  run_spec(test_ending_with_a_bang);
  run_spec(test_ending_with_a_question_mark);
  run_spec(test_question_mark_standalone);
  run_spec(test_question_mark_assignment);

  run_spec(test_ids_with_caps_fail);

  spec_teardown();

  return NULL;
}

run_all_specs(all_specs);
