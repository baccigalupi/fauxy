#include "pool.h"

FxI_Pool *FxI_Pool_create(int literal_capacity, int global_capacity, int lookup_capacity) {
  FxB_HashMap *literals = NULL;
  FxB_List    *contexts = NULL;
  FxB_HashMap *globals  = NULL;

  FxI_Pool *pool = fx_alloc(FxI_Pool);
  verify_memory(pool);

  // defaults for size
  literal_capacity =  literal_capacity ? literal_capacity : FXI_POOL_LITERAL_CAPACITY_DEFAULT;
  global_capacity =   global_capacity ? global_capacity :  FXI_POOL_CLASS_CAPACITY_DEFAULT;

  literals = FxB_HashMap_create(literal_capacity);
  verify(literals);
  fxi_pool_literals(pool) = literals;

  contexts = FxB_List_create();
  verify(contexts)
  fxi_pool_contexts(pool) = contexts;

  globals = FxB_HashMap_create(global_capacity);
  verify(globals);
  fxi_pool_contexts_push(pool, globals);

  return pool;
error:
  if (pool) { fx_pfree(pool); }
  if (contexts) { fxb_list_free(literals); }
  if (globals) { fxb_hash_map_free(globals); }

  return NULL;
}
