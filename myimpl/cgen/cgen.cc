
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <map>
#include <sstream>

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;
int curr_lineno;

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  copy_sym    = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);


// Define method for code generation
void program_class::cgen(ostream &os) 
{
  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_load(char *dest_reg, char *source_reg, int offset, ostream& s)
{
  s << MOV << dest_reg << ARG << "[" << source_reg << "+" << offset * WORD_SIZE << "]" << NL;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << MOV << "[" << source_reg << "+" << offset * WORD_SIZE << "]" << ARG << dest_reg << endl;
}

// static void emit_load_imm(char *dest_reg, int val, ostream& s)
// { s << LI << dest_reg << " " << val << endl; }

// static void emit_load_address(char *dest_reg, char *address, ostream& s)
// { s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << MOV << dest_reg << ARG; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s) {
  s << MOV << dest_reg << ARG << source_reg << endl;
}

static void emit_call(char *label, ostream& s) {
  s << CALL << label << NL;
}

static void emit_init_call(Symbol classname, ostream& s) {
  s << CALL << classname << CLASSINIT_SUFFIX << NL;
}

// static void emit_neg(char *dest, char *src1, ostream& s)
// { s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src, ostream& s)
{ s << ADD << dest << ARG << src << NL; }

// static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
// { s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addi(char *dest, int imm, ostream& s)
{ s << ADD << dest << ARG << imm << endl; }

// static void emit_div(char *dest, char *src1, char *src2, ostream& s)
// { s << DIV << dest << " " << src1 << " " << src2 << endl; }

// static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
// { s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src, ostream& s)
{ s << SUB << dest << ARG << src << endl; }

static void emit_cmp(char *lreg, char *rreg, ostream& s) {
  s << CMP << lreg << ARG << rreg << endl;
}

static void emit_cmpi(char *reg, int imm, ostream& s) {
  s << CMP << reg << ARG << imm << endl;
}

static void emit_jump(char *jump_type, int label, ostream& s) {
  s << jump_type;
  emit_label_ref(label, s);
  s << NL;
}

// static void emit_sll(char *dest, char *src1, int num, ostream& s)
// { s << SLL << dest << " " << src1 << " " << num << endl; }

// static void emit_jalr(char *dest, ostream& s)
// { s << JALR << "\t" << dest << endl; }

// static void emit_jal(char *address,ostream &s)
// { s << JAL << address << endl; }

// static void emit_return(ostream& s)
// { s << RET << endl; }

// static void emit_gc_assign(ostream& s)
// { s << JAL << "_GenGC_Assign" << endl; }

// static void emit_disptable_ref(Symbol sym, ostream& s)
// {  s << sym << DISPTAB_SUFFIX; }

static void emit_method_call(Symbol classname, Symbol methodname, ostream& s) {
  s << CALL;
  emit_method_ref(classname, methodname, s);
  s << NL;
}

// static void emit_label_def(int l, ostream &s) {
//   // TODO
//   emit_label_ref(l,s);
//   s << ":" << endl;
// }

// static void emit_beqz(char *source, int label, ostream &s)
// {
//   s << BEQZ << source << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_beq(char *src1, char *src2, int label, ostream &s)
// {
//   s << BEQ << src1 << " " << src2 << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_bne(char *src1, char *src2, int label, ostream &s)
// {
//   s << BNE << src1 << " " << src2 << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_bleq(char *src1, char *src2, int label, ostream &s)
// {
//   s << BLEQ << src1 << " " << src2 << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_blt(char *src1, char *src2, int label, ostream &s)
// {
//   s << BLT << src1 << " " << src2 << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_blti(char *src1, int imm, int label, ostream &s)
// {
//   s << BLT << src1 << " " << imm << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_bgti(char *src1, int imm, int label, ostream &s)
// {
//   s << BGT << src1 << " " << imm << " ";
//   emit_label_ref(label,s);
//   s << endl;
// }

// static void emit_branch(int l, ostream& s)
// {
//   s << BRANCH;
//   emit_label_ref(l,s);
//   s << endl;
// }

// Push a register on the stack. The stack grows towards smaller addresses.
static void emit_push(char *reg, ostream& str) {
  str << PUSH << reg << NL;
}

// Pop the top of the stack onto a register
static void emit_pop(char *reg, ostream& str) {
  str << POP << reg << NL;
}

static void emit_stackframe(ostream& s, int vars) {
  emit_push(EBP, s); // push base pointer
  emit_move(EBP, ESP, s); // Move the stack pointer to the base pointer
  emit_addi(ESP, -(vars * 4), s); // Allocate space for local variables
}

static void emit_stackframe_close(ostream& s) {
  emit_move(ESP, EBP, s); // Move the stack pointer to the base of the frame
  emit_pop(EBP, s); // restore the base pointer
  s << RET << EMPTYSLOT << NL;
}

static void emit_self_arg(ostream& s) {
  emit_push(ECX, s);
  emit_move(ECX, EAX, s);
}

static void emit_return_reg(char *reg, ostream& s) {
  emit_move(EAX, reg, s);
  emit_pop(reg, s);
}

static void emit_initialize_class(Symbol classname, ostream& s) {
  // move address of the prototype into the argument
  s << MOV << EAX << ARG;
  emit_protobj_ref(classname, s);
  s << NL;
  // Copy prototype
  s << CALL;
  emit_method_ref(Object, copy_sym, s);
  s << NL;
  // Call initialization function
  emit_init_call(classname, s);
}

// //
// // Fetch the integer value in an Int object.
// // Emits code to fetch the integer value of the Integer object pointed
// // to by register source into the register dest
// //
// static void emit_fetch_int(char *dest, char *source, ostream& s)
// { emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

// //
// // Emits code to store the integer value contained in register source
// // into the Integer object pointed to by dest.
// //
// static void emit_store_int(char *source, char *dest, ostream& s)
// { emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


// static void emit_test_collector(ostream &s)
// {
//   emit_push(ACC, s);
//   emit_move(ACC, SP, s); // stack end
//   emit_move(A1, ZERO, s); // allocate nothing
//   s << JAL << gc_collect_names[cgen_Memmgr] << endl;
//   emit_addiu(SP,SP,4,s);
//   emit_load(ACC,0,SP,s);
// }

// static void emit_gc_check(char *source, ostream &s)
// {
//   /*if (source != A1)*/ emit_move(A1, source, s);
//   s << JAL << "_gc_check" << endl;
// }


///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

// Strings
void StringEntry::code_ref(ostream& s) {
  s << STRCONST_PREFIX << index;
}

// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
void StringEntry::code_def(ostream& s, int stringclasstag) {
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD << STRINGNAME << DISPTAB_SUFFIX << endl;        // dispatch table
      s << WORD;  lensym->code_ref(s);  s << endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
void StrTable::code_string_table(ostream& s, int stringclasstag) {
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

// Ints
void IntEntry::code_ref(ostream &s) {
  s << INTCONST_PREFIX << index;
}

// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
void IntEntry::code_def(ostream &s, int intclasstag) {
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD << INTNAME << DISPTAB_SUFFIX << endl;       // dispatch table
      s << WORD << str << endl;                           // integer value
}


// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
void IntTable::code_string_table(ostream &s, int intclasstag) {
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}

// Bools
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const {
  s << BOOLCONST_PREFIX << val;
}
  

// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
void BoolConst::code_def(ostream& s, int boolclasstag) {
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD << BOOLNAME << DISPTAB_SUFFIX << endl;        // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//  Emit code to start the .data segment and to declare the global names.
void CgenClassTable::code_extern() {
  // Garbage collector initialization
  str << EXTERN << "_NoGC_Init" << NL;
  str << EXTERN << "_NoGC_Collect" << NL;
  // Built in class functions
  CgenNodeP objectNode = lookup(Object);
  CgenNodeP stringNode = lookup(Str);
  CgenNodeP ioNode = lookup(IO);
  CgenNodeP builtIn[] = { objectNode, stringNode, ioNode };
  for (int i = 0; i < 3; ++i) {
    CgenNodeP node = builtIn[i];
    for (int j = 0; j < node->features->len(); ++j) {
      if (node->features->nth(j)->get_method() != nullptr)
        str << EXTERN << node->name << METHOD_SEP 
          << node->features->nth(j)->get_method() << NL;
    }
  }
  // Metafunctions
  str << EXTERN << "_dispatch_abort" << NL;
  str << EXTERN << "equality_test" << NL;
  str << EXTERN << "_case_abort" << NL;
  str << EXTERN << "_case_abort2" << NL;
}

void CgenClassTable::code_global_data() {
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\tsection .data\n" << ALIGN;
  // Define external built in functions after being installed
  code_extern();

  // The following global names must be defined first.
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
}


//  Emit code to start the .text segment and to
//  declare the global names.
void CgenClassTable::code_global_text() {
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\tsection .text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

// Code the bool constants (true and false)
void CgenClassTable::code_bools(int boolclasstag) {
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

// Generate GC choice constants (pointers to GC functions)
void CgenClassTable::code_select_gc() {
  // GARBAGE COLLECTOR is disabled in this implementation
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}

// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
void CgenClassTable::code_constants() {
  // Add constants that are required by the code generator.
  stringtable.add_string(""); // Initial value of strings
  inttable.add_string("0"); // Initial value of integers

  stringtable.code_string_table(str, stringclasstag);
  inttable.code_string_table(str, intclasstag);
  code_bools(boolclasstag);
}

void CgenClassTable::code_obj_tab() {
  str << CLASSOBJTAB << LABEL;
  lookup(Object)->code_obj_tab_entry(str);
}

void CgenClassTable::code_name_tab() {
  str << CLASSNAMETAB << LABEL;
  lookup(Object)->code_name_tab_entry(str);
}

// Code the dispatch table for each class in the program for
// the class methods
void CgenClassTable::code_dispatch_tables() {
  // Go through the class tree (starting from Object because Object is the root)
  lookup(Object)->code_dispatch_table(str); // this is recursive
}

// Code the object prototypes for the objects defined in this program
void CgenClassTable::code_object_prototypes() {
  // Go through the class tree (starting from Object because Object is the root)
  lookup(Object)->code_prototype(str, 0);
}

// Code the initializer functions for each object defined in this program
void CgenClassTable::code_object_initializers() {
  // Go through the class tree (starting from Object because Object is the root)
  lookup(Object)->code_initializer(str);
}

// Code the methods for each object defined in this program
void CgenClassTable::code_object_methods() {
  // Go through the class tree (starting from Object because Object is the root)
  lookup(Object)->code_methods(str);
}

// Constructor for the classtable, this will actually produce the code
CgenClassTable::CgenClassTable(Classes _classes, ostream& s) : 
  nds(NULL),
  str(s)
{
  stringclasstag = 4;
  intclasstag =    2;
  boolclasstag =   3;

  enterscope();
  if (cgen_debug) cout << "Building CgenClassTable" << endl;
  install_basic_classes();
  install_classes(_classes);
  build_inheritance_tree();

  code();
  exitscope();
}

// Installs the basic classes for the compiler (Object, String, Int, IO)
void CgenClassTable::install_basic_classes() {
  // The tree package uses these globals to annotate the classes built below.
  curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

  // A few special class names are installed in the lookup table but not
  // the class list.  Thus, these classes exist, but are not part of the
  // inheritance hierarchy.
  // No_class serves as the parent of Object and the other special classes.
  // SELF_TYPE is the self class; it cannot be redefined or inherited.
  // prim_slot is a class known to the code generator.
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  // Not needed for x86 implementation
  // addid(prim_slot,
  // new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
	// 		    Basic,this));

  // The Object class has no parent class. Its methods are
  //        cool_abort() : Object    aborts the program
  //        type_name() : Str        returns a string representation of class name
  //        copy() : SELF_TYPE       returns a copy of the object
  //
  // There is no need for method bodies in the basic classes---these
  // are already built in to the runtime system.
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy_sym, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

  // The IO class inherits from Object. Its methods are
  //        out_string(Str) : SELF_TYPE          writes a string to the output
  //        out_int(Int) : SELF_TYPE               "    an int    "  "     "
  //        in_string() : Str                    reads a string from the input
  //        in_int() : Int                         "   an int     "  "     "
  install_class(
    new CgenNode(
      class_(IO, 
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
      filename),	    
    Basic,this));

  // The Int class has no methods and only a single attribute, the
  // "val" for the integer. 
  install_class(
    new CgenNode(
      class_(Int, 
      Object,
            single_Features(attr(val, prim_slot, no_expr())),
      filename),
      Basic,this));

  // Bool also has only the "val" slot.
  install_class(
    new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
    Basic,this));

  // The class Str has a number of slots and operations:
  //       val                                  ???
  //       str_field                            the string itself
  //       length() : Int                       length of the string
  //       concat(arg: Str) : Str               string concatenation
  //       substr(arg: Int, arg2: Int): Str     substring
  install_class(
    new CgenNode(
      class_(Str, 
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat, 
				   single_Formals(formal(arg, Str)),
				   Str, 
				   no_expr()))),
	    single_Features(method(substr, 
				   append_Formals(single_Formals(formal(arg, Int)), 
						  single_Formals(formal(arg2, Int))),
				   Str, 
				   no_expr()))),
	     filename),
        Basic,this));
}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name)) {
    return;
  }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

// CgenClassTable::build_inheritance_tree
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

void CgenClassTable::code() {
  // Global data
  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  // Garbage collector
  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  // Constants
  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();

  // Code the name table
  if (cgen_debug) cout << "coding name table" << endl;
  code_name_tab();

  // Code the object table
  if (cgen_debug) cout << "coding object table" << endl;
  code_obj_tab();

  // Dispatch tables
  if (cgen_debug) cout << "dispatch tables" << endl;
  code_dispatch_tables();

  // Prototypes
  if (cgen_debug) cout << "object prototypes" << endl;
  code_object_prototypes();

  // Global text (heap etc.)
  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();

  // Object initializations
  if (cgen_debug) cout << "coding object initializations" << endl;
  code_object_initializers();

  // Function definitions
  if (cgen_debug) cout << "coding function definitions" << endl;
  code_object_methods();
}


CgenNodeP CgenClassTable::root() {
   return probe(Object);
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////
CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus)
{ 
   stringtable.add_string(name->get_string());          // Add class name to string table
}

void CgenNode::code_obj_tab_entry(ostream& str) {
  List<CgenNode> *childCursor = children;

  str << WORD << name << PROTOBJ_SUFFIX << NL;
  str << WORD << name << CLASSINIT_SUFFIX << NL;

  // Now generate for the children
  if (childCursor == NULL) return; // Leaf node
  // Iterate through the children depth first
  do {
    childCursor->hd()->code_obj_tab_entry(str);
  } while ((childCursor = childCursor->tl()) != NULL);
}

void CgenNode::code_name_tab_entry(ostream& str) {
  List<CgenNode> *childCursor = children;

  str << WORD;
  stringtable.lookup_string(name->get_string())->code_ref(str);
  str << NL;

  // Now generate for the children
  if (childCursor == NULL) return; // Leaf node
  // Iterate through the children depth first
  do {
    childCursor->hd()->code_name_tab_entry(str);
  } while ((childCursor = childCursor->tl()) != NULL);
}

// Make a dispatch table for each node
void CgenNode::code_dispatch_table(ostream& str) {
  CgenNodeP p = parentnd;
  List<CgenNode> *childCursor = children;
  
  // Insert this function's method to the dispatch table
  for (int i = features->first(); features->more(i); i = features->next(i)) {
     Feature feature = features->nth(i);
     Symbol methodName = feature->get_method();
     if (methodName == nullptr) continue;
     dispatch_table.insert(DispatchTableEntry(methodName, name));
  }
  
  // For inheritted methods
  while (p != NULL) {
    // Add its methods to the dispatch table, existing function names are not inserted
    for (int i = p->features->first(); p->features->more(i); i = p->features->next(i)) {
      Feature feature = p->features->nth(i);
      Symbol methodName = feature->get_method();
      if (methodName == nullptr) continue;
      // Only new method names will add a new entry to the dispatch table
      dispatch_table.insert(DispatchTableEntry(methodName, p->name));
    }
    p = p->parentnd;
  }
  
  // Actually emit the dispatch table
  // Dispatch table header
  str << GLOBAL_NOTAB << name << DISPTAB_SUFFIX << NL;
  str << name << DISPTAB_SUFFIX << LABEL;
  // Write a method for each
  for (DispatchTable::iterator it = dispatch_table.begin(); it != dispatch_table.end(); it++ ) {
    str << WORD << it->second << METHOD_SEP << it->first << NL;
  }
  
  // Now generate for the children
  if (childCursor == NULL) return; // Leaf node
  // Iterate through the children depth first
  do {
    childCursor->hd()->code_dispatch_table(str);
  } while ((childCursor = childCursor->tl()) != NULL);
}

void CgenNode::resolve_attributes(CgenNodeP node, std::stringstream& str, int *len) {
  // go as far upwards as possible and resolve downwards
  if (node == NULL) return;
  // Add the new attribute entries
  resolve_attributes(node->get_parentnd(), str, len);
  for (int i = node->features->first(); node->features->more(i); i = node->features->next(i)) {
    Feature feature = node->features->nth(i);
    Symbol attribute = feature->get_attribute();
    if (attribute == nullptr) continue;
    // Add the attribute to the map
    str << WORD;
    if (attribute == Str)
      stringtable.lookup_string("")->code_ref(str);
    else if (attribute == Int)
      inttable.lookup_string("0")->code_ref(str);
    else if (attribute == Bool)
      falsebool.code_ref(str);
    else
      str << EMPTYSLOT;
    str << NL;
    attributes.insert(AttributeOffsetEntry(feature->get_name(), *len));
    *len += 1;
  }
  return;
}

// Code the prototype of the object (i.e. pointer to dispatch table and attributes)
int CgenNode::code_prototype(ostream& str, int tag) {
  std::stringstream attributes;
  List<CgenNode> *childCursor = children;
  CgenNodeP p = parentnd;
  int prototypeSize = 3;
  // First find all the inheritted attributes top down
  resolve_attributes(this, attributes, &prototypeSize); 

  // Add -1 eye catcher
  str << WORD << "-1" << endl;
  // Add label:
  str << name << PROTOBJ_SUFFIX << LABEL;
  // Add the class tag
  str << WORD << tag << NL;
  // Add the class size
  str << WORD << prototypeSize << NL;
  // Add the dispatch table
  str << WORD << name << DISPTAB_SUFFIX << NL;
  // Add the attributes
  str << attributes.str();

  // Now generate for the children
  if (childCursor == NULL) return tag; // Leaf node
  // Iterate through the children depth first
  do {
    tag = childCursor->hd()->code_prototype(str, tag + 1);
  } while ((childCursor = childCursor->tl()) != NULL);

  return tag;
}

// Code the initializer function for the object
void CgenNode::code_initializer(ostream &str) {
  List<CgenNode> *childCursor = children;
  // First emit the label
  str << name << CLASSINIT_SUFFIX << LABEL;
  // Set up the stackframe
  // TODO: determine max accumulator level (sum of the required arguments for the height of the tree)
  emit_stackframe(str, accumulator_level);
  // Set up the self argument (eax)
  emit_self_arg(str);
  // Call the initializer for the object's parent
  if (get_parentnd() != NULL && get_parentnd()->name != No_class) emit_init_call(parentnd->name, str);
  // Resolve the expressions to initialize the attributes
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    Feature feature = features->nth(i);
    if (feature->get_attribute() == nullptr) continue;
    accumulator_level = 2;
    if (feature->code(str) >= 0) { // Code the feature
      // Move the result (stored in eax) into the destination register
      emit_store(ECX, attributes.find(feature->get_name())->second, EAX, str);
    }
  }
  // Set up the return value (eax)
  emit_move(EAX, ECX, str);
  emit_pop(ECX, str);
  // Close the stackframe and return from the function
  emit_stackframe_close(str);

  // Now generate for the children
  if (childCursor == NULL) return; // Leaf node
  // Iterate through the children depth first
  do {
    childCursor->hd()->code_initializer(str);
  } while ((childCursor = childCursor->tl()) != NULL);

  return;
}

void CgenNode::code_methods(ostream& str) {
  List<CgenNode> *childCursor = children;
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    Feature feature = features->nth(i);
    if (feature->get_method() == nullptr) continue;
    // First emit the label
    str << name << CLASSINIT_SUFFIX << feature->get_name() << LABEL;
    feature->code(str); // emit the method's code
  }
  // Now generate for the children
  if (childCursor == NULL) return; // Leaf node
  // Iterate through the children depth first
  do {
    childCursor->hd()->code_methods(str);
  } while ((childCursor = childCursor->tl()) != NULL);
}

//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
int assign_class::code(ostream &s) {
  // TODO: code an assignment statement (i.e. add a value to an object ID)
  s << COMMENT << "implement assignment" << NL;
  return 0;
}

int static_dispatch_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement static dispatch" << NL;
  return 0;
}

int dispatch_class::code(ostream &s) {
  // load arguments into the stack
  for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
    Expression e = actual->nth(i);
    e->code(s);
    emit_push(EAX, s);
  }
  // resolve the calling expression
  expr->code(s);
  // check for void
  emit_cmpi(EAX, 0, s);
  emit_jump(JNE, label_counter, s);
  // if void, throw runtime error
  emit_move(EAX, "str_const0", s);
  emit_partial_load_address(EAX, s);
  s << curr_lineno << NL;
  emit_call("_dispatch_abort", s);
  // if not void, get the dispatch table for the object
  emit_label_ref(label_counter++, s);
  s << LABEL;
  emit_load(ESI, EAX, DISPTABLE_OFFSET, s);
  // TODO: find the desired function in the dispatch table
  s << COMMENT << "find dispatched function" << NL;
  // call the dispatched function
  emit_call(ESI, s);
  // DONE
  return 0;
}

int cond_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement conditional" << NL;
  return 0;
}

int loop_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement loop" << NL;
  return 0;
}

int typcase_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement case" << NL;
  return 0;
}

int block_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement block" << NL;
  return 0;
}

int let_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement let" << NL;
  return 0;
}

// binary arithmetic operations: stack machine rules
//   - first emit code for each subexpression (result is always stored into eax)
//   - then initialize new int object
//   - perform operation
//   - store value into new int object
void binary_operation(char *op, Expression e1, Expression e2, ostream &s) {
  e1->code(s); // emit subexpression 1
  emit_store(EBP, -accumulator_level, EAX, s); // put result onto the stack
  --accumulator_level; // decrement accumulator for lower actions
  e2->code(s); // emit subexpression 2
  emit_method_call(Object, copy_sym, s); // Copy that object
  // Perform addition
  ++accumulator_level; // increment accumulator
  emit_load(ESI, EBP, -accumulator_level, s);
  emit_load(EDI, EAX, 3, s);
  emit_load(ESI, ESI, 3, s);
  s << op << ESI << ARG << EDI << NL;
  emit_store(EAX, 3, ESI, s); 
}

int plus_class::code(ostream &s) {
  binary_operation(ADD, e1, e2, s);
  return 1;
}

int sub_class::code(ostream &s) {
  binary_operation(SUB, e1, e2, s);
  return 1;
}

int mul_class::code(ostream &s) {
  // TODO: figure out how multiplication works in x86
  s << COMMENT << "implement mul" << NL;
  return 0;
}

int divide_class::code(ostream &s) {
  // TODO: figure out how integer division works in x86
  s << COMMENT << "implement division" << NL;
  return 0;
}

int neg_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement negation" << NL;
  return 0;
}

int lt_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement lt" << NL;
  return 0;
}

int eq_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement eq" << NL;
  return 0;
}

int leq_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement leq" << NL;
  return 0;
}

int comp_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement comp" << NL;
  return 0;
}

int int_const_class::code(ostream& s) {
  emit_load_int(EAX,inttable.lookup_string(token->get_string()), s);
  return 1;
}

int string_const_class::code(ostream& s) {
  emit_load_string(EAX,stringtable.lookup_string(token->get_string()),s);
  return 1;
}

int bool_const_class::code(ostream& s) {
  emit_load_bool(EAX, BoolConst(val), s);
  return 1;
}

int new__class::code(ostream &s) {
  emit_initialize_class(type_name, s);
  return 1;
}

int isvoid_class::code(ostream &s) {
  e1->code(s); // code the subexpression
  emit_move(ESI, EAX, s); // move result of said subexpression to ESI
  emit_load_bool(EAX, truebool, s); // move true to return
  emit_cmpi(ESI, 0, s);
  emit_jump(JZ, label_counter, s);
  emit_load_bool(EAX, falsebool, s); // move false to return
  emit_label_ref(label_counter++, s);
  s << LABEL;
  return 0;
}

int no_expr_class::code(ostream &s) {
  return -1;
}

int object_class::code(ostream &s) {
  // TODO
  s << COMMENT << "implement object" << NL;
  return 0;
}


