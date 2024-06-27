#include "lib/Dialect/Tiny/TinyOps.h"

#include "mlir/Dialect/CommonFolders.h"
#include "mlir/Dialect/Complex/IR/Complex.h"
#include "mlir/IR/PatternMatch.h"

// Required after PatternMatch.h
// #include "lib/Dialect/Poly/TinyCanonicalize.cpp.inc"

namespace mlir {
namespace tinydialect {
namespace tiny {

OpFoldResult ConstantOp::fold(ConstantOp::FoldAdaptor adaptor) {
    return adaptor.getCoefficients();
}

OpFoldResult AddOp::fold(AddOp::FoldAdaptor adaptor) {
  auto lhs = dyn_cast_or_null<DenseIntElementsAttr>(adaptor.getOperands()[0]);
  auto rhs = dyn_cast_or_null<DenseIntElementsAttr>(adaptor.getOperands()[1]);

  if (!lhs || !rhs) return nullptr;

  if (lhs.getType() != rhs.getType()) return nullptr;

  // Assuming the tensors are ranked and sizes are defined.
  auto lhsType = lhs.getType().dyn_cast<RankedTensorType>();
  auto rhsType = rhs.getType().dyn_cast<RankedTensorType>();
  if (!lhsType || !rhsType || lhsType.getShape() != rhsType.getShape()) return nullptr;

  auto size = lhsType.getDimSize(0); // Assuming 1D tensors for simplicity.
  SmallVector<APInt, 8> result;
  result.reserve(size);

  // Initialize the resulting vector with all zeros
  for (int i = 0; i < size; ++i) {
    result.push_back(APInt(lhsType.getElementType().cast<IntegerType>().getWidth(), 0));
  }

  int i = 0;
  for (auto lhsIt = lhs.value_begin<APInt>(), rhsIt = rhs.value_begin<APInt>(); // returns an iterator to the first element for lhs and rhs, respectively
       lhsIt != lhs.value_end<APInt>() && rhsIt != rhs.value_end<APInt>(); // condition to check if we have reached the end of the tensor
       ++lhsIt, ++rhsIt) { // increment the iterators
    result[i] += *lhsIt + *rhsIt;
    ++i;
  }

  return DenseIntElementsAttr::get(
      RankedTensorType::get({size}, lhsType.getElementType()),
      result
  );
}

}  // namespace tiny
}  // namespace tinydialect
}  // namespace mlir
