; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare void @printf(i8*, ...)

declare void @puts(i8*)

declare void* @malloc(i64)

declare void @free(void*)

define void @__main(i32 %argc, i8** %argv) {
entry:
  %xs = alloca i64**, align 8
  %argv2 = alloca i8**, align 8
  %argc1 = alloca i32, align 4
  store i32 %argc, i32* %argc1, align 4
  store i8** %argv, i8*** %argv2, align 8
  %0 = call void* @malloc(i64 32)
  %bitcast = bitcast void* %0 to i64**
  store i64** %bitcast, i64*** %xs, align 8
  %xs3 = load i64**, i64*** %xs, align 8
  %gep30 = bitcast i64** %xs3 to i64**
  %1 = call void* @malloc(i64 32)
  %ptrtoint = ptrtoint void* %1 to i64
  store i64 %ptrtoint, i64** %gep30, align 4
  %xs4 = load i64**, i64*** %xs, align 8
  %gep5 = getelementptr i64*, i64** %xs4, i64 1
  %2 = call void* @malloc(i64 32)
  %ptrtoint6 = ptrtoint void* %2 to i64
  store i64 %ptrtoint6, i64** %gep5, align 4
  %xs7 = load i64**, i64*** %xs, align 8
  %gep8 = getelementptr i64*, i64** %xs7, i64 2
  %3 = call void* @malloc(i64 32)
  %ptrtoint9 = ptrtoint void* %3 to i64
  store i64 %ptrtoint9, i64** %gep8, align 4
  %xs10 = load i64**, i64*** %xs, align 8
  %gep11 = getelementptr i64*, i64** %xs10, i64 3
  %4 = call void* @malloc(i64 32)
  %ptrtoint12 = ptrtoint void* %4 to i64
  store i64 %ptrtoint12, i64** %gep11, align 4
  %xs13 = load i64**, i64*** %xs, align 8
  %gep1431 = bitcast i64** %xs13 to i64**
  %deref = load i64*, i64** %gep1431, align 8
  %bitcast15 = bitcast i64* %deref to void*
  call void @free(void* %bitcast15)
  %xs16 = load i64**, i64*** %xs, align 8
  %gep17 = getelementptr i64*, i64** %xs16, i64 1
  %deref18 = load i64*, i64** %gep17, align 8
  %bitcast19 = bitcast i64* %deref18 to void*
  call void @free(void* %bitcast19)
  %xs20 = load i64**, i64*** %xs, align 8
  %gep21 = getelementptr i64*, i64** %xs20, i64 2
  %deref22 = load i64*, i64** %gep21, align 8
  %bitcast23 = bitcast i64* %deref22 to void*
  call void @free(void* %bitcast23)
  %xs24 = load i64**, i64*** %xs, align 8
  %gep25 = getelementptr i64*, i64** %xs24, i64 3
  %deref26 = load i64*, i64** %gep25, align 8
  %bitcast27 = bitcast i64* %deref26 to void*
  call void @free(void* %bitcast27)
  %xs28 = load i64**, i64*** %xs, align 8
  %bitcast29 = bitcast i64** %xs28 to void*
  call void @free(void* %bitcast29)
  ret void
}
