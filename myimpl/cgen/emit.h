///////////////////////////////////////////////////////////////////////
//
//  Assembly Code Naming Conventions:
//
//     Dispatch table            <classname>_dispTab
//     Method entry point        <classname>.<method>
//     Class init code           <classname>_init
//     Abort method entry        <classname>.<method>.Abort
//     Prototype object          <classname>_protObj
//     Integer constant          int_const<Symbol>
//     String constant           str_const<Symbol>
//
///////////////////////////////////////////////////////////////////////

#include "stringtab.h"

#define MAXINT  100000000    
#define WORD_SIZE    4
#define LOG_WORD_SIZE 2     // for logical shifts

// Global names
#define CLASSNAMETAB         "class_nameTab"
#define CLASSOBJTAB          "class_objTab"
#define INTTAG               "_int_tag"
#define BOOLTAG              "_bool_tag"
#define STRINGTAG            "_string_tag"
#define HEAP_START           "heap_start"

// Naming conventions
#define DISPTAB_SUFFIX       "_dispTab"
#define METHOD_SEP           "."
#define CLASSINIT_SUFFIX     "_init"
#define PROTOBJ_SUFFIX       "_protObj"
#define OBJECTPROTOBJ        "Object" PROTOBJ_SUFFIX
#define INTCONST_PREFIX      "int_const"
#define STRCONST_PREFIX      "str_const"
#define BOOLCONST_PREFIX     "bool_const"


#define EMPTYSLOT            0
#define LABEL                ":\n"

#define STRINGNAME (char *) "String"
#define INTNAME    (char *) "Int"
#define BOOLNAME   (char *) "Bool"
#define MAINNAME   (char *) "Main"

//
// information about object headers
//
#define DEFAULT_OBJFIELDS   3
#define TAG_OFFSET          0
#define SIZE_OFFSET         1
#define DISPTABLE_OFFSET    2

#define STRING_SLOTS      1
#define INT_SLOTS         1
#define BOOL_SLOTS        1

#define GLOBAL        "\tglobal\t"
#define ALIGN         "\talign\t4\n"
#define WORD          "\tdd\t"

//
// comment symbol in 
// 
#define COMMENT "; "
#define NL      "\n"

// START OF REGISTERS
// => General registers
//      => 32-bit
#define EAX     "eax"
#define EBX     "ebx"
#define ECX     "ecx"
#define EDX     "edx"
//      => 16-bit
#define AX      "ax"
#define BX      "bx"
#define CX      "cx"
#define DX      "dx"
//      => 8-bit
#define AH      "ah"
#define AL      "al"
#define BH      "bh"
#define BL      "bl"
#define CH      "ch"
#define CL      "cl"
#define DH      "dh"
#define DL      "dl"
// => Index registers
//      => 32-bit
#define EDI     "edi"
#define ESI     "esi"
//      => 16-bit
#define DI      "di"
#define SI      "si"
// => Stack registers (32-bit)
#define EBP     "ebp"
#define ESP     "esp"
// => Instruction pointer (32-bit)
#define EIP     "eip"
// END OF REGISTERS

// START OPCODES
// => Arithmetic / Logic operations
//      => Arthimetic operations
#define ADD     "\tadd\t"
#define SUB     "\tsub\t"
#define DEC     "\tdec\t"
#define NEG     "\tneg\t"
//      => Logic operations
#define AND     "\tand\t"
#define OR      "\tor\t"
#define XOR     "\txor\t"
#define TEST    "\ttest\t"
#define CMP     "\tcmp\t"
#define SHL     "\tshl\t"
#define SHR     "\tshr\t"
#define NOT     "\tnot\t"
// => Branches and Jumps
//      => Branches
#define JNE     "\tjne\t"
#define JE      "\tje\t"
#define JLE     "\tjle\t"
#define JL      "\tjl\t"
//      => Jumps
#define JMP     "\tjmp\t"
// => Memory operations
//      => function
#define CALL    "\tcall\t"
#define RET     "\tret\t"
//      => other
#define MOV     "\tmov\t"
#define LODSB   "\tlodsb\t"
#define STODSB  "\tstodsb\t"
//      => stack
#define POP     "\tpop\t"
#define PUSH    "\tpush\t"
// END OPCODES
