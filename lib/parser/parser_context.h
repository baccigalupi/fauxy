#ifndef __FxParserContext
#define __FxParserContext 1

#include "../bricks/list.h"

typedef List FxParserContext;

#include "expressions.h"

#define fx_parser_context_push(C, V)    list_push(C, V)
#define fx_parser_context_pop(C)        ((FxExpressions *)list_pop(C, V))
#define fx_parser_context_current(C)    ((FxExpressions *)list_last(C))

FxParserContext *FxParserContext_create();

#endif