#ifndef LIB_DIALECT_TINY_TINYOPS_H_
#define LIB_DIALECT_TINY_TINYOPS_H_

#include "lib/Dialect/Tiny/TinyDialect.h"
// #include "lib/Dialect/Tiny/TinyTraits.h"
// #include "lib/Dialect/Tiny/TinyTypes.h"
#include "mlir/Interfaces/InferTypeOpInterface.h" // from @llvm-project
#include "mlir/IR/BuiltinOps.h"      // from @llvm-project
#include "mlir/IR/BuiltinTypes.h"    // from @llvm-project
#include "mlir/IR/Dialect.h"         // from @llvm-project

#define GET_OP_CLASSES
#include "lib/Dialect/Tiny/TinyOps.h.inc"

#endif // LIB_DIALECT_TINY_TINYOPS_H_