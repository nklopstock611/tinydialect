// RUN: tinydialect --tiny-to-llvm %s | FileCheck %s

// CHECK-LABEL: test_tiny_fn
func.func @test_tiny_fn(%arg : i32) -> tensor<3xi32> {
    //%tens = tensor.splat %arg : tensor<3xi32>
    %0 = tiny.constant dense<[1, 2, 3]> : tensor<3xi32> : tensor<3xi32>
    //%1 = tiny.add %0, %tens : tensor<3xi32>
    return %0 : tensor<3xi32>
}