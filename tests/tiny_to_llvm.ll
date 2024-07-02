module {
  llvm.func @free(!llvm.ptr)
  llvm.func @malloc(i64) -> !llvm.ptr
  llvm.mlir.global private constant @__constant_3xi32_0(dense<[1, 2, 3]> : tensor<3xi32>) {addr_space = 0 : i32, alignment = 64 : i64} : !llvm.array<3 x i32>
  llvm.mlir.global private constant @__constant_3xi32(dense<0> : tensor<3xi32>) {addr_space = 0 : i32, alignment = 64 : i64} : !llvm.array<3 x i32>
  llvm.func @test_tiny_fn(%arg0: i32) -> !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> {
    %0 = llvm.mlir.zero : !llvm.ptr
    %1 = llvm.mlir.addressof @__constant_3xi32 : !llvm.ptr
    %2 = llvm.mlir.undef : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>
    %3 = llvm.mlir.addressof @__constant_3xi32_0 : !llvm.ptr
    %4 = llvm.mlir.constant(64 : index) : i64
    %5 = llvm.mlir.constant(1 : index) : i64
    %6 = llvm.mlir.constant(3 : index) : i64
    %7 = llvm.mlir.constant(0 : index) : i64
    %8 = llvm.getelementptr %1[0, 0] : (!llvm.ptr) -> !llvm.ptr, !llvm.array<3 x i32>
    %9 = llvm.getelementptr %3[0, 0] : (!llvm.ptr) -> !llvm.ptr, !llvm.array<3 x i32>
    %10 = llvm.getelementptr %0[3] : (!llvm.ptr) -> !llvm.ptr, i32
    %11 = llvm.ptrtoint %10 : !llvm.ptr to i64
    %12 = llvm.add %11, %4 : i64
    %13 = llvm.call @malloc(%12) : (i64) -> !llvm.ptr
    %14 = llvm.ptrtoint %13 : !llvm.ptr to i64
    %15 = llvm.sub %4, %5 : i64
    %16 = llvm.add %14, %15 : i64
    %17 = llvm.urem %16, %4  : i64
    %18 = llvm.sub %16, %17 : i64
    %19 = llvm.inttoptr %18 : i64 to !llvm.ptr
    llvm.br ^bb1(%7 : i64)
  ^bb1(%20: i64):  // 2 preds: ^bb0, ^bb2
    %21 = llvm.icmp "slt" %20, %6 : i64
    llvm.cond_br %21, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %22 = llvm.getelementptr %19[%20] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %arg0, %22 : i32, !llvm.ptr
    %23 = llvm.add %20, %5 : i64
    llvm.br ^bb1(%23 : i64)
  ^bb3:  // pred: ^bb1
    %24 = llvm.call @malloc(%12) : (i64) -> !llvm.ptr
    %25 = llvm.ptrtoint %24 : !llvm.ptr to i64
    %26 = llvm.add %25, %15 : i64
    %27 = llvm.urem %26, %4  : i64
    %28 = llvm.sub %26, %27 : i64
    %29 = llvm.inttoptr %28 : i64 to !llvm.ptr
    %30 = llvm.insertvalue %24, %2[0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %31 = llvm.insertvalue %29, %30[1] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %32 = llvm.insertvalue %7, %31[2] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %33 = llvm.insertvalue %6, %32[3, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %34 = llvm.insertvalue %5, %33[4, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %35 = llvm.mul %6, %5 : i64
    %36 = llvm.getelementptr %0[1] : (!llvm.ptr) -> !llvm.ptr, i32
    %37 = llvm.ptrtoint %36 : !llvm.ptr to i64
    %38 = llvm.mul %35, %37 : i64
    "llvm.intr.memcpy"(%29, %8, %38) <{isVolatile = false}> : (!llvm.ptr, !llvm.ptr, i64) -> ()
    llvm.br ^bb4(%7 : i64)
  ^bb4(%39: i64):  // 2 preds: ^bb3, ^bb5
    %40 = llvm.icmp "slt" %39, %6 : i64
    llvm.cond_br %40, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %41 = llvm.getelementptr %9[%39] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    %42 = llvm.load %41 : !llvm.ptr -> i32
    %43 = llvm.getelementptr %19[%39] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    %44 = llvm.load %43 : !llvm.ptr -> i32
    %45 = llvm.add %42, %44 : i32
    %46 = llvm.getelementptr %29[%39] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %45, %46 : i32, !llvm.ptr
    %47 = llvm.add %39, %5 : i64
    llvm.br ^bb4(%47 : i64)
  ^bb6:  // pred: ^bb4
    llvm.call @free(%13) : (!llvm.ptr) -> ()
    llvm.return %34 : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>
  }
}

