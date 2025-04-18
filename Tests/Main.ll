; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare void @print(double)

declare void @puts(i8*)

declare void* @malloc(i64)

declare void @free(void*)

define void @__main() {
entry:
  %call = call void* @malloc(i64 27)
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i.0 = phi i64 [ 0, %entry ], [ %2, %while.body ]
  %ilt = icmp slt i64 %i.0, 26
  br i1 %ilt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %0 = bitcast void* %call to i8*
  %gep = getelementptr i8, i8* %0, i64 %i.0
  %1 = add i64 %i.0, 65
  %lsr.chain = trunc i64 %1 to i8
  store i8 %lsr.chain, i8* %gep, align 1
  %2 = add i64 %1, -64
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %3 = bitcast void* %call to i8*
  %gep8 = getelementptr i8, i8* %3, i64 26
  store i8 0, i8* %gep8, align 1
  %call10 = call void @puts(i8* %3)
  %bitcast12 = bitcast i8* %3 to void*
  %call13 = call void @free(void* %bitcast12)
  ret void
}
