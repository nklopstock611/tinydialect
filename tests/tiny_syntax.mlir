// RUN: tinydialect %s > %t
// RUN FileCheck %s < %t

module {
  // CHECK-LABEL: test_op_syntax
  func.func @test_op_syntax(%arg0: tensor<2xi32>, %arg1: tensor<2xi32>) -> tensor<2xi32> {
    // CHECK: tiny.add
    %0 = tiny.add %arg0, %arg1 : tensor<2xi32>

    %1 = arith.constant dense<[1, 2, 3]> : tensor<3xi32>
    %2 = arith.constant dense<[2, 3, 4]> : tensor<3xi32>

    // CHECK: tiny.add
    %3 = tiny.add %1, %2 : tensor<3xi32>

    return %3 : tensor<3xi32>
  }
}