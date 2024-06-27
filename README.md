# tinydialect
Tests for a MLIR dialect influenced by the tinygrad python library

## TO-DO
### 4 Operation Types
- **Unary Operations:** Take a single input tensor and produce a single output tensor of the same size. It does some pointwise unary operation to it: `exp`, `log`, `neg`, `sin`, `relu` (almost all activation functions).
- **Binaryy Operations:** Take two input tensors of the same size and produce a single output tensor of the same size. It does some pointwise binary operation to them: `add`, `sub`, `mul`, `div`.
- **Reduce Operations:** Take a single input tensor and produce a single output tensor of a smaller size. It does some reduction operation to it: `sum`, `mean`, `max`, `min`.
- **Movement Operations:** Take a single input tensor and produce a single output tensor of the same size. It does some movement operation to it: `reshape`, `permutes`, `expands`, `flip`.