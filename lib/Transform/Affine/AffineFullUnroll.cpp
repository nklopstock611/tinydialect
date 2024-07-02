#include "lib/Transform/Affine/AffineFullUnroll.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Affine/LoopUtils.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"
#include "mlir/Pass/Pass.h"

namespace mlir {
namespace tinydialect {

#define GEN_PASS_DEF_AFFINEFULLUNROLL
#include "lib/Transform/Affine/Passes.h.inc"

using mlir::affine::AffineForOp;
using mlir::affine::loopUnrollFull;

// A pattern that matches on AffineForOp and unrolls it.
struct AffineFullUnroll : public OpRewritePattern<AffineForOp> {
  AffineFullUnroll(mlir::MLIRContext *context)
      : OpRewritePattern<AffineForOp>(context, /*benefit=*/1) {}

  LogicalResult matchAndRewrite(AffineForOp op,
                                PatternRewriter &rewriter) const override {
    // This is technically not allowed, since in a RewritePattern all
    // modifications to the IR are supposed to go through the `rewriter` arg,
    // but it works for our limited test cases.
    return loopUnrollFull(op);
  }
};

// A pass that invokes the pattern rewrite engine.
struct AffineFullUnrollPatternRewrite
    : impl::AffineFullUnrollBase<AffineFullUnrollPatternRewrite> {
  using AffineFullUnrollBase::AffineFullUnrollBase;
  void runOnOperation() override {
    mlir::RewritePatternSet patterns(&getContext());
    patterns.add<AffineFullUnroll>(&getContext());
    // One could use GreedyRewriteConfig here to slightly tweak the behavior of
    // the pattern application.
    (void)applyPatternsAndFoldGreedily(getOperation(), std::move(patterns));
  }
};

} // namespace tinydialect
} // namespace mlir
