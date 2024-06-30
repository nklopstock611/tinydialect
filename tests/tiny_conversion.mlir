// RUN: tinydialect %s > %t
// RUN FileCheck %s < %t

module {
  // CHECK-LABEL: test_lower_add_and_fold
  // CHECK-NOT: tiny
  func.func @test_lower_add_and_fold(%arg : tensor<3xi32>) -> tensor<3xi32> {
    %0 = tiny.constant dense<[1, 2, 3]> : tensor<3xi32> : tensor<3xi32>

    %2 = tiny.add %0, %arg : tensor<3xi32>

    return %2 : tensor<3xi32>
  }
}