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

#define GEN_PASS_DEF_TINYTOSTANDARD
#include "lib/Conversion/TinyToStandard/TinyToStandard.h.inc"

/*
 * struct ConvertMulOp -> this should replace a tiny.add op to a loop that uses arith.add
 * to add each pair of elements in the vectors.
*/
struct ConvertAdd : public OpConversionPattern<AddOp> {
  ConvertAdd(mlir::MLIRContext *context) : OpConversionPattern<AddOp>(context) {}

  LogicalResult matchAndRewrite(AddOp op, OpAdaptor adaptor, ConversionPatternRewriter &rewriter) const override {
    llvm::errs() << "Lowering AddOp...\n";
    auto tinyAddTensorType = mlir::dyn_cast<RankedTensorType>(adaptor.getLhs().getType());

    auto numTerms = tinyAddTensorType.getShape()[0];
    ImplicitLocOpBuilder b(op.getLoc(), rewriter);

    // creates an all-zeros tensor to store the resulting tensor
    auto elementType = tinyAddTensorType.getElementType();
    auto zeroTensor = b.create<arith::ConstantOp>(
      tinyAddTensorType,
      DenseElementsAttr::get(
        tinyAddTensorType,
        llvm::SmallVector<APInt, 8>(numTerms, APInt(elementType.getIntOrFloatBitWidth(), 0))
      )
    );

    // creates constants for the loop
    auto lowerBound = b.create<arith::ConstantOp>(b.getIndexType(), b.getIndexAttr(0));
    auto numTermsOp = b.create<arith::ConstantOp>(b.getIndexType(), b.getIndexAttr(numTerms));
    auto step = b.create<arith::ConstantOp>(b.getIndexType(), b.getIndexAttr(1));
    
    auto lhs = adaptor.getLhs();
    auto rhs = adaptor.getRhs();

    auto loop = b.create<scf::ForOp>(
      lowerBound, numTermsOp, step, ValueRange(zeroTensor.getResult()),
      [&](OpBuilder &builder, Location loc, Value index, ValueRange loopState) {
        ImplicitLocOpBuilder b(op.getLoc(), builder);
        auto accumTensor = loopState.front();

        // extracts the elements from the input tensors
        auto lhsElement = b.create<tensor::ExtractOp>(lhs, index);
        auto rhsElement = b.create<tensor::ExtractOp>(rhs, index);

        // adds the elements
        auto sum = b.create<arith::AddIOp>(lhsElement, rhsElement);

        // stores the result in the accumulator tensor
        auto resultTensor = b.create<tensor::InsertOp>(sum, accumTensor, index);
        // says that the result of the current iteration is the accumulator tensor for the next iteration
        // makes the accumTensor for the next iteration the result of the current iteration (resultTensor)

        // yields the accumulator tensor
        b.create<scf::YieldOp>(resultTensor.getResult()); // stablishes resultTensor as the next accumTensor
      } 
    );

    rewriter.replaceOp(op, loop.getResult(0));
    return success();
  }
};

/*
 * struct ConvertMulOp -> this should replace a tiny.mul op to a loop that uses arith.mul
 * to multiply each pair of elements in the vectors.
*/
struct ConvertMul : public OpConversionPattern<MulOp> {
  ConvertMul(mlir::MLIRContext *context) : OpConversionPattern<MulOp>(context) {}

  LogicalResult matchAndRewrite(MulOp op, OpAdaptor adaptor, ConversionPatternRewriter &rewriter) const override {
  auto tinyMulTensorType = mlir::dyn_cast<RankedTensorType>(adaptor.getLhs().getType());

    auto numTerms = tinyMulTensorType.getShape()[0];
    ImplicitLocOpBuilder b(op.getLoc(), rewriter);

    // creates an all-zeros tensor to store the resulting tensor
    auto elementType = tinyMulTensorType.getElementType();
    auto zeroTensor = b.create<arith::ConstantOp>(
      tinyMulTensorType,
        DenseElementsAttr::get(
          tinyMulTensorType,
          llvm::SmallVector<APInt, 8>(numTerms, APInt(elementType.getIntOrFloatBitWidth(), 0))
      )
    );

    // creates constants for the loop
    auto lowerBound = b.create<arith::ConstantOp>(b.getIndexType(), b.getIndexAttr(0));
    auto numTermsOp = b.create<arith::ConstantOp>(b.getIndexType(), b.getIndexAttr(numTerms));
    auto step = b.create<arith::ConstantOp>(b.getIndexType(), b.getIndexAttr(1));
    
    auto lhs = adaptor.getLhs();
    auto rhs = adaptor.getRhs();

    auto loop = b.create<scf::ForOp>(
      lowerBound, numTermsOp, step, ValueRange(zeroTensor.getResult()),
      [&](OpBuilder &builder, Location loc, Value index, ValueRange loopState) {
        ImplicitLocOpBuilder b(op.getLoc(), builder);
        auto accumTensor = loopState.front();

        // extracts the elements from the input tensors
        auto lhsElement = b.create<tensor::ExtractOp>(lhs, index);
        auto rhsElement = b.create<tensor::ExtractOp>(rhs, index);

        // adds the elements
        auto sum = b.create<arith::MulIOp>(lhsElement, rhsElement);

        // stores the result in the accumulator tensor
        auto resultTensor = b.create<tensor::InsertOp>(sum, accumTensor, index);
        // says that the result of the current iteration is the accumulator tensor for the next iteration
        // makes the accumTensor for the next iteration the result of the current iteration (resultTensor)

        // yields the accumulator tensor
        b.create<scf::YieldOp>(resultTensor.getResult()); // establishes resultTensor as the next accumTensor
      } 
    );

    rewriter.replaceOp(op, loop.getResult(0));
    return success();
  }
};

struct ConvertConstant : public OpConversionPattern<ConstantOp> {
  ConvertConstant(mlir::MLIRContext *context)
      : OpConversionPattern<ConstantOp>(context) {}

  using OpConversionPattern::OpConversionPattern;
  LogicalResult matchAndRewrite(ConstantOp op, OpAdaptor adaptor, ConversionPatternRewriter &rewriter) const override {
    llvm::errs() << "ConvertConstant\n";
    ImplicitLocOpBuilder b(op.getLoc(), rewriter);

    // get the atribute of the coefficients of the original operation -> arith.constant dense<[1, 2, 3]> -> atribute of coefficients dense<[1, 2, 3]>
    auto valueAttr = adaptor.getCoefficients();

    // creates a constant operation in the new dialect
    auto constOp = b.create<arith::ConstantOp>(adaptor.getCoefficients());

    // replaces the original operation with the new constant operation
    rewriter.replaceOp(op, constOp.getResult());

    return success();
  }
};

struct TinyToStandard : impl::TinyToStandardBase<TinyToStandard> {
  using TinyToStandardBase::TinyToStandardBase;

  void runOnOperation() override {
    MLIRContext *context = &getContext();
    auto *module = getOperation();

    ConversionTarget target(*context);
    // i'm not sure if it's the best practice to but thhis here like this...
    target.addLegalDialect<arith::ArithDialect>();
    target.addLegalDialect<scf::SCFDialect>();
    target.addLegalDialect<tensor::TensorDialect>();
    target.addIllegalDialect<TinyDialect>();

    RewritePatternSet patterns(context);
    patterns.add<ConvertAdd, ConvertMul, ConvertConstant>(context);

    if (failed(applyPartialConversion(module, target, std::move(patterns)))) {
      llvm::errs() << "Conversion failed. TinyDialect operations still exist.\n";
      signalPassFailure();
    }
  }
};

}  // namespace tiny
}  // namespace tinydialect
}  // namespace mlir