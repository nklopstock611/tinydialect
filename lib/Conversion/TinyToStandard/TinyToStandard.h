#ifndef LIB_CONVERSION_TINYTOSTANDARD_TINYTOSTANDARD_H_
#define LIB_CONVERSION_TINYTOSTANDARD_TINYTOSTANDARD_H_

#include "mlir/Pass/Pass.h"  // from @llvm-project

// Extra includes needed for dependent dialects
#include "mlir/Dialect/Arith/IR/Arith.h"   // from @llvm-project
#include "mlir/Dialect/Tensor/IR/Tensor.h"  // from @llvm-project

namespace mlir {
namespace tinydialect {
namespace tiny {

#define GEN_PASS_DECL
#include "lib/Conversion/TinyToStandard/TinyToStandard.h.inc"

#define GEN_PASS_REGISTRATION
#include "lib/Conversion/TinyToStandard/TinyToStandard.h.inc"

}  // namespace tiny
}  // namespace tinydialect
}  // namespace mlir

#endif  // LIB_CONVERSION_TINYTOSTANDARD_TINYTOSTANDARD_H_