; ModuleID = 'Tests/Main.txt'
source_filename = "Tests/Main.txt"

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"NODE-%d\0A\00", align 1

declare void @printf(i8*, ...)

declare void* @calloc(i64, i64)

declare void @free(void*)

define { i64, i64, void*, void* }* @ast_create(i64 %kind) {
entry:
  %0 = tail call void* @calloc(i64 1, i64 32)
  %bitcast = bitcast void* %0 to { i64, i64, void*, void* }*
  %field_ptr = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %bitcast, i64 0, i32 1
  store i64 %kind, i64* %field_ptr, align 4
  ret { i64, i64, void*, void* }* %bitcast
}

define void @ast_attach({ i64, i64, void*, void* }* %root, { i64, i64, void*, void* }* %node) {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  %current.0 = phi { i64, i64, void*, void* }* [ %root, %entry ], [ %0, %while.cond ]
  %field_ptr = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %current.0, i64 0, i32 3
  %field_val = load void*, void** %field_ptr, align 8
  %ine.not = icmp eq void* %field_val, null
  %0 = bitcast void* %field_val to { i64, i64, void*, void* }*
  br i1 %ine.not, label %while.end, label %while.cond

while.end:                                        ; preds = %while.cond
  %field_ptr.lcssa = phi void** [ %field_ptr, %while.cond ]
  %1 = bitcast void** %field_ptr.lcssa to { i64, i64, void*, void* }**
  store { i64, i64, void*, void* }* %node, { i64, i64, void*, void* }** %1, align 8
  ret void
}

define void @ast_append({ i64, i64, void*, void* }* %root, { i64, i64, void*, void* }* %node) {
entry:
  %field_ptr = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %root, i64 0, i32 2
  %field_val = load void*, void** %field_ptr, align 8
  %ieq = icmp eq void* %field_val, null
  %0 = bitcast void* %field_val to { i64, i64, void*, void* }*
  br i1 %ieq, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = bitcast void** %field_ptr to { i64, i64, void*, void* }**
  store { i64, i64, void*, void* }* %node, { i64, i64, void*, void* }** %1, align 8
  br label %if.merge

if.else:                                          ; preds = %entry
  tail call void @ast_attach({ i64, i64, void*, void* }* %0, { i64, i64, void*, void* }* %node)
  br label %if.merge

if.merge:                                         ; preds = %if.else, %if.then
  ret void
}

define void @ast_destroy({ i64, i64, void*, void* }* %root) {
entry:
  %field_ptr = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %root, i64 0, i32 2
  %field_val = load void*, void** %field_ptr, align 8
  %ine.not = icmp eq void* %field_val, null
  %0 = bitcast void* %field_val to { i64, i64, void*, void* }*
  br i1 %ine.not, label %if.merge, label %if.then

if.then:                                          ; preds = %entry
  tail call void @ast_destroy({ i64, i64, void*, void* }* %0)
  br label %if.merge

if.merge:                                         ; preds = %entry, %if.then
  %field_ptr7 = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %root, i64 0, i32 3
  %field_val8 = load void*, void** %field_ptr7, align 8
  %ine10.not = icmp eq void* %field_val8, null
  %1 = bitcast void* %field_val8 to { i64, i64, void*, void* }*
  br i1 %ine10.not, label %if.merge13, label %if.then11

if.then11:                                        ; preds = %if.merge
  tail call void @ast_destroy({ i64, i64, void*, void* }* %1)
  br label %if.merge13

if.merge13:                                       ; preds = %if.merge, %if.then11
  %bitcast19 = bitcast { i64, i64, void*, void* }* %root to void*
  tail call void @free(void* %bitcast19)
  ret void
}

define void @ast_diagnostic({ i64, i64, void*, void* }* %root, i64 %depth) {
entry:
  %mul = shl i64 %depth, 2
  %igt29 = icmp sgt i64 %mul, 0
  %add = add i64 %depth, 1
  br label %tailrecurse

tailrecurse:                                      ; preds = %if.merge, %entry
  %root.tr = phi { i64, i64, void*, void* }* [ %root, %entry ], [ %1, %if.merge ]
  br i1 %igt29, label %while.body.lr.ph, label %while.end

while.body.lr.ph:                                 ; preds = %tailrecurse
  br label %while.body

while.body:                                       ; preds = %while.body.lr.ph, %while.body
  %i.030 = phi i64 [ %mul, %while.body.lr.ph ], [ %sub, %while.body ]
  %putchar = tail call i32 @putchar(i32 32)
  %sub = add i64 %i.030, -1
  %igt = icmp sgt i64 %sub, 0
  br i1 %igt, label %while.body, label %while.cond.while.end_crit_edge

while.cond.while.end_crit_edge:                   ; preds = %while.body
  br label %while.end

while.end:                                        ; preds = %while.cond.while.end_crit_edge, %tailrecurse
  %field_ptr = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %root.tr, i64 0, i32 1
  %field_val = load i64, i64* %field_ptr, align 4
  tail call void (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i64 0, i64 0), i64 %field_val)
  %field_ptr8 = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %root.tr, i64 0, i32 2
  %field_val9 = load void*, void** %field_ptr8, align 8
  %ine.not = icmp eq void* %field_val9, null
  %0 = bitcast void* %field_val9 to { i64, i64, void*, void* }*
  br i1 %ine.not, label %if.merge, label %if.then

if.then:                                          ; preds = %while.end
  tail call void @ast_diagnostic({ i64, i64, void*, void* }* %0, i64 %add)
  br label %if.merge

if.merge:                                         ; preds = %while.end, %if.then
  %field_ptr15 = getelementptr inbounds { i64, i64, void*, void* }, { i64, i64, void*, void* }* %root.tr, i64 0, i32 3
  %field_val16 = load void*, void** %field_ptr15, align 8
  %ine18.not = icmp eq void* %field_val16, null
  %1 = bitcast void* %field_val16 to { i64, i64, void*, void* }*
  br i1 %ine18.not, label %if.merge21, label %tailrecurse

if.merge21:                                       ; preds = %if.merge
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) #0

define void @__main(i32 %argc, i8** %argv) {
entry:
  %0 = tail call { i64, i64, void*, void* }* @ast_create(i64 0)
  %1 = tail call { i64, i64, void*, void* }* @ast_create(i64 1)
  %2 = tail call { i64, i64, void*, void* }* @ast_create(i64 3)
  %3 = tail call { i64, i64, void*, void* }* @ast_create(i64 4)
  tail call void @ast_append({ i64, i64, void*, void* }* %1, { i64, i64, void*, void* }* %2)
  tail call void @ast_append({ i64, i64, void*, void* }* %1, { i64, i64, void*, void* }* %3)
  %4 = tail call { i64, i64, void*, void* }* @ast_create(i64 2)
  %5 = tail call { i64, i64, void*, void* }* @ast_create(i64 5)
  %6 = tail call { i64, i64, void*, void* }* @ast_create(i64 6)
  tail call void @ast_append({ i64, i64, void*, void* }* %4, { i64, i64, void*, void* }* %5)
  tail call void @ast_append({ i64, i64, void*, void* }* %4, { i64, i64, void*, void* }* %6)
  tail call void @ast_append({ i64, i64, void*, void* }* %0, { i64, i64, void*, void* }* %1)
  tail call void @ast_append({ i64, i64, void*, void* }* %0, { i64, i64, void*, void* }* %4)
  tail call void @ast_diagnostic({ i64, i64, void*, void* }* %0, i64 0)
  tail call void @ast_destroy({ i64, i64, void*, void* }* %0)
  ret void
}

attributes #0 = { nofree nounwind }
