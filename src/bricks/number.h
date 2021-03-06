#ifndef __FxB_Number_h
#define __FxB_Number_h 1

#include "helpers.h"
#include "string.h"

typedef struct FxB_Number {
  int type;
  Boolean exponent;
  void *value;
} FxB_Number;

#define fxb_number_type(I)         ((I)->type)
#define fxb_number_value(I)        ((I)->value)
#define fxb_number_exponent(I)     ((I)->exponent)

#define fxb_number_value_integer(I)  (*(long long   *)(fxb_number_value(I)))
#define fxb_number_value_decimal(I)  (*(double      *)(fxb_number_value(I)))

#define fxb_number_is_decimal(I)    (                                               \
                                      (fxb_number_type(I) == FX_DECIMAL_DOUBLE) || \
                                      (fxb_number_type(I) == FX_DECIMAL_LDOUBLE)   \
                                    )

#define fxb_number_free_husk(I)    (fx_pfree(I))

#define fxb_number_create_value(I, TYPE)    TYPE *value = fx_alloc(TYPE); \
                                            verify_memory(value);         \
                                            *value = (TYPE)number_value;  \
                                            fxb_number_value(I) = value;
#include "../types.h"

FxB_Number *FxB_Number_create();
FxB_Number *FxB_Integer_from_string(char *str);
FxB_Number *FxB_Decimal_from_string(char *str);
FxB_Number *FxB_Exponent_from_string(char *str);
FxB_String *fxb_integer_inspect(FxB_Number *number);
FxB_String *fxb_decimal_inspect(FxB_Number *number);
FxB_String *fxb_decimal_full_inspect(FxB_Number *number);
FxB_String *fxb_number_inspect(FxB_Number *number);
FxB_String *fxb_number_full_inspect(FxB_Number *number);
FxB_String *fxb_number_type_description(FxB_Number *number);

#endif
