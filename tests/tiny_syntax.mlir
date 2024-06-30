// RUN: tinydialect %s > %t
// RUN FileCheck %s < %t

module {
  // CHECK-LABEL: test_op_syntax
  func.func @test_op_syntax(%arg0: tensor<3xi32>, %arg1: tensor<3xi32>) -> tensor<3xi32> {
    // CHECK: tiny.add
    %0 = tiny.add %arg0, %arg1 : tensor<3xi32>

    // CHECK: tiny.mul
    %1 = tiny.mul %arg0, %arg1 : tensor<3xi32>

    return %1 : tensor<3xi32>
  }
}