; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c" -> \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare void @printf(i8*, ...)

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

define void @node_free({ i64, void* }* %node) {
entry:
  %field_ptr = getelementptr inbounds { i64, void* }, { i64, void* }* %node, i64 0, i32 1
  %field_val = load void*, void** %field_ptr, align 8
  %ine.not = icmp eq void* %field_val, null
  br i1 %ine.not, label %if.merge, label %if.then

if.then:                                          ; preds = %entry
  %0 = bitcast void* %field_val to { i64, void* }*
  tail call void @node_free({ i64, void* }* %0)
  br label %if.merge

if.merge:                                         ; preds = %entry, %if.then
  %bitcast7 = bitcast { i64, void* }* %node to void*
  tail call void @free(void* %bitcast7)
  ret void
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
  %ine.not22 = icmp eq { i64, void* }* %0, null
  br i1 %ine.not22, label %while.end, label %while.body.lr.ph

while.body.lr.ph:                                 ; preds = %entry
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %if.merge
  %current.023 = phi { i64, void* }* [ %0, %while.body.lr.ph ], [ %field_val1521, %if.merge ]
  %field_ptr1224 = bitcast { i64, void* }* %current.023 to i64*
  %field_val = load i64, i64* %field_ptr1224, align 4
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @str, i64 0, i64 0), i64 %field_val)
  %field_ptr14 = getelementptr inbounds { i64, void* }, { i64, void* }* %current.023, i64 0, i32 1
  %5 = bitcast void** %field_ptr14 to { i64, void* }**
  %field_val1521 = load { i64, void* }*, { i64, void* }** %5, align 8
  %ine19.not = icmp eq { i64, void* }* %field_val1521, null
  br i1 %ine19.not, label %if.else, label %if.then

while.end:                                        ; preds = %if.merge, %entry
  tail call void @node_free({ i64, void* }* %0)
  ret void

if.then:                                          ; preds = %while.body
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i64 0, i64 0))
  br label %if.merge

if.else:                                          ; preds = %while.body
  %putchar = tail call i32 @putchar(i32 10)
  br label %if.merge

if.merge:                                         ; preds = %if.else, %if.then
  %ine.not = icmp eq { i64, void* }* %field_val1521, null
  br i1 %ine.not, label %while.end, label %while.body
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) #0

attributes #0 = { nofree nounwind }
