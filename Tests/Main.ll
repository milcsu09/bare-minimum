; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1

declare void @printf(i8*, ...)

declare void* @calloc(i64, i64)

declare void @free(void*)

define void @__main(i32 %argc, i8** %argv) {
entry:
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([5 x i8], [5 x i8]* @str, i64 0, i64 0), i64 16)
  ret void
}
