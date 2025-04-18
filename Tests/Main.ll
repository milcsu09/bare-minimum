; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare void @print(double)

define void @__main() {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %b.0 = phi i64 [ 1, %entry ], [ %add, %while.body ]
  %i.0 = phi i64 [ 3, %entry ], [ %add8, %while.body ]
  %a.0 = phi i64 [ 1, %entry ], [ %b.0, %while.body ]
  %ilt = icmp slt i64 %i.0, 20
  br i1 %ilt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %add = add i64 %a.0, %b.0
  %sitofp = sitofp i64 %add to double
  %call = call void @print(double %sitofp)
  %add8 = add i64 %i.0, 1
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret void
}
