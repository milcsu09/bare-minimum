; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [8 x i8] c"%f, %f\0A\00", align 1

declare void @printf(i8*, ...)

define { double, double } @vec2_create(double %x, double %y) {
entry:
  %insert = insertvalue { double, double } undef, double %x, 0
  %insert5 = insertvalue { double, double } %insert, double %y, 1
  ret { double, double } %insert5
}

define void @__main(i32 %argc, i8** %argv) {
entry:
  %0 = tail call { double, double } @vec2_create(double 1.000000e+01, double 2.000000e+01)
  %.elt = extractvalue { double, double } %0, 0
  %.elt6 = extractvalue { double, double } %0, 1
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([8 x i8], [8 x i8]* @str, i64 0, i64 0), double %.elt, double %.elt6)
  ret void
}
