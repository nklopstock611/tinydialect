#ifndef LIB_DIALECT_TINY_TINYOPS_TD_
#define LIB_DIALECT_TINY_TINYOPS_TD_

include "TinyDialect.td"
// include "TinyTypes.td"
include "mlir/IR/BuiltinTypes.td"
include "mlir/IR/BuiltinAttributes.td"
include "mlir/IR/BuiltinTypeInterfaces.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

class Tiny_BinOp<string mnemonic> : Op<Tiny_Dialect, mnemonic> {
    let arguments = (ins Builtin_RankedTensor:$lhs, Builtin_RankedTensor:$rhs);
    let results = (outs Builtin_RankedTensor:$output);
    let assemblyFormat = "$lhs `,` $rhs attr-dict `:` `(` type($lhs) `,` type($rhs) `)` `->` type($output)";
    let hasFolder = 1;
    // let hasCanonicalizer = 1;
}

def Tiny_AddOp : Tiny_BinOp<"add"> {
    let summary = "Add two tensors";
}

def Tiny_ConstantOp : Op<Tiny_Dialect, "constant"> {
  let summary = "Define a constant tensor via an attribute.";
  let arguments = (ins AnyIntElementsAttr:$coefficients);
  let results = (outs Builtin_RankedTensor:$output);
  let assemblyFormat = "$coefficients attr-dict `:` type($output)";
  let hasFolder = 1;
}

#endif  // LIB_DIALECT_TINY_TINYOPS_TD_