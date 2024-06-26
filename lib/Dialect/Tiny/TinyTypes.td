#ifndef LIB_DIALECT_TINY_TINYTYPES_TD_
#define LIB_DIALECT_TINY_TINYTYPES_TD_

include "TinyDialect.td"

class Tiny_Type<string name, string typeMnemonic> : TypeDef<Tiny_Dialect, name> {
    let mnemonic = typeMnemonic;
}

def Tensor : Tiny_Type<"Tensor", "tiny"> {
    let summary = "A tensor";

    let description = [{
        "A type for multi-dimensional arrays of elements of a specific type."
    }];

    let parameters = (ins "int":$elementType, "int":$dimensionsRows, "int":$dimensionsCols);
    let assemblyFormat = "`<` $dimensionsRows `x` $dimensionsCols `x` $elementType `>`";

    // let builders = [
    //     OpBuilder<(ins "Type":$elementType, "ArrayRef<int64_t>":$dimensions), [{
    //         $_self = $_builder.getTensorType($dimensions, $elementType);
    //     }]>
    // ];
}

#endif  // LIB_DIALECT_TINY_TINYTYPES_TD_