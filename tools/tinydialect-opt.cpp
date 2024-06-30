#include "lib/Dialect/Tiny/TinyDialect.h"
#include "mlir/InitAllDialects.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Pass/PassRegistry.h"
#include "lib/Conversion/TinyToStandard/TinyToStandard.h"
#include "mlir/Transforms/Passes.h"

// void tinyToLLVMPipelineBuilder(mlir::OpPassManager &manager) {
//   // Tiny
//   manager.addPass(mlir::tinydialect::tiny::convertTinyToStandard());
//   manager.addPass(mlir::createCanonicalizerPass());

//   // Cleanup
//   manager.addPass(mlir::createCanonicalizerPass());
//   manager.addPass(mlir::createSCCPPass());
//   manager.addPass(mlir::createCSEPass());
//   manager.addPass(mlir::createSymbolDCEPass());
// }

int main(int argc, char**argv) {
  mlir::DialectRegistry registry;
  registry.insert<mlir::tinydialect::tiny::TinyDialect>();

  mlir::registerAllDialects(registry);
  mlir::registerAllPasses();

  mlir::tinydialect::tiny::registerTinyToStandardPasses();

  // mlir::PassPipelineRegistration<>(
  //     "tiny-to-llvm", "Run passes to lower the tiny dialect to LLVM",
  //     tinyToLLVMPipelineBuilder);

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "Tiny Dialect Pass Driver", registry));
}