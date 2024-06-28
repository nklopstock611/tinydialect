// RUN: tinydialect %s > %t
// RUN FileCheck %s < %t

module {
  // CHECK-LABEL: test_op_mul
  func.func @test_op_syntax() -> tensor<3xi32> {
    %1 = arith.constant dense<[1, 2, 3]> : tensor<3xi32>
    %2 = arith.constant dense<[2, 3, 4]> : tensor<3xi32>

    // CHECK: tiny.mul
    %3 = tiny.mul %1, %2 : tensor<3xi32>

    return %3 : tensor<3xi32>
  }
}