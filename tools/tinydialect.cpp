#include "lib/Dialect/Tiny/TinyDialect.h"
#include "mlir/InitAllDialects.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"

int main(int argc, char**argv) {
  mlir::DialectRegistry registry;
  registry.insert<mlir::tinydialect::tiny::TinyDialect>();

  mlir::registerAllDialects(registry);

  return mlir::asMainReturnCode(
      mlir::MlirOptMain(argc, argv, "Tiny Dialect Pass Driver", registry));
}