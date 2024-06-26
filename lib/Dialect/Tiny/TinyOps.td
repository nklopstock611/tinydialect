#ifndef LIB_DIALECT_TINY_TINYOPS_TD_
#define LIB_DIALECT_TINY_TINYOPS_TD_

include "TinyDialect.td"
include "TinyTypes.td"
include "mlir/IR/BuiltinAttributes.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

class Tiny_BinOp<string mnemonic> : Op<Tiny_Dialect, mnemonic> {
    let arguments = (ins Tensor:$lhs, Tensor:$rhs);
    let results = (outs Tensor:$result);
    let assemblyFormat = "$lhs `,` $rhs attr-dict `:` qualified(type($output))";
    let hasFolder = 1;
    let hasCanonicalizer = 1;
}

class Tiny_AddOp : Tiny_BinOp<"add"> {
    let summary = "Add two tensors";
}

#endif  // LIB_DIALECT_TINY_TINYOPS_TD_