#ifndef LIB_CONVERSION_TINYTOSTANDARD_TINYTOSTANDARD_TD_
#define LIB_CONVERSION_TINYTOSTANDARD_TINYTOSTANDARD_TD_

include "mlir/Pass/PassBase.td"

def TinyToStandard : Pass<"tiny-to-standard"> {
    let summary = "Lower tiny dialect to standard MLIR dialects.";

    let description = [{
        This pass lowers the tiny dialect to standard MLIR.
    }];

    let dependentDialects = [
        "mlir::arith::ArithDialect",
        "mlir::tinydialect::tiny::TinyDialect",
        "mlir::tensor::TensorDialect",
        "mlir::scf::SCFDialect",
    ];
}

#endif  // LIB_CONVERSION_TINYTOSTANDARD_TINYTOSTANDARD_TD_