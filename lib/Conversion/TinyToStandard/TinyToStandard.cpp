#include "lib/Conversion/TinyToStandard/TinyToStandard.h"

#include "lib/Dialect/Tiny/TinyOps.h"
#include "llvm/ADT/SmallVector.h"          // from @llvm-project
#include "mlir/Dialect/SCF/IR/SCF.h"                    // from @llvm-project
#include "mlir/Dialect/Func/IR/FuncOps.h"  // from @llvm-project
#include "mlir/Dialect/Func/Transforms/FuncConversions.h"  // from @llvm-project
#include "mlir/IR/ImplicitLocOpBuilder.h"  // from @llvm-project
#include "mlir/Transforms/DialectConversion.h"  // from @llvm-project

namespace mlir {
namespace tinydialect {
namespace tiny {

#define GEN_PASS_DEF_POLYTOSTANDARD
#include "lib/Conversion/TinyToStandard/TinyToStandard.h.inc"

// class TinyToStandardTypeConverter : public TypeConverter {
//  public:
//   TinyToStandardTypeConverter(MLIRContext *ctx) {
//     addConversion([](Type type) { return type; });
//     addConversion([ctx](TinynomialType type) -> Type {
//       int degreeBound = type.getDegreeBound();
//       IntegerType elementTy =
//           IntegerType::get(ctx, 32, IntegerType::SignednessSemantics::Signless);
//       return RankedTensorType::get({degreeBound}, elementTy);
//     });
//   }
// };

// struct ConvertAppOp
// struct ConvertMulOp
// struct ConvertConstant

struct TinyToStandard : impl::TinyToStandardBase<TinyToStandard> {
  using TinyToStandardBase::TinyToStandardBase;

  void runOnOperation() override {
    MLIRContext *context = &getContext();
    auto *module = getOperation();

    ConversionTarget target(*context);
    target.addLegalDialect<arith::ArithDialect>();
    target.addIllegalDialect<TinyDialect>();

    RewritePatternSet patterns(context);
    // TinyToStandardTypeConverter typeConverter(context);
    patterns.add<ConvertAdd, ConvertConstant, ConvertMul>(typeConverter, context);

    populateFunctionOpInterfaceTypeConversionPattern<func::FuncOp>(
        patterns, typeConverter);
    target.addDynamicallyLegalOp<func::FuncOp>([&](func::FuncOp op) {
      return typeConverter.isSignatureLegal(op.getFunctionType()) &&
             typeConverter.isLegal(&op.getBody());
    });

    populateReturnOpTypeConversionPattern(patterns, typeConverter);
    target.addDynamicallyLegalOp<func::ReturnOp>(
        [&](func::ReturnOp op) { return typeConverter.isLegal(op); });

    populateCallOpTypeConversionPattern(patterns, typeConverter);
    target.addDynamicallyLegalOp<func::CallOp>(
        [&](func::CallOp op) { return typeConverter.isLegal(op); });

    populateBranchOpInterfaceTypeConversionPattern(patterns, typeConverter);
    target.markUnknownOpDynamicallyLegal([&](Operation *op) {
      return isNotBranchOpInterfaceOrReturnLikeOp(op) ||
             isLegalForBranchOpInterfaceTypeConversionPattern(op,
                                                              typeConverter) ||
             isLegalForReturnOpTypeConversionPattern(op, typeConverter);
    });

    if (failed(applyPartialConversion(module, target, std::move(patterns)))) {
      signalPassFailure();
    }
  }
};

}  // namespace tiny
}  // namespace tinydialect
}  // namespace mlir