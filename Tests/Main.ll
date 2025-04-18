; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare void @print(double)

define void @__main() {
entry:
  %b = alloca i64*, align 8
  %a = alloca i64, align 8
  store i64 5, i64* %a, align 4
  store i64* %a, i64** %b, align 8
  %b1 = load i64*, i64** %b, align 8
  store i64 12, i64* %b1, align 4
  %a2 = load i64, i64* %a, align 4
  %sitofp = sitofp i64 %a2 to double
  %call = call void @print(double %sitofp)
  %b3 = load i64*, i64** %b, align 8
  %ptrtoint = ptrtoint i64* %b3 to i64
  %sitofp4 = sitofp i64 %ptrtoint to double
  %call5 = call void @print(double %sitofp4)
  %b6 = load i64*, i64** %b, align 8
  %deref = load i64, i64* %b6, align 4
  %sitofp7 = sitofp i64 %deref to double
  %call8 = call void @print(double %sitofp7)
  ret void
}
