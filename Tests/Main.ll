; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare void @printf(i8*, ...)

declare void* @malloc(i64)

declare void* @calloc(i64, i64)

declare void @free(void*)

define { i64, void* }* @node_create(i64 %value) {
entry:
  %0 = tail call void* @calloc(i64 1, i64 16)
  %bitcast = bitcast void* %0 to { i64, void* }*
  %field_ptr = bitcast void* %0 to i64*
  store i64 %value, i64* %field_ptr, align 4
  %field_ptr5 = getelementptr inbounds { i64, void* }, { i64, void* }* %bitcast, i64 0, i32 1
  store void* null, void** %field_ptr5, align 8
  ret { i64, void* }* %bitcast
}

define void @__main(i32 %argc, i8** %argv) {
entry:
  %0 = tail call { i64, void* }* @node_create(i64 12)
  %1 = tail call { i64, void* }* @node_create(i64 34)
  %2 = tail call { i64, void* }* @node_create(i64 56)
  %field_ptr = getelementptr inbounds { i64, void* }, { i64, void* }* %1, i64 0, i32 1
  %3 = bitcast void** %field_ptr to { i64, void* }**
  store { i64, void* }* %2, { i64, void* }** %3, align 8
  %field_ptr6 = getelementptr inbounds { i64, void* }, { i64, void* }* %0, i64 0, i32 1
  %4 = bitcast void** %field_ptr6 to { i64, void* }**
  store { i64, void* }* %1, { i64, void* }** %4, align 8
  %ine.not24 = icmp eq { i64, void* }* %0, null
  br i1 %ine.not24, label %while.end, label %while.body.lr.ph

while.body.lr.ph:                                 ; preds = %entry
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %while.body
  %current.025 = phi { i64, void* }* [ %0, %while.body.lr.ph ], [ %field_val1523, %while.body ]
  %field_ptr1226 = bitcast { i64, void* }* %current.025 to i64*
  %field_val = load i64, i64* %field_ptr1226, align 4
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([4 x i8], [4 x i8]* @str, i64 0, i64 0), i64 %field_val)
  %field_ptr14 = getelementptr inbounds { i64, void* }, { i64, void* }* %current.025, i64 0, i32 1
  %5 = bitcast void** %field_ptr14 to { i64, void* }**
  %field_val1523 = load { i64, void* }*, { i64, void* }** %5, align 8
  %ine.not = icmp eq { i64, void* }* %field_val1523, null
  br i1 %ine.not, label %while.end, label %while.body

while.end:                                        ; preds = %while.body, %entry
  %bitcast18 = bitcast { i64, void* }* %2 to void*
  tail call void @free(void* %bitcast18)
  %bitcast20 = bitcast { i64, void* }* %1 to void*
  tail call void @free(void* %bitcast20)
  %bitcast22 = bitcast { i64, void* }* %0 to void*
  tail call void @free(void* %bitcast22)
  ret void
}
