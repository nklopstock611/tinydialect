#include "lib/Dialect/Tiny/TinyDialect.h"

#include "lib/Dialect/Tiny/TinyOps.h"
// #include "lib/Dialect/Tiny/TinyTypes.h"
#include "mlir/IR/Builders.h"
#include "llvm/ADT/TypeSwitch.h"

#include "lib/Dialect/Tiny/TinyDialect.cpp.inc"
// #define GET_TYPEDEF_CLASSES
// #include "lib/Dialect/Tiny/TinyTypes.cpp.inc"
#define GET_OP_CLASSES
#include "lib/Dialect/Tiny/TinyOps.cpp.inc"

namespace mlir {
namespace tinydialect {
namespace tiny {

void TinyDialect::initialize() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "lib/Dialect/Tiny/TinyTypes.cpp.inc"
      >();
  addOperations<
#define GET_OP_LIST
#include "lib/Dialect/Tiny/TinyOps.cpp.inc"
      >();
}

Operation *TinyDialect::materializeConstant(OpBuilder &builder, Attribute value,
                                            Type type, Location loc) {
  auto coeffs = dyn_cast<DenseIntElementsAttr>(value);
  if (!coeffs)
    return nullptr;
  return builder.create<ConstantOp>(loc, type, coeffs);
}

} // namespace tiny
} // namespace tinydialect
} // namespace mlir