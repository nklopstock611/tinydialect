// RUN: tinydialect %s > %t
// RUN FileCheck %s < %t

module {
  // CHECK-LABEL: test_op_syntax
  func.func @test_op_syntax() -> tensor<3xi32> {
    %1 = arith.constant dense<[1, 2, 3]> : tensor<3xi32>
    %2 = arith.constant dense<[2, 3, 4]> : tensor<3xi32>

    // CHECK: tiny.add
    %3 = tiny.add %1, %2 : tensor<3xi32>

    return %3 : tensor<3xi32>
  }
}