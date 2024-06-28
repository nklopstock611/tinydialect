add_mlir_library(TinyToStandard
  TinyToStandard.cpp

  ${PROJECT_SOURCE_DIR}/lib/Conversion/TinyToStandard/
  ADDITIONAL_HEADER_DIRS

  DEPENDS
  TinyToStandardPassIncGen

  LINK_COMPONENTS
  Core

  LINK_LIBS PUBLIC
  MLIRTiny
  MLIRArithDialect
  MLIRFuncDialect
  MLIRFuncTransforms
  MLIRIR
  MLIRPass
  MLIRSCFDialect
  MLIRTensorDialect
  MLIRTransforms
)

set(LLVM_TARGET_DEFINITIONS TinyToStandard.td)
mlir_tablegen(TinyToStandard.h.inc -gen-pass-decls -name TinyToStandard)
add_dependencies(mlir-headers MLIRTinyOpsIncGen)
add_public_tablegen_target(TinyToStandardPassIncGen)
add_mlir_doc(TinyToStandard TinyToStandard TinyToStandard/ -gen-pass-doc)