get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)

set (LIBS
    ${dialect_libs}
    ${conversion_libs}
#     AffineFullUnroll
#     MLIRNoisy
    MLIROptLib
    MLIRPass
    MLIRTiny
#     MulToAdd
#     NoisyPasses
    TinyToStandard
    ortools::ortools
)

add_llvm_executable(tinydialect tinydialect-opt.cpp)

llvm_update_compile_flags(tinydialect)
target_link_libraries(tinydialect PRIVATE ${LIBS})

mlir_check_all_link_libraries(tinydialect)