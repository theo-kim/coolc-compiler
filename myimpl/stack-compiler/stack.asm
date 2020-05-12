	section .data
	align	4
extern _NoGC_Init
extern _NoGC_Collect
extern Object.abort
extern Object.type_name
extern Object.copy
extern String.length
extern String.concat
extern String.substr
extern IO.out_string
extern IO.out_int
extern IO.in_string
extern IO.in_int
extern _dispatch_abort
extern equality_test
extern _case_abort
extern _case_abort2
	global	class_nameTab
	global	Main_protObj
	global	Int_protObj
	global	String_protObj
	global	bool_const0
	global	bool_const1
	global	_int_tag
	global	_bool_tag
	global	_string_tag
_int_tag:
	dd	10
_bool_tag:
	dd	11
_string_tag:
	dd	12
	global	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	dd	_NoGC_Init
	global	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	dd	_NoGC_Collect
	global	_MemMgr_TEST
_MemMgr_TEST:
	dd	0
	dd	-1
str_const37:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const3
	db	"A2I"
	db	0	
	align	4
	dd	-1
str_const36:
	dd	12
	dd	6
	dd	String_dispTab
	dd	int_const6
	db	"String"
	db	0	
	align	4
	dd	-1
str_const35:
	dd	12
	dd	6
	dd	String_dispTab
	dd	int_const4
	db	"Bool"
	db	0	
	align	4
	dd	-1
str_const34:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const3
	db	"Int"
	db	0	
	align	4
	dd	-1
str_const33:
	dd	12
	dd	6
	dd	String_dispTab
	dd	int_const4
	db	"Main"
	db	0	
	align	4
	dd	-1
str_const32:
	dd	12
	dd	8
	dd	String_dispTab
	dd	int_const11
	db	"StackMachine"
	db	0	
	align	4
	dd	-1
str_const31:
	dd	12
	dd	7
	dd	String_dispTab
	dd	int_const10
	db	"StackValue"
	db	0	
	align	4
	dd	-1
str_const30:
	dd	12
	dd	7
	dd	String_dispTab
	dd	int_const9
	db	"StackSwap"
	db	0	
	align	4
	dd	-1
str_const29:
	dd	12
	dd	8
	dd	String_dispTab
	dd	int_const12
	db	"StackAddition"
	db	0	
	align	4
	dd	-1
str_const28:
	dd	12
	dd	8
	dd	String_dispTab
	dd	int_const13
	db	"StackOperation"
	db	0	
	align	4
	dd	-1
str_const27:
	dd	12
	dd	7
	dd	String_dispTab
	dd	int_const8
	db	"StackEnd"
	db	0	
	align	4
	dd	-1
str_const26:
	dd	12
	dd	8
	dd	String_dispTab
	dd	int_const11
	db	"StackCommand"
	db	0	
	align	4
	dd	-1
str_const25:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const2
	db	"IO"
	db	0	
	align	4
	dd	-1
str_const24:
	dd	12
	dd	6
	dd	String_dispTab
	dd	int_const6
	db	"Object"
	db	0	
	align	4
	dd	-1
str_const23:
	dd	12
	dd	8
	dd	String_dispTab
	dd	int_const12
	db	"<basic class>"
	db	0	
	align	4
	dd	-1
str_const22:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"-"
	db	0	
	align	4
	dd	-1
str_const21:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const0
	db	0	
	align	4
	dd	-1
str_const20:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"9"
	db	0	
	align	4
	dd	-1
str_const19:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"8"
	db	0	
	align	4
	dd	-1
str_const18:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"7"
	db	0	
	align	4
	dd	-1
str_const17:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"6"
	db	0	
	align	4
	dd	-1
str_const16:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"5"
	db	0	
	align	4
	dd	-1
str_const15:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"4"
	db	0	
	align	4
	dd	-1
str_const14:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"3"
	db	0	
	align	4
	dd	-1
str_const13:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"2"
	db	0	
	align	4
	dd	-1
str_const12:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"1"
	db	0	
	align	4
	dd	-1
str_const11:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"0"
	db	0	
	align	4
	dd	-1
str_const10:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"x"
	db	0	
	align	4
	dd	-1
str_const9:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	">"
	db	0	
	align	4
	dd	-1
str_const8:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	10
	db	0	
	align	4
	dd	-1
str_const7:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	","
	db	0	
	align	4
	dd	-1
str_const6:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"e"
	db	0	
	align	4
	dd	-1
str_const5:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"d"
	db	0	
	align	4
	dd	-1
str_const4:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"s"
	db	0	
	align	4
	dd	-1
str_const3:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const1
	db	"+"
	db	0	
	align	4
	dd	-1
str_const2:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const3
	db	"END"
	db	0	
	align	4
	dd	-1
str_const1:
	dd	12
	dd	8
	dd	String_dispTab
	dd	int_const12
	db	"Empty Command"
	db	0	
	align	4
	dd	-1
str_const0:
	dd	12
	dd	7
	dd	String_dispTab
	dd	int_const10
	db	"/dev/fd/63"
	db	0	
	align	4
	dd	-1
int_const13:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	14
	dd	-1
int_const12:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	13
	dd	-1
int_const11:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	12
	dd	-1
int_const10:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	10
	dd	-1
int_const9:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	9
	dd	-1
int_const8:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	8
	dd	-1
int_const7:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	7
	dd	-1
int_const6:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	6
	dd	-1
int_const5:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	5
	dd	-1
int_const4:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	4
	dd	-1
int_const3:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	3
	dd	-1
int_const2:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	2
	dd	-1
int_const1:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	1
	dd	-1
int_const0:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	0
	dd	-1
bool_const0:
	dd	11
	dd	4
	dd	Bool_dispTab
	dd	0
	dd	-1
bool_const1:
	dd	11
	dd	4
	dd	Bool_dispTab
	dd	1
class_nameTab:
	dd	str_const24
	dd	str_const25
	dd	str_const26
	dd	str_const27
	dd	str_const28
	dd	str_const29
	dd	str_const30
	dd	str_const31
	dd	str_const32
	dd	str_const33
	dd	str_const34
	dd	str_const35
	dd	str_const36
	dd	str_const37
class_objTab:
	dd	Object_protObj
	dd	Object_init
	dd	IO_protObj
	dd	IO_init
	dd	StackCommand_protObj
	dd	StackCommand_init
	dd	StackEnd_protObj
	dd	StackEnd_init
	dd	StackOperation_protObj
	dd	StackOperation_init
	dd	StackAddition_protObj
	dd	StackAddition_init
	dd	StackSwap_protObj
	dd	StackSwap_init
	dd	StackValue_protObj
	dd	StackValue_init
	dd	StackMachine_protObj
	dd	StackMachine_init
	dd	Main_protObj
	dd	Main_init
	dd	Int_protObj
	dd	Int_init
	dd	Bool_protObj
	dd	Bool_init
	dd	String_protObj
	dd	String_init
	dd	A2I_protObj
	dd	A2I_init
global Object_dispTab
Object_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
global A2I_dispTab
A2I_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	A2I.c2i
	dd	A2I.i2c
	dd	A2I.a2i
	dd	A2I.a2i_aux
	dd	A2I.i2a
	dd	A2I.i2a_aux
global String_dispTab
String_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	String.length
	dd	String.concat
	dd	String.substr
global Bool_dispTab
Bool_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
global Int_dispTab
Int_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
global IO_dispTab
IO_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
global Main_dispTab
Main_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	Main.promptLine
	dd	Main.main
global StackMachine_dispTab
StackMachine_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackMachine.push
	dd	StackMachine.displayStack
	dd	StackMachine.evaluate
global StackCommand_dispTab
StackCommand_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackCommand.getNext
	dd	StackCommand.setNext
	dd	StackCommand.display
global StackValue_dispTab
StackValue_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackCommand.getNext
	dd	StackCommand.setNext
	dd	StackValue.display
	dd	StackValue.setValue
global StackOperation_dispTab
StackOperation_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackCommand.getNext
	dd	StackCommand.setNext
	dd	StackCommand.display
global StackSwap_dispTab
StackSwap_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackCommand.getNext
	dd	StackCommand.setNext
	dd	StackSwap.display
	dd	StackSwap.do
global StackAddition_dispTab
StackAddition_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackCommand.getNext
	dd	StackCommand.setNext
	dd	StackAddition.display
	dd	StackAddition.do
global StackEnd_dispTab
StackEnd_dispTab:
	dd	Object.abort
	dd	Object.type_name
	dd	Object.copy
	dd	IO.out_string
	dd	IO.out_int
	dd	IO.in_string
	dd	IO.in_int
	dd	StackCommand.getNext
	dd	StackCommand.setNext
	dd	StackEnd.display
	dd	-1
Object_protObj:
	dd	0
	dd	3
	dd	Object_dispTab
	dd	-1
A2I_protObj:
	dd	13
	dd	3
	dd	A2I_dispTab
	dd	-1
String_protObj:
	dd	12
	dd	5
	dd	String_dispTab
	dd	int_const0
	dd	0
	dd	-1
Bool_protObj:
	dd	11
	dd	4
	dd	Bool_dispTab
	dd	0
	dd	-1
Int_protObj:
	dd	10
	dd	4
	dd	Int_dispTab
	dd	0
	dd	-1
IO_protObj:
	dd	1
	dd	3
	dd	IO_dispTab
	dd	-1
Main_protObj:
	dd	9
	dd	5
	dd	Main_dispTab
	dd	0
	dd	str_const21
	dd	-1
StackMachine_protObj:
	dd	8
	dd	6
	dd	StackMachine_dispTab
	dd	0
	dd	0
	dd	0
	dd	-1
StackCommand_protObj:
	dd	2
	dd	4
	dd	StackCommand_dispTab
	dd	0
	dd	-1
StackValue_protObj:
	dd	7
	dd	5
	dd	StackValue_dispTab
	dd	0
	dd	str_const21
	dd	-1
StackOperation_protObj:
	dd	4
	dd	4
	dd	StackOperation_dispTab
	dd	0
	dd	-1
StackSwap_protObj:
	dd	6
	dd	5
	dd	StackSwap_dispTab
	dd	0
	dd	0
	dd	-1
StackAddition_protObj:
	dd	5
	dd	7
	dd	StackAddition_dispTab
	dd	0
	dd	0
	dd	int_const0
	dd	0
	dd	-1
StackEnd_protObj:
	dd	3
	dd	4
	dd	StackEnd_dispTab
	dd	0
	global	heap_start
heap_start:
	dd	0
	section .text
	global	Main_init
	global	Int_init
	global	String_init
	global	Bool_init
	global	Main.main
Object_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
A2I_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	Object_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
String_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	Object_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
Bool_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	Object_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
Int_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	Object_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
IO_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	Object_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
Main_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	IO_init
	mov	eax, StackMachine_protObj
	call	Object.copy
	call	StackMachine_init
	mov	[ecx+12], eax
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackMachine_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	IO_init
	mov	eax, StackEnd_protObj
	call	Object.copy
	call	StackEnd_init
	mov	[ecx+12], eax
	mov	eax, [ecx+12]
	mov	[ecx+16], eax
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackCommand_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	IO_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackValue_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	StackCommand_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackOperation_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	StackCommand_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackSwap_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	StackOperation_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackAddition_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	StackOperation_init
	mov	eax, A2I_protObj
	call	Object.copy
	call	A2I_init
	mov	[ecx+24], eax
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackEnd_init:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	call	StackCommand_init
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
A2I.c2i:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const11
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label2
	mov	ebx, bool_const0
	call	equality_test
label2:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label0
	mov	eax, int_const0
	jmp	label1
label0:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const12
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label5
	mov	ebx, bool_const0
	call	equality_test
label5:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label3
	mov	eax, int_const1
	jmp	label4
label3:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const13
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label8
	mov	ebx, bool_const0
	call	equality_test
label8:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label6
	mov	eax, int_const2
	jmp	label7
label6:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const14
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label11
	mov	ebx, bool_const0
	call	equality_test
label11:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label9
	mov	eax, int_const3
	jmp	label10
label9:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const15
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label14
	mov	ebx, bool_const0
	call	equality_test
label14:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label12
	mov	eax, int_const4
	jmp	label13
label12:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const16
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label17
	mov	ebx, bool_const0
	call	equality_test
label17:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label15
	mov	eax, int_const5
	jmp	label16
label15:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const17
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label20
	mov	ebx, bool_const0
	call	equality_test
label20:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label18
	mov	eax, int_const6
	jmp	label19
label18:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const18
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label23
	mov	ebx, bool_const0
	call	equality_test
label23:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label21
	mov	eax, int_const7
	jmp	label22
label21:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const19
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label26
	mov	ebx, bool_const0
	call	equality_test
label26:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label24
	mov	eax, int_const8
	jmp	label25
label24:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const20
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label29
	mov	ebx, bool_const0
	call	equality_test
label29:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label27
	mov	eax, int_const9
	jmp	label28
label27:
	mov	eax, ecx
	cmp	eax, 0
	jne	label30
	mov	eax, str_const0
	mov	esi, 174
	call	_dispatch_abort
label30:
	mov	esi, [eax+8]
	mov	esi, [esi+0]
	call	esi
	mov	eax, int_const0
label28:
label25:
label22:
label19:
label16:
label13:
label10:
label7:
label4:
label1:
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
A2I.i2c:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const0
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label33
	mov	ebx, bool_const0
	call	equality_test
label33:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label31
	mov	eax, str_const11
	jmp	label32
label31:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const1
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label36
	mov	ebx, bool_const0
	call	equality_test
label36:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label34
	mov	eax, str_const12
	jmp	label35
label34:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const2
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label39
	mov	ebx, bool_const0
	call	equality_test
label39:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label37
	mov	eax, str_const13
	jmp	label38
label37:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const3
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label42
	mov	ebx, bool_const0
	call	equality_test
label42:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label40
	mov	eax, str_const14
	jmp	label41
label40:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const4
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label45
	mov	ebx, bool_const0
	call	equality_test
label45:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label43
	mov	eax, str_const15
	jmp	label44
label43:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const5
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label48
	mov	ebx, bool_const0
	call	equality_test
label48:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label46
	mov	eax, str_const16
	jmp	label47
label46:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const6
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label51
	mov	ebx, bool_const0
	call	equality_test
label51:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label49
	mov	eax, str_const17
	jmp	label50
label49:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const7
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label54
	mov	ebx, bool_const0
	call	equality_test
label54:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label52
	mov	eax, str_const18
	jmp	label53
label52:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const8
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label57
	mov	ebx, bool_const0
	call	equality_test
label57:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label55
	mov	eax, str_const19
	jmp	label56
label55:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const9
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label60
	mov	ebx, bool_const0
	call	equality_test
label60:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label58
	mov	eax, str_const20
	jmp	label59
label58:
	mov	eax, ecx
	cmp	eax, 0
	jne	label61
	mov	eax, str_const0
	mov	esi, 192
	call	_dispatch_abort
label61:
	mov	esi, [eax+8]
	mov	esi, [esi+0]
	call	esi
	mov	eax, str_const21
label59:
label56:
label53:
label50:
label47:
label44:
label41:
label38:
label35:
label32:
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
A2I.a2i:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label65
	mov	eax, str_const0
	mov	esi, 205
	call	_dispatch_abort
label65:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	mov	[ebp+-4], eax
	mov	edi, int_const0
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label64
	mov	ebx, bool_const0
	call	equality_test
label64:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label62
	mov	eax, int_const0
	jmp	label63
label62:
	mov	eax, int_const0
	push	eax
	mov	eax, int_const1
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label69
	mov	eax, str_const0
	mov	esi, 206
	call	_dispatch_abort
label69:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	mov	[ebp+-4], eax
	mov	edi, str_const22
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label68
	mov	ebx, bool_const0
	call	equality_test
label68:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label66
	mov	eax, int_const1
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label70
	mov	eax, str_const0
	mov	esi, 206
	call	_dispatch_abort
label70:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	mov	[ebp+-4], eax
	mov	eax, int_const1
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	sub	esi, edi
	mov	[eax+12], esi
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label71
	mov	eax, str_const0
	mov	esi, 206
	call	_dispatch_abort
label71:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label72
	mov	eax, str_const0
	mov	esi, 206
	call	_dispatch_abort
label72:
	mov	esi, [eax+8]
	mov	esi, [esi+24]
	call	esi
	call	Object.copy
	mov	esi, [eax+12]
	neg	esi
	mov	[eax+12], esi
	jmp	label67
label66:
	mov	eax, int_const0
	push	eax
	mov	eax, int_const1
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label76
	mov	eax, str_const0
	mov	esi, 207
	call	_dispatch_abort
label76:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	mov	[ebp+-4], eax
	mov	edi, str_const3
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label75
	mov	ebx, bool_const0
	call	equality_test
label75:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label73
	mov	eax, int_const1
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label77
	mov	eax, str_const0
	mov	esi, 207
	call	_dispatch_abort
label77:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	mov	[ebp+-4], eax
	mov	eax, int_const1
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	sub	esi, edi
	mov	[eax+12], esi
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label78
	mov	eax, str_const0
	mov	esi, 207
	call	_dispatch_abort
label78:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label79
	mov	eax, str_const0
	mov	esi, 207
	call	_dispatch_abort
label79:
	mov	esi, [eax+8]
	mov	esi, [esi+24]
	call	esi
	jmp	label74
label73:
	mov	eax, [ebp+8]
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label80
	mov	eax, str_const0
	mov	esi, 208
	call	_dispatch_abort
label80:
	mov	esi, [eax+8]
	mov	esi, [esi+24]
	call	esi
label74:
label67:
label63:
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
A2I.a2i_aux:
	push	ebp
	mov	ebp, esp
	add	esp, -16
	push	ecx
	mov	ecx, eax
	push	ebx
	push	ecx
	push	edx
	push	esi
	mov	eax, int_const0
	mov	[ebp+-16], eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label81
	mov	eax, str_const0
	mov	esi, 219
	call	_dispatch_abort
label81:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	mov	[ebp+-12], eax
	mov	eax, int_const0
	mov	[ebp+-8], eax
label82:
	mov	eax, [ebp+-8]
	mov	[ebp+-4], eax
	mov	eax, [ebp+-12]
	mov	esi, [ebp+-4]
	mov	esi, [esi+12]
	mov	edi, [eax+12]
	mov	eax, bool_const1
	cmp	esi, edi
	jl	label84
	mov	eax, bool_const0
label84:
	mov	esi, [eax+12]
	cmp	esi, 0
	je	label83
	mov	eax, [ebp+-16]
	mov	[ebp+-4], eax
	mov	eax, int_const10
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	push	eax
	push	edx
	mov	eax, esi
	mul	edi
	mov	esi, eax
	pop	edx
	pop	eax
	mov	[eax+12], esi
	mov	[ebp+-4], eax
	mov	eax, [ebp+-8]
	push	eax
	mov	eax, int_const1
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label85
	mov	eax, str_const0
	mov	esi, 223
	call	_dispatch_abort
label85:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label86
	mov	eax, str_const0
	mov	esi, 223
	call	_dispatch_abort
label86:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	add	esi, edi
	mov	[eax+12], esi
	mov	[ebp+-16], eax
	mov	eax, [ebp+-8]
	mov	[ebp+-4], eax
	mov	eax, int_const1
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	add	esi, edi
	mov	[eax+12], esi
	mov	[ebp+-8], eax
	jmp	label82
label83:
	mov	eax, 0
	mov	eax, [ebp+-16]
	pop	esi
	pop	edx
	pop	ecx
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
A2I.i2a:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, int_const0
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label89
	mov	ebx, bool_const0
	call	equality_test
label89:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label87
	mov	eax, str_const11
	jmp	label88
label87:
	mov	eax, int_const0
	mov	[ebp+-4], eax
	mov	eax, [ebp+8]
	mov	esi, [ebp+-4]
	mov	esi, [esi+12]
	mov	edi, [eax+12]
	mov	eax, bool_const1
	cmp	esi, edi
	jl	label92
	mov	eax, bool_const0
label92:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label90
	mov	eax, [ebp+8]
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label93
	mov	eax, str_const0
	mov	esi, 240
	call	_dispatch_abort
label93:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	jmp	label91
label90:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	eax, int_const1
	call	Object.copy
	mov	esi, [eax+12]
	neg	esi
	mov	[eax+12], esi
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	push	eax
	push	edx
	mov	eax, esi
	mul	edi
	mov	esi, eax
	pop	edx
	pop	eax
	mov	[eax+12], esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label94
	mov	eax, str_const0
	mov	esi, 241
	call	_dispatch_abort
label94:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	push	eax
	mov	eax, str_const22
	cmp	eax, 0
	jne	label95
	mov	eax, str_const0
	mov	esi, 241
	call	_dispatch_abort
label95:
	mov	esi, [eax+8]
	mov	esi, [esi+16]
	call	esi
label91:
label88:
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
A2I.i2a_aux:
	push	ebp
	mov	ebp, esp
	add	esp, -12
	push	ecx
	mov	ecx, eax
	push	ebx
	push	ecx
	push	edx
	mov	eax, [ebp+8]
	mov	[ebp+-12], eax
	mov	edi, int_const0
	mov	esi, [ebp+-12]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label98
	mov	ebx, bool_const0
	call	equality_test
label98:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label96
	mov	eax, str_const21
	jmp	label97
label96:
	mov	eax, [ebp+8]
	mov	[ebp+-12], eax
	mov	eax, int_const10
	call	Object.copy
	mov	esi, [ebp+-12]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	push	eax
	push	edx
	xor	edx, edx
	mov	eax, esi
	cdq
	idiv	edi
	mov	esi, eax
	pop	edx
	pop	eax
	mov	[eax+12], esi
	mov	[ebp+-12], eax
	mov	eax, [ebp+8]
	mov	[ebp+-8], eax
	mov	eax, [ebp+-12]
	mov	[ebp+-4], eax
	mov	eax, int_const10
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	push	eax
	push	edx
	mov	eax, esi
	mul	edi
	mov	esi, eax
	pop	edx
	pop	eax
	mov	[eax+12], esi
	call	Object.copy
	mov	esi, [ebp+-8]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	sub	esi, edi
	mov	[eax+12], esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label99
	mov	eax, str_const0
	mov	esi, 251
	call	_dispatch_abort
label99:
	mov	esi, [eax+8]
	mov	esi, [esi+16]
	call	esi
	push	eax
	mov	eax, [ebp+-12]
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label100
	mov	eax, str_const0
	mov	esi, 251
	call	_dispatch_abort
label100:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	cmp	eax, 0
	jne	label101
	mov	eax, str_const0
	mov	esi, 251
	call	_dispatch_abort
label101:
	mov	esi, [eax+8]
	mov	esi, [esi+16]
	call	esi
label97:
	pop	edx
	pop	ecx
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
Main.promptLine:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, str_const9
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label102
	mov	eax, str_const0
	mov	esi, 140
	call	_dispatch_abort
label102:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	cmp	eax, 0
	jne	label103
	mov	eax, str_const0
	mov	esi, 140
	call	_dispatch_abort
label103:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
Main.main:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
label104:
	mov	eax, ecx
	cmp	eax, 0
	jne	label108
	mov	eax, str_const0
	mov	esi, 144
	call	_dispatch_abort
label108:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	mov	[ecx+16], eax
	mov	[ebp+-4], eax
	mov	edi, str_const10
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label107
	mov	ebx, bool_const0
	call	equality_test
label107:
	mov	esi, [eax+12]
	mov	eax, bool_const1
	cmp	esi, 0
	jz	label106
	mov	eax, bool_const0
label106:
	mov	esi, [eax+12]
	cmp	esi, 0
	je	label105
	mov	eax, str_const8
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label109
	mov	eax, str_const0
	mov	esi, 146
	call	_dispatch_abort
label109:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	mov	eax, [ecx+16]
	push	eax
	mov	eax, [ecx+12]
	cmp	eax, 0
	jne	label110
	mov	eax, str_const0
	mov	esi, 146
	call	_dispatch_abort
label110:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	jmp	label104
label105:
	mov	eax, 0
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackMachine.push:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const3
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label113
	mov	ebx, bool_const0
	call	equality_test
label113:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label111
	mov	eax, [ecx+16]
	push	eax
	mov	eax, StackAddition_protObj
	call	Object.copy
	call	StackAddition_init
	cmp	eax, 0
	jne	label114
	mov	eax, str_const0
	mov	esi, 96
	call	_dispatch_abort
label114:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	mov	[ecx+16], eax
	jmp	label112
label111:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const4
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label117
	mov	ebx, bool_const0
	call	equality_test
label117:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label115
	mov	eax, [ecx+16]
	push	eax
	mov	eax, StackSwap_protObj
	call	Object.copy
	call	StackSwap_init
	cmp	eax, 0
	jne	label118
	mov	eax, str_const0
	mov	esi, 98
	call	_dispatch_abort
label118:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	mov	[ecx+16], eax
	jmp	label116
label115:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const5
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label121
	mov	ebx, bool_const0
	call	equality_test
label121:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label119
	mov	eax, ecx
	cmp	eax, 0
	jne	label122
	mov	eax, str_const0
	mov	esi, 100
	call	_dispatch_abort
label122:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	mov	eax, [ecx+16]
	jmp	label120
label119:
	mov	eax, [ebp+8]
	mov	[ebp+-4], eax
	mov	edi, str_const6
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label125
	mov	ebx, bool_const0
	call	equality_test
label125:
	mov	esi, [eax+12]
	cmp	esi, 0
	jz	label123
	mov	eax, ecx
	cmp	eax, 0
	jne	label126
	mov	eax, str_const0
	mov	esi, 102
	call	_dispatch_abort
label126:
	mov	esi, [eax+8]
	mov	esi, [esi+36]
	call	esi
	mov	eax, [ecx+16]
	jmp	label124
label123:
	mov	eax, [ecx+16]
	push	eax
	mov	eax, [ebp+8]
	push	eax
	mov	eax, StackValue_protObj
	call	Object.copy
	call	StackValue_init
	cmp	eax, 0
	jne	label127
	mov	eax, str_const0
	mov	esi, 104
	call	_dispatch_abort
label127:
	mov	esi, [eax+8]
	mov	esi, [esi+40]
	call	esi
	cmp	eax, 0
	jne	label128
	mov	eax, str_const0
	mov	esi, 104
	call	_dispatch_abort
label128:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	mov	[ecx+16], eax
label124:
label120:
label116:
label112:
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
StackMachine.displayStack:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ecx+16]
	mov	[ecx+20], eax
	mov	eax, [ecx+20]
	cmp	eax, 0
	jne	label129
	mov	eax, str_const0
	mov	esi, 112
	call	_dispatch_abort
label129:
	mov	esi, [eax+8]
	mov	esi, [esi+36]
	call	esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label130
	mov	eax, str_const0
	mov	esi, 112
	call	_dispatch_abort
label130:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
label131:
	mov	eax, [ecx+20]
	cmp	eax, 0
	jne	label135
	mov	eax, str_const0
	mov	esi, 113
	call	_dispatch_abort
label135:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	mov	[ecx+20], eax
	mov	[ebp+-4], eax
	mov	edi, [ecx+12]
	mov	esi, [ebp+-4]
	mov	eax, bool_const1
	cmp	esi, edi
	je	label134
	mov	ebx, bool_const0
	call	equality_test
label134:
	mov	esi, [eax+12]
	mov	eax, bool_const1
	cmp	esi, 0
	jz	label133
	mov	eax, bool_const0
label133:
	mov	esi, [eax+12]
	cmp	esi, 0
	je	label132
	mov	eax, str_const7
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label136
	mov	eax, str_const0
	mov	esi, 115
	call	_dispatch_abort
label136:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	mov	eax, [ecx+20]
	cmp	eax, 0
	jne	label137
	mov	eax, str_const0
	mov	esi, 115
	call	_dispatch_abort
label137:
	mov	esi, [eax+8]
	mov	esi, [esi+36]
	call	esi
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label138
	mov	eax, str_const0
	mov	esi, 115
	call	_dispatch_abort
label138:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	jmp	label131
label132:
	mov	eax, 0
	mov	eax, str_const8
	push	eax
	mov	eax, ecx
	cmp	eax, 0
	jne	label139
	mov	eax, str_const0
	mov	esi, 117
	call	_dispatch_abort
label139:
	mov	esi, [eax+8]
	mov	esi, [esi+12]
	call	esi
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackMachine.evaluate:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label141
	mov	eax, str_const0
	mov	esi, 123
	call	_case_abort2
label141:
	mov	edi, [eax+0]
	cmp	edi, 6
	jl	label142
	cmp	edi, 6
	jg	label142
	mov	[ebp+-4], eax
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label143
	mov	eax, str_const0
	mov	esi, 127
	call	_dispatch_abort
label143:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	push	eax
	mov	eax, [ebp+-4]
	cmp	eax, 0
	jne	label144
	mov	eax, str_const0
	mov	esi, 127
	call	_dispatch_abort
label144:
	mov	esi, [eax+8]
	mov	esi, [esi+40]
	call	esi
	mov	[ecx+16], eax
	jmp	label140
label142:
	cmp	edi, 5
	jl	label145
	cmp	edi, 5
	jg	label145
	mov	[ebp+-4], eax
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label146
	mov	eax, str_const0
	mov	esi, 125
	call	_dispatch_abort
label146:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	push	eax
	mov	eax, [ebp+-4]
	cmp	eax, 0
	jne	label147
	mov	eax, str_const0
	mov	esi, 125
	call	_dispatch_abort
label147:
	mov	esi, [eax+8]
	mov	esi, [esi+40]
	call	esi
	mov	[ecx+16], eax
	jmp	label140
label145:
	cmp	edi, 0
	jl	label148
	cmp	edi, 13
	jg	label148
	mov	[ebp+-4], eax
	mov	eax, ecx
	jmp	label140
label148:
	call	_case_abort
label140:
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackCommand.getNext:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, [ecx+12]
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackCommand.setNext:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, [ebp+8]
	mov	[ecx+12], eax
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
StackCommand.display:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, str_const1
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackValue.setValue:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, [ebp+8]
	mov	[ecx+16], eax
	mov	eax, ecx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
StackValue.display:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, [ecx+16]
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackSwap.do:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label149
	mov	eax, str_const0
	mov	esi, 62
	call	_dispatch_abort
label149:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	mov	[ecx+16], eax
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label150
	mov	eax, str_const0
	mov	esi, 63
	call	_dispatch_abort
label150:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	push	eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label151
	mov	eax, str_const0
	mov	esi, 63
	call	_dispatch_abort
label151:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	mov	eax, [ebp+8]
	push	eax
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label152
	mov	eax, str_const0
	mov	esi, 64
	call	_dispatch_abort
label152:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
StackSwap.display:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, str_const4
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackAddition.do:
	push	ebp
	mov	ebp, esp
	add	esp, -4
	push	ecx
	mov	ecx, eax
	push	ebx
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label153
	mov	eax, str_const0
	mov	esi, 46
	call	_dispatch_abort
label153:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	mov	[ecx+16], eax
	mov	eax, [ebp+8]
	cmp	eax, 0
	jne	label154
	mov	eax, str_const0
	mov	esi, 47
	call	_dispatch_abort
label154:
	mov	esi, [eax+8]
	mov	esi, [esi+36]
	call	esi
	push	eax
	mov	eax, [ecx+24]
	cmp	eax, 0
	jne	label155
	mov	eax, str_const0
	mov	esi, 47
	call	_dispatch_abort
label155:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	mov	[ebp+-4], eax
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label156
	mov	eax, str_const0
	mov	esi, 47
	call	_dispatch_abort
label156:
	mov	esi, [eax+8]
	mov	esi, [esi+36]
	call	esi
	push	eax
	mov	eax, [ecx+24]
	cmp	eax, 0
	jne	label157
	mov	eax, str_const0
	mov	esi, 47
	call	_dispatch_abort
label157:
	mov	esi, [eax+8]
	mov	esi, [esi+20]
	call	esi
	call	Object.copy
	mov	esi, [ebp+-4]
	mov	edi, [eax+12]
	mov	esi, [esi+12]
	add	esi, edi
	mov	[eax+12], esi
	mov	[ecx+20], eax
	mov	eax, [ecx+16]
	cmp	eax, 0
	jne	label158
	mov	eax, str_const0
	mov	esi, 48
	call	_dispatch_abort
label158:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	push	eax
	mov	eax, [ecx+20]
	push	eax
	mov	eax, [ecx+24]
	cmp	eax, 0
	jne	label159
	mov	eax, str_const0
	mov	esi, 48
	call	_dispatch_abort
label159:
	mov	esi, [eax+8]
	mov	esi, [esi+28]
	call	esi
	push	eax
	mov	eax, StackValue_protObj
	call	Object.copy
	call	StackValue_init
	cmp	eax, 0
	jne	label160
	mov	eax, str_const0
	mov	esi, 48
	call	_dispatch_abort
label160:
	mov	esi, [eax+8]
	mov	esi, [esi+40]
	call	esi
	cmp	eax, 0
	jne	label161
	mov	eax, str_const0
	mov	esi, 48
	call	_dispatch_abort
label161:
	mov	esi, [eax+8]
	mov	esi, [esi+32]
	call	esi
	pop	ebx
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	4
StackAddition.display:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, str_const3
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
StackEnd.display:
	push	ebp
	mov	ebp, esp
	add	esp, 0
	push	ecx
	mov	ecx, eax
	mov	eax, str_const2
	pop	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
