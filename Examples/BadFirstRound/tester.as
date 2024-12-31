; Tester for Maman 14.
; Show errors in the first round on the file

	,extern L3

; Checking Label 

Label12345Way12345Too12345Long12345: clr *r4
entry: .string "HI"
123: .data 1,2,3	
digital!@#: add #4, r4
LOOP2: .string "HELLO"
END: stop

; Checking guidance

.stringgggggggggggggggggggggggg "123"
.number 4,5,6
.data
.data	, 5,	12,	-110
.data	5,	,12,	-110
.data 	5	12,	-110
.data	5,	12,	-110,
.data	5,,	12,	-110
.data	5,	12,	a
 .data	5,	12,	-110

.string
.string	abc"
.string		"abc
.string         "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#"
.string	   "abc" "ABC"
.string "abc"

.entry
.entry Label12345Way12345Too12345Long12345
.entry Main L4
Main: .entry L3

.extern
.extern fn1
.extern Label12345Way12345Too12345Long12345
.extern END
FN1: .extern fn1

; Checking instruction

JMP: jmp123 r2
DEL: del *r5
L3: cmp
add , #7, r7
sub Label12345Way12345Too12345Long12345	, *r4
mov #1
add #a,	r3
add #3, r10
add #3,	*r10
add label!@#,	r10
sub #3	r2
sub #3,	,r2
lea #3,	r2
mov r2,	#3
sub *r4,	Label12345Way12345Too12345Long12345
sub #3,	r2 ,#3
not #3
not r3	#3
jmp r3
rts r5
ASASD: cmp #5, #-29
r12: dec *r6
LOOP2: clr r12
 .extern L3
