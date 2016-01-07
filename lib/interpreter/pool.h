#ifndef __FxI_Pool
#define __FxI_Pool 1

#include "../bricks/hash_map.h"
#include "../bricks/list.h"
#include "../bricks/helpers.h"

// These are the defaults for setting up pool sizing
// Eventually, I want to have this configurable via a config.json file
// that is passed to the `fauxy` command line binary
#define FXI_POOL_LITERAL_CAPACITY_DEFAULT  400
#define FXI_POOL_GLOBAL_CAPACITY_DEFAULT   200
#define FXI_POOL_NATIVE_CAPACITY_DEFAULT   1000

// Garbage managment: ??
//   * New allocations end up in the literals
//     for stuff that is part of the source code
//   * Otherwise it ends up in 'all', which is a list
//   * Global assignments go into the 'globals' hash
//
// Generational-ish-ness:
//   * Things that survive a generation can be moved to the back of the list
//   * Thing that are known to go out of scope can be bumped to the front
//   * Algorithm that figures out how far to go into list
//   * All of this collection happens in a background event loop?
//
// Defragmentation ... not really covered, for now.

#include "structs.h"

#define fxi_pool_literals(P)        ((P)->literals)
#define fxi_pool_all(P)             ((P)->all)
#define fxi_pool_globals(P)         ((P)->globals)
#define fxi_pool_natives(P)         ((P)->natives)

#define fxi_pool_literal_get(P, K)      (fxb_hash_map_get(fxi_pool_literals(P), K))
#define fxi_pool_literal_set(P, K, V)   (fxb_hash_map_set(fxi_pool_literals(P), K, V))

#define fxi_pool_global_get(P, K)       (fxi_object_get_attribute(fxi_pool_globals(P), K))
#define fxi_pool_global_set(P, K, V)    (fxi_object_set_attribute(fxi_pool_globals(P), K, V))

#define fxi_pool_free(P)            (fx_pfree(P)) // TODO: more better

FxI_Pool *FxI_Pool_create(FxB_HashMap *config);

#endif
