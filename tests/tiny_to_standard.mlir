// RUN: tinydialect --tiny-to-standard %s > %t
// RUN FileCheck %s < %t

// CHECK-LABEL: test_lower_add
// CHECK-SAME:  (%[[p0:.*]]: [[T:tensor<3xi32>]], %[[p1:.*]]: [[T:tensor<3xi32>]]) -> [[T]] {
// CHECK: %[[cst:.*]] = arith.constant dense<0> : tensor<3xi32>
// CHECK: %[[c0:.*]] = arith.constant 0 : index
// CHECK: %[[c3:.*]] = arith.constant 3 : index
// CHECK: %[[c1:.*]] = arith.constant 1 : index
// CHECK: %[[loop:.*]] = scf.for %[[loop_iv:.*]] = %[[c0]] to %[[c3]] step %[[c1]] iter_args(%[[loop_iter_arg:.*]] = %[[cst]]) -> ([[T]]) {
// CHECK-DAG: %[[p0_extracted:.*]] = tensor.extract %[[p0]][%[[loop_iv]]]
// CHECK-DAG: %[[p1_extracted:.*]] = tensor.extract %[[p1]][%[[loop_iv]]]
// CHECK: %[[add:.*]] = arith.addi %[[p0_extracted]], %[[p1_extracted]]
// CHECK: %[[inserted:.*]] = tensor.insert %[[add]] into %[[loop_iter_arg]][%[[loop_iv]]]
// CHECK: scf.yield %[[inserted]]
// CHECK: }
// CHECK: return %[[loop]] : [[T]]
func.func @test_lower_add(%arg0 : tensor<3xi32>, %arg1 : tensor<3xi32>) -> tensor<3xi32> {
  %0 = tiny.add %arg0, %arg1: tensor<3xi32>
  return %0 : tensor<3xi32>
}

// CHECK-LABEL: test_lower_add_and_fold
func.func @test_lower_add_and_fold() {
  // CHECK: arith.constant dense<[1, 3, 5]> : tensor<3xi32>
  %0 = tiny.constant dense<[1, 3, 5]> : tensor<3xi32> : tensor<3xi32>
  // CHECK: arith.constant dense<[2, 4, 6]> : tensor<3xi32>
  %1 = tiny.constant dense<[2, 4, 6]> : tensor<3xi32> : tensor<3xi32>
  // like in the tutorial -> would be an arith.addi, but it was folded
  // CHECK: arith.constant
  %2 = tiny.add %0, %1: tensor<3xi32>

  return
}

// CHECK-LABEL: test_lower_mul
// CHECK-SAME:  (%[[p0:.*]]: [[T:tensor<3xi32>]], %[[p1:.*]]: [[T:tensor<3xi32>]]) -> [[T]] {
// CHECK: %[[cst:.*]] = arith.constant dense<0> : tensor<3xi32>
// CHECK: %[[c0:.*]] = arith.constant 0 : index
// CHECK: %[[c3:.*]] = arith.constant 3 : index
// CHECK: %[[c1:.*]] = arith.constant 1 : index
// CHECK: %[[loop:.*]] = scf.for %[[loop_iv:.*]] = %[[c0]] to %[[c3]] step %[[c1]] iter_args(%[[loop_iter_arg:.*]] = %[[cst]]) -> ([[T]]) {
// CHECK-DAG: %[[p0_extracted:.*]] = tensor.extract %[[p0]][%[[loop_iv]]]
// CHECK-DAG: %[[p1_extracted:.*]] = tensor.extract %[[p1]][%[[loop_iv]]]
// CHECK: %[[mul:.*]] = arith.muli %[[p0_extracted]], %[[p1_extracted]]
// CHECK: %[[inserted:.*]] = tensor.insert %[[mul]] into %[[loop_iter_arg]][%[[loop_iv]]]
// CHECK: scf.yield %[[inserted]]
// CHECK: }
// CHECK: return %[[loop]] : [[T]]
func.func @test_lower_mul(%arg0 : tensor<3xi32>, %arg1 : tensor<3xi32>) -> tensor<3xi32> {
  %0 = tiny.mul %arg0, %arg1: tensor<3xi32>
  return %0 : tensor<3xi32>
}

// CHECK-LABEL: test_lower_mul_and_fold
func.func @test_lower_mul_and_fold() {
  // CHECK: arith.constant dense<[1, 3, 5]> : tensor<3xi32>
  %0 = tiny.constant dense<[1, 3, 5]> : tensor<3xi32> : tensor<3xi32>
  // CHECK: arith.constant dense<[2, 4, 6]> : tensor<3xi32>
  %1 = tiny.constant dense<[2, 4, 6]> : tensor<3xi32> : tensor<3xi32>
  // like in the tutorial -> would be an arith.muli, but it was folded
  // CHECK: arith.constant
  %2 = tiny.mul %0, %1: tensor<3xi32>
  
  return
}
