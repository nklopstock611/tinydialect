#ifndef LIB_DIALECT_TINY_TINYDIALECT_TD_
#define LIB_DIALECT_TINY_TINYDIALECT_TD_

include "mlir/IR/OpBase.td"

def Tiny_Dialect : Dialect {
    let name = "tiny";
    let summary = "A dialect for tensor operations based on the python library 'tinygrad'";
    let description = [{
        The tiny dialect defines types and operations for tensors.
    }];

    let cppNamespace = "mlir::tinydialect::tiny";

    // let useDefaultTypePrinterParser  = 1;
    let hasConstantMaterializer = 1;
}

#endif  // LIB_DIALECT_TINY_TINYDIALECT_TD_