; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [15 x i8] c"Vec2 [%g, %g]\0A\00", align 1
@str.1 = private unnamed_addr constant [16 x i8] c"*Vec2 [%g, %g]\0A\00", align 1
@str.2 = private unnamed_addr constant [15 x i8] c"Vec2 [%g, %g]\0A\00", align 1
@str.3 = private unnamed_addr constant [16 x i8] c"*Vec2 [%g, %g]\0A\00", align 1

declare void @printf(i8*, ...)

define void @__main(i32 %argc, i8** %argv) {
entry:
  %p = alloca { double, double }*, align 8
  %v = alloca { double, double }, align 8
  %argv2 = alloca i8**, align 8
  %argc1 = alloca i32, align 4
  store i32 %argc, i32* %argc1, align 4
  store i8** %argv, i8*** %argv2, align 8
  store { double, double } { double 1.500000e+00, double 2.500000e+00 }, { double, double }* %v, align 8
  %field_ptr25 = bitcast { double, double }* %v to double*
  %field_val = load double, double* %field_ptr25, align 8
  %field_ptr3 = getelementptr inbounds { double, double }, { double, double }* %v, i32 0, i32 1
  %field_val4 = load double, double* %field_ptr3, align 8
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), double %field_val, double %field_val4)
  store { double, double }* %v, { double, double }** %p, align 8
  %p5 = load { double, double }*, { double, double }** %p, align 8
  %field_ptr626 = bitcast { double, double }* %p5 to double*
  %field_val7 = load double, double* %field_ptr626, align 8
  %p8 = load { double, double }*, { double, double }** %p, align 8
  %field_ptr9 = getelementptr inbounds { double, double }, { double, double }* %p8, i32 0, i32 1
  %field_val10 = load double, double* %field_ptr9, align 8
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), double %field_val7, double %field_val10)
  %p11 = load { double, double }*, { double, double }** %p, align 8
  %field_ptr1227 = bitcast { double, double }* %p11 to double*
  store double 1.000000e+01, double* %field_ptr1227, align 8
  %p13 = load { double, double }*, { double, double }** %p, align 8
  %field_ptr14 = getelementptr inbounds { double, double }, { double, double }* %p13, i32 0, i32 1
  store double 2.000000e+01, double* %field_ptr14, align 8
  %field_ptr1528 = bitcast { double, double }* %v to double*
  %field_val16 = load double, double* %field_ptr1528, align 8
  %field_ptr17 = getelementptr inbounds { double, double }, { double, double }* %v, i32 0, i32 1
  %field_val18 = load double, double* %field_ptr17, align 8
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.2, i32 0, i32 0), double %field_val16, double %field_val18)
  %p19 = load { double, double }*, { double, double }** %p, align 8
  %field_ptr2029 = bitcast { double, double }* %p19 to double*
  %field_val21 = load double, double* %field_ptr2029, align 8
  %p22 = load { double, double }*, { double, double }** %p, align 8
  %field_ptr23 = getelementptr inbounds { double, double }, { double, double }* %p22, i32 0, i32 1
  %field_val24 = load double, double* %field_ptr23, align 8
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.3, i32 0, i32 0), double %field_val21, double %field_val24)
  ret void
}
