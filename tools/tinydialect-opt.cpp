#include "lib/Conversion/TinyToStandard/TinyToStandard.h"
#include "lib/Dialect/Tiny/TinyDialect.h"
#include "lib/Transform/Affine/Passes.h"

#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Pass/PassRegistry.h"
#include "mlir/Transforms/Passes.h"
#include "mlir/Transforms/Passes.h"
#include "mlir/Dialect/Linalg/Passes.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"
#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Dialect/Bufferization/Pipelines/Passes.h"
#include "mlir/Dialect/Bufferization/Transforms/Passes.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVMPass.h"
#include "mlir/Conversion/SCFToControlFlow/SCFToControlFlow.h"
#include "mlir/Conversion/TensorToLinalg/TensorToLinalgPass.h"
#include "mlir/Conversion/ControlFlowToLLVM/ControlFlowToLLVM.h"

void tinyToLLVMPipelineBuilder(mlir::OpPassManager &manager) {
  // Tiny to Standard
  manager.addPass(mlir::tinydialect::tiny::createTinyToStandard());
  
  // Convert Elementwise and Tensor to Linalg
  manager.addPass(mlir::createConvertElementwiseToLinalgPass());
  manager.addPass(mlir::createConvertTensorToLinalgPass());
  
  // Canonicalize
  manager.addPass(mlir::createCanonicalizerPass());

  // Bufferization
  mlir::bufferization::OneShotBufferizationOptions bufferizationOptions;
  bufferizationOptions.bufferizeFunctionBoundaries = true;
  manager.addPass(mlir::bufferization::createOneShotBufferizePass(bufferizationOptions));
  // mlir::bufferization::BufferDeallocationPipelineOptions deallocationOptions;
  // mlir::bufferization::buildBufferDeallocationPipeline(manager, deallocationOptions);
  manager.addPass(mlir::bufferization::createBufferDeallocationPass());
  
  // Convert Linalg to Loops
  manager.addPass(mlir::createConvertLinalgToLoopsPass());

  // Expand Strided Metadata for MemRef
  manager.addPass(mlir::memref::createExpandStridedMetadataPass());

  // Convert SCF to CF
  manager.addPass(mlir::createConvertSCFToCFPass());

  // Convert Bufferization results to MemRef
  manager.addPass(mlir::bufferization::createBufferDeallocationPass());

  // Convert Control Flow to LLVM
  manager.addPass(mlir::createConvertControlFlowToLLVMPass());

  // Convert Arithmetic operations to LLVM
  manager.addPass(mlir::createArithToLLVMConversionPass());

  // Convert Func dialect to LLVM
  manager.addPass(mlir::createConvertFuncToLLVMPass());

  // Finalize MemRef to LLVM conversion
  manager.addPass(mlir::createFinalizeMemRefToLLVMConversionPass());

  manager.addPass(mlir::bufferization::createOneShotBufferizePass(bufferizationOptions));

  // Reconcile Unrealized Casts (must be at the end)
  manager.addPass(mlir::createReconcileUnrealizedCastsPass());

  // Cleanup passes
  manager.addPass(mlir::createCanonicalizerPass());
  manager.addPass(mlir::createSCCPPass());
  manager.addPass(mlir::createCSEPass());
  manager.addPass(mlir::createSymbolDCEPass());
}


int main(int argc, char**argv) {
  mlir::DialectRegistry registry;
  registry.insert<mlir::tinydialect::tiny::TinyDialect>();

  mlir::registerAllDialects(registry);
  mlir::registerAllPasses();

  // mlir::tinydialect::registerAffinePasses();

  mlir::tinydialect::tiny::registerTinyToStandardPasses();

  mlir::PassPipelineRegistration<>(
      "tiny-to-llvm", "Run passes to lower the tiny dialect to LLVM",
      tinyToLLVMPipelineBuilder);

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "Tiny Dialect Pass Driver", registry));
}