; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"(+-*/)\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare void @printf(i8*, ...)

declare void* @malloc(i64)

declare void* @calloc(i64, i64)

declare void @free(void*)

declare i8* @strcpy(i8*, i8*)

declare i8* @strncpy(i8*, i8*, i64)

define { i64 } @lexerNext({ i8* }* %lexer) {
entry:
  %field_ptr354 = bitcast { i8* }* %lexer to i8**
  %field_val = load i8*, i8** %field_ptr354, align 8
  %deref = load i8, i8* %field_val, align 1
  switch i8 %deref, label %if.merge [
    i8 40, label %if.then
    i8 41, label %if.then8
    i8 43, label %if.then14
    i8 45, label %if.then20
    i8 42, label %if.then26
    i8 47, label %if.then32
  ]

if.then:                                          ; preds = %entry
  br label %if.merge

if.merge:                                         ; preds = %if.then32, %entry, %if.then26, %if.then20, %if.then14, %if.then8, %if.then
  %token46.unpack = phi i64 [ 0, %if.then ], [ 1, %if.then8 ], [ 2, %if.then14 ], [ 3, %if.then20 ], [ 4, %if.then26 ], [ 5, %if.then32 ], [ -1, %entry ]
  %ine.not = icmp eq i64 %token46.unpack, -1
  br i1 %ine.not, label %if.merge40, label %if.then38

if.then8:                                         ; preds = %entry
  br label %if.merge

if.then14:                                        ; preds = %entry
  br label %if.merge

if.then20:                                        ; preds = %entry
  br label %if.merge

if.then26:                                        ; preds = %entry
  br label %if.merge

if.then32:                                        ; preds = %entry
  br label %if.merge

if.then38:                                        ; preds = %if.merge
  %0 = bitcast { i8* }* %lexer to i8**
  %gep = getelementptr i8, i8* %field_val, i64 1
  store i8* %gep, i8** %0, align 8
  br label %if.merge40

if.merge40:                                       ; preds = %if.merge, %if.then38
  %token4647 = insertvalue { i64 } undef, i64 %token46.unpack, 0
  ret { i64 } %token4647
}

define void @__main(i32 %argc, i8** %argv) {
entry:
  %lexer = alloca { i8* }, align 8
  %0 = bitcast { i8* }* %lexer to i8**
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i64 0, i64 0), i8** %0, align 8
  %1 = call { i64 } @lexerNext({ i8* }* nonnull %lexer)
  %2 = extractvalue { i64 } %1, 0
  %ine.not5 = icmp eq i64 %2, -1
  br i1 %ine.not5, label %while.end, label %while.body.lr.ph

while.body.lr.ph:                                 ; preds = %entry
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %while.body
  %3 = phi i64 [ %2, %while.body.lr.ph ], [ %5, %while.body ]
  call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i64 0, i64 0), i64 %3)
  %4 = call { i64 } @lexerNext({ i8* }* nonnull %lexer)
  %5 = extractvalue { i64 } %4, 0
  %ine.not = icmp eq i64 %5, -1
  br i1 %ine.not, label %while.end, label %while.body

while.end:                                        ; preds = %while.body, %entry
  ret void
}
