; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"Test\00", align 1

declare void @printf(i8*, ...)

declare void @scanf(i8*, void*)

declare void @puts(i8*)

declare i64 @strlen(i8*)

define i64 @my_strlen(i8* %s) {
entry:
  %deref8 = load i8, i8* %s, align 1
  %igt.not9 = icmp eq i8 %deref8, 0
  br i1 %igt.not9, label %while.end, label %while.body.lr.ph

while.body.lr.ph:                                 ; preds = %entry
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %while.body
  %i.011 = phi i64 [ 0, %while.body.lr.ph ], [ %add, %while.body ]
  %add = add i64 %i.011, 1
  %sunkaddr = getelementptr i8, i8* %s, i64 %i.011
  %sunkaddr15 = getelementptr i8, i8* %sunkaddr, i64 1
  %deref = load i8, i8* %sunkaddr15, align 1
  %igt.not = icmp eq i8 %deref, 0
  br i1 %igt.not, label %while.end, label %while.body

while.end:                                        ; preds = %while.body, %entry
  %i.0.lcssa = phi i64 [ 0, %entry ], [ %add, %while.body ]
  ret i64 %i.0.lcssa
}

define void @__main(i32 %argc, i8** %argv) {
entry:
  %0 = tail call i64 @my_strlen(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i64 0, i64 0))
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([5 x i8], [5 x i8]* @str, i64 0, i64 0), i64 %0)
  ret void
}
