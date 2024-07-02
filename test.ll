module {
  llvm.func @free(!llvm.ptr)
  llvm.mlir.global private constant @__constant_3xi32(dense<[1, 2, 3]> : tensor<3xi32>) {addr_space = 0 : i32, alignment = 64 : i64} : !llvm.array<3 x i32>
  llvm.func @test_tiny_fn(%arg0: i32, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: i64, %arg4: i64, %arg5: i64) {
    %0 = llvm.mlir.undef : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>
    %1 = llvm.mlir.zero : !llvm.ptr
    %2 = llvm.mlir.addressof @__constant_3xi32 : !llvm.ptr
    %3 = llvm.mlir.constant(0 : index) : i64
    %4 = llvm.mlir.constant(3735928559 : index) : i64
    %5 = llvm.mlir.constant(1 : index) : i64
    %6 = llvm.mlir.constant(3 : index) : i64
    %7 = llvm.getelementptr %2[0, 0] : (!llvm.ptr) -> !llvm.ptr, !llvm.array<3 x i32>
    %8 = llvm.inttoptr %4 : i64 to !llvm.ptr
    %9 = llvm.insertvalue %8, %0[0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %10 = llvm.insertvalue %7, %9[1] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %11 = llvm.insertvalue %3, %10[2] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %12 = llvm.insertvalue %6, %11[3, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %13 = llvm.insertvalue %5, %12[4, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %14 = builtin.unrealized_conversion_cast %13 : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> to memref<3xi32>
    %15 = bufferization.clone %14 : memref<3xi32> to memref<3xi32>
    %16 = builtin.unrealized_conversion_cast %15 : memref<3xi32> to !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>
    %17 = llvm.extractvalue %16[3, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %18 = llvm.mul %17, %5 : i64
    %19 = llvm.getelementptr %1[1] : (!llvm.ptr) -> !llvm.ptr, i32
    %20 = llvm.ptrtoint %19 : !llvm.ptr to i64
    %21 = llvm.mul %18, %20 : i64
    %22 = llvm.extractvalue %16[1] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %23 = llvm.extractvalue %16[2] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %24 = llvm.getelementptr %22[%23] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    %25 = llvm.getelementptr %arg2[%arg3] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    "llvm.intr.memcpy"(%25, %24, %21) <{isVolatile = false}> : (!llvm.ptr, !llvm.ptr, i64) -> ()
    %26 = llvm.extractvalue %16[0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    llvm.call @free(%26) : (!llvm.ptr) -> ()
    llvm.return
  }
}

