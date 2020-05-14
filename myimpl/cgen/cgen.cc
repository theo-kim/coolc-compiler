
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
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

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

static void emit_neg(char *dest, ostream& s)
{ s << NEG << dest << endl; }

static void emit_add(char *dest, char *src, ostream& s)
{ s << ADD << dest << ARG << src << NL; }

static void emit_addi(char *dest, int imm, ostream& s)
{ s << ADD << dest << ARG << imm << endl; }

static void emit_div(char *div, ostream& s)
{ s << DIV << div << endl; }

static void emit_mul(char *mul, ostream& s)
{ s << MUL << mul << endl; }

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

static void emit_xor(char *dst, char* src, ostream& s) 
{ s << XOR << dst << ARG << src << endl; }

static void emit_cdq(ostream& s)
{ s << CDQ << endl; }

static void emit_method_call(Symbol classname, Symbol methodname, ostream& s) {
  s << CALL;
  emit_method_ref(classname, methodname, s);
  s << NL;
}

static void emit_label_def(int l, ostream &s) {
  emit_label_ref(l,s);
  s << ":" << endl;
}

// Push a register on the stack. The stack grows towards smaller addresses.
static void emit_push(char *reg, ostream& str) {
  stack_offset++;
  str << PUSH << reg << NL;
}

// Pop the top of the stack onto a register
static void emit_pop(char *reg, ostream& str) {
  stack_offset--;
  str << POP << reg << NL;
}

static void emit_stackframe(ostream& s, int vars) {
  emit_push(EBP, s); // push base pointer
  emit_move(EBP, ESP, s); // Move the stack pointer to the base pointer
  emit_addi(ESP, -(vars * 4), s); // Allocate space for local variables
  stack_history.push_back(stack_offset);
  stack_offset = vars;
}

static void emit_stackframe_close(ostream& s, int args) {
  emit_move(ESP, EBP, s); // Move the stack pointer to the base of the frame
  emit_pop(EBP, s); // restore the base pointer
  s << RET << args * WORD_SIZE << NL;
  stack_offset = stack_history[stack_history.size() - 1];
  stack_history.pop_back();
  stack_offset -= args;
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
  stringclasstag = 3;
  intclasstag =    1;
  boolclasstag =   2;

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
   basic_status(bstatus),
   table(ct)
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

int CgenNode::resolve_dispatches(CgenNodeP node) {
  if (node == NULL) return 0; // reached the root
  int offset = resolve_dispatches(node->get_parentnd()); // get highest offset from the parent
  Symbol nodeName = node->name; // name of the node
  for (int i = node->features->first(); node->features->more(i); i = node->features->next(i)) {
    Feature feature = node->features->nth(i);
    Symbol methodName = feature->get_method();
    if (methodName == nullptr) continue; // it's an attribute, so ignore
    int methodOffset = 0;
    if (nodeName != name) 
      methodOffset = std::get<2>(table->lookup(nodeName)->dispatch_table[methodName]);
    else if (node->parentnd != NULL && // If it has a parent
      table->lookup(node->parentnd->name) != NULL && // If the parent is in the symbol table
      table->lookup(node->parentnd->name)->dispatch_table.find(methodName) != table->lookup(node->parentnd->name)->dispatch_table.end()) // If the parent has a function by the same name
    { methodOffset = std::get<2>(table->lookup(node->parentnd->name)->dispatch_table[methodName]); }
    else methodOffset = offset++;
    dispatch_table[methodName] = DispatchTableEntry(methodName, nodeName, methodOffset);
    offset = std::max(offset, methodOffset + 1);
  }
  return offset;
}

// Make a dispatch table for each node
void CgenNode::code_dispatch_table(ostream& str) {
  int offset = 0;
  CgenNodeP p = parentnd;
  List<CgenNode> *childCursor = children;
  
  // Insert this function's method to the dispatch table
  resolve_dispatches(this);
  std::vector<DispatchTableEntry *> entries;
  while (entries.size() != dispatch_table.size()) {
    DispatchTableEntry *minimum = nullptr;
    for (DispatchTable::iterator it = dispatch_table.begin(); it != dispatch_table.end(); it++ ) {
      // If the last element in the list is greater than the current its already in the list
      if (entries.size() > 0 && std::get<2>(*entries[entries.size() - 1]) >= std::get<2>(it->second)) continue;
      if (minimum == nullptr || std::get<2>(*minimum) > std::get<2>(it->second)) {
        minimum = &dispatch_table[it->first];
      }
    }
    entries.push_back(minimum);
  }
  // Actually emit the dispatch table
  // Dispatch table header
  str << GLOBAL_NOTAB << name << DISPTAB_SUFFIX << NL;
  str << name << DISPTAB_SUFFIX << LABEL;
  // Write a method for each
  for (size_t i = 0; i < entries.size(); ++i) {
    str << WORD << std::get<1>(*entries[i]) << METHOD_SEP << std::get<0>(*entries[i]) << NL;
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
int CgenNode::code_prototype(ostream& str, int _tag) {
  std::stringstream attributes;
  List<CgenNode> *childCursor = children;
  CgenNodeP p = parentnd;
  int prototypeSize = 3;
  tag = _tag;
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
    _tag = childCursor->hd()->code_prototype(str, _tag + 1);
  } while ((childCursor = childCursor->tl()) != NULL);

  return _tag;
}

// Code the initializer function for the object
void CgenNode::code_initializer(ostream &str) {
  List<CgenNode> *childCursor = children;
  int attributeOffset = 0;
  // Initialize scope
  table->scope.enterscope();
  // First emit the label
  str << name << CLASSINIT_SUFFIX << LABEL;
  // Set up the stackframe
  emit_stackframe(str, 0);
  // Set up the self argument (eax)
  emit_self_arg(str);
  // Call the initializer for the object's parent
  if (get_parentnd() != NULL && get_parentnd()->name != No_class) emit_init_call(parentnd->name, str);
  // Resolve the expressions to initialize the attributes
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    Feature feature = features->nth(i);
    if (feature->get_attribute() == nullptr) continue;
    accumulator_level = 2;
    table->self_reference = name;
    if (feature->code(str, this) >= 0) { // Code the attribute
      // Move the result (stored in eax) into the destination register (if no no_exp)
      emit_store(ECX, attributes.find(feature->get_name())->second, EAX, str);
    }
    table->scope.addid(feature->get_name(), new IdentifierOffset(ECX, attributes.find(feature->get_name())->second));
  }
  // Set up the return value (eax)
  emit_move(EAX, ECX, str);
  emit_pop(ECX, str);
  // Close the stackframe and return from the function
  emit_stackframe_close(str, 0);

  // Now generate for the children
  if (childCursor != NULL)  {
    // Iterate through the children depth first
    do {
      childCursor->hd()->code_initializer(str);
    } while ((childCursor = childCursor->tl()) != NULL);
  } // Leaf node
  // Exit the scope
  current_scope = table->scope; // save the scope
  table->scope.exitscope();
  return;
}

void CgenNode::code_methods(ostream& str) {
  List<CgenNode> *childCursor = children;
  if (basic_status == Basicness::NotBasic) {
    for (int i = features->first(); features->more(i); i = features->next(i)) {
      Feature feature = features->nth(i);
      if (feature->get_method() == nullptr) continue;
      // First emit the label
      str << name << METHOD_SEP << feature->get_name() << LABEL;
      table->self_reference = name;
      feature->code(str, this); // emit the method's code
    }
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
int assign_class::code(ostream &s, CgenNodeP ct) {
  int offset;
  char *reg;
  IdentifierOffset *id = ct->current_scope.lookup(name); // lookup the location of the variable being assigned to
  if (id == nullptr) { return 1; } // shouldn't happen...
  offset = id->second;
  reg = id->first;
  this->expr->code(s, ct); // resolve the expression
  // move the value from the expression to the variable at given offset
  if (strncmp(reg, EBP, 3) == 0) {
    emit_store(reg, offset + 1, EAX, s);
  }
  else {
    emit_store(reg, offset, EAX, s);
  }
  return 1;
}

int static_dispatch_class::code(ostream &s, CgenNodeP ct) {
  // load arguments into the stack
  for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
    Expression e = actual->nth(i);
    e->code(s, ct);
    emit_push(EAX, s);
  }
  // resolve the calling expression
  expr->code(s, ct);
  // check for void
  emit_cmpi(EAX, 0, s);
  emit_jump(JNE, label_counter, s);
  // if void, throw runtime error
  emit_move(EAX, "str_const0", s);
  emit_partial_load_address(ESI, s);
  s << line_number << NL;
  emit_call("_dispatch_abort", s);
  // if not void, get the dispatch table for the object
  emit_label_def(label_counter++, s);
  // find the desired function in the dispatch table
  Symbol type = type_name;
  if (type == SELF_TYPE) type = ct->table->self_reference;
  CgenNode *node = ct->table->lookup(type);
  DispatchTableEntry *entry = node->get_dispatch_offset(name);
  if (entry == nullptr) cout << "Something went wrong" << NL;
  int offset = std::get<2>(*entry);
  emit_partial_load_address(ESI, s);
  s << type << DISPTAB_SUFFIX << NL;
  emit_load(ESI, ESI, offset, s);
  emit_call(ESI, s); // call the dispatched function
  // DONE
  return 1;
}

int dispatch_class::code(ostream &s, CgenNodeP ct) {
  // load arguments into the stack
  for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
    Expression e = actual->nth(i);
    e->code(s, ct);
    emit_push(EAX, s);
  }
  // resolve the calling expression
  expr->code(s, ct);
  // check for void
  emit_cmpi(EAX, 0, s);
  emit_jump(JNE, label_counter, s);
  // if void, throw runtime error
  emit_move(EAX, "str_const0", s);
  emit_partial_load_address(ESI, s);
  s << line_number << NL;
  emit_call("_dispatch_abort", s);
  // if not void, get the dispatch table for the object
  emit_label_def(label_counter++, s);
  emit_load(ESI, EAX, DISPTABLE_OFFSET, s);
  // find the desired function in the dispatch table
  Symbol type = expr->get_type();
  if (type == SELF_TYPE) type = ct->table->self_reference;
  CgenNode *node = ct->table->lookup(type);
  DispatchTableEntry *entry = node->get_dispatch_offset(name);
  if (entry == nullptr) cout << "Something went wrong" << NL;
  int offset = std::get<2>(*entry);
  emit_load(ESI, ESI, offset, s);
  emit_call(ESI, s); // call the dispatched function
  // DONE
  return 1;
}

int cond_class::code(ostream &s, CgenNodeP ct) {
  int false_label = label_counter++;
  int end_label = label_counter++;
  pred->code(s, ct); // Code the predicate
  emit_load(ESI, EAX, BOOL_VALUE_OFFSET, s); // Move predicate value into ESI
  emit_cmpi(ESI, 0, s); // Check if the predicate is false
  emit_jump(JZ, false_label, s); // If false jump to the false label
  then_exp->code(s, ct); // Execute the true body
  emit_jump(JMP, end_label, s); // Unconditionally jump to the end of the conditional
  emit_label_def(false_label, s); // Emit the false label
  else_exp->code(s, ct); // Execute the false body
  emit_label_def(end_label, s); // Emit the end label
  return 1;
}

int loop_class::code(ostream &s, CgenNodeP ct) {
  // Loop
  int loop_begin = label_counter++;
  int loop_end = label_counter++;
  emit_label_def(loop_begin, s); // Emit the loop start label
  pred->code(s, ct); // Code the predicate
  emit_load(ESI, EAX, BOOL_VALUE_OFFSET, s); // Fetch the value of the predicate boolean
  emit_cmpi(ESI, 0, s); // Check if the predicate is false
  emit_jump(JE, loop_end, s); // If so, jump to end of the loop
  body->code(s, ct); // Else execute the loop
  emit_jump(JMP, loop_begin, s); // Unconditionally jump back to the beginning of the loop
  emit_label_def(loop_end, s); // Emit end of the loop label
  emit_move(EAX, "0", s); // return void by moving zero into EAX
  return 0;
}

int typcase_class::code(ostream &s, CgenNodeP ct) {
  std::vector<std::pair<CgenNodeP, Case> > branches;
  int case_label = label_counter++;
  int end_of_case = label_counter++;
  expr->code(s, ct); // Evaluate the expression
  emit_cmpi(EAX, VOID_VAL, s); // check if the value of the expression is void, if so runtime error
  emit_jump(JNE, case_label, s); // jump past runtime error if expr is not void
  emit_move(EAX, "str_const0", s); // move filename into argument
  emit_partial_load_address(ESI, s); // move current filename into the other argument
  s << line_number << NL;
  emit_call("_case_abort2", s); // call the runtime error
  emit_label_def(case_label, s); // emit label for rest of case
  emit_load(EDI, EAX, TAG_OFFSET, s); // Load the object type tag
  while (branches.size() != (size_t)cases->len()) { // order the branches by tag
    std::pair<CgenNodeP, Case> maximum = std::pair<CgenNodeP, Case>(nullptr, nullptr);
    for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
      Case branch = cases->nth(i);
      CgenNodeP curr = ct->table->lookup(branch->get_type());
      if (branches.size() > 0 && branches[branches.size() - 1].first->get_tag() <= curr->get_tag()) continue;
      if (maximum.first == nullptr || maximum.first->get_tag() < curr->get_tag()) {
        maximum = std::pair<CgenNodeP, Case>(curr, branch);
      }
    }
    branches.push_back(maximum);
  }
  for (size_t i = 0; i < branches.size(); ++i) {
    Case branch = branches[i].second;
    branch->code(s, ct); // Code each branch
    emit_jump(JMP, end_of_case, s); // emit unconditional jump to end
  }
  // If none is found, emit _case_abort runtime error
  emit_label_def(label_counter++, s); // Emit case runtime error label
  emit_call("_case_abort", s); // call the runtime error
  emit_label_def(end_of_case, s); // emit end of the case label

  return 0;
}

int search_tag(CgenNodeP node) {
  List<CgenNode> *childCursor = node->get_children();
  if (childCursor == NULL) { // If no children, no need to check
    return node->get_tag();
  }
  while (childCursor->tl() != NULL) { // look for the last child
    childCursor = childCursor->tl();
  }
  return search_tag(childCursor->hd());
}

void branch_class::code(ostream& s, CgenNodeP ct) {
  // get class tag number for the type of the branch
  CgenNodeP branch_node = ct->table->lookup(type_decl);
  int class_tag = branch_node->get_tag();
  emit_label_def(label_counter++, s);// Emit label
  emit_cmpi(EDI, class_tag, s); // Emit bottom check (this tag)
  emit_jump(JL, label_counter, s); // Jump to next branch if false
  emit_cmpi(EDI, search_tag(branch_node), s); // Emit top check (furthest child)
  emit_jump(JG, label_counter, s); // Jump to next branch if false
  ct->current_scope.enterscope(); // Enter scope branch variable
  emit_push(EAX, s); // Allocate variable
  expr->code(s, ct); // Execute code body
  emit_addi(ESP, 4, s); // Clean up local variables
  stack_offset--;
  ct->current_scope.exitscope(); // Leave
}

int block_class::code(ostream &s, CgenNodeP ct) {
  // iterate through each expression and emit code
  for (int i = body->first(); body->more(i); i = body->next(i)) {
    Expression e = body->nth(i);
    e->code(s, ct);
  }
  return 1;
}

int let_class::code(ostream &s, CgenNodeP ct) {
  ct->current_scope.enterscope(); // enter a new scope for the local variables
  init->code(s, ct); // emit the code for the local variable
  emit_push(EAX, s); // push local variable onto the stack
  ct->current_scope.addid(identifier, new IdentifierOffset(EBP, -stack_offset)); // add to scope
  body->code(s, ct);
  emit_addi(ESP, 4, s); // clean up local variables from the stack
  stack_offset--;
  ct->current_scope.exitscope(); // leave scope
  return 0;
}

// binary arithmetic operations: stack machine rules
//   - first emit code for each subexpression (result is always stored into eax)
//   - then initialize new int object
//   - perform operation
//   - store value into new int object
void binary_operation(char *op, Expression e1, Expression e2, ostream &s, CgenNodeP ct) {
  e1->code(s, ct); // emit subexpression 1
  emit_push(EAX, s); // put result onto the stack
  e2->code(s, ct); // emit subexpression 2
  emit_method_call(Object, copy_sym, s); // Copy that object
  emit_pop(ESI, s); // get the other operand from the stack
  emit_load(EDI, EAX, INT_VALUE_OFFSET, s); // get the value from e2
  emit_load(ESI, ESI, INT_VALUE_OFFSET, s); // get the value from e1
  s << op << ESI << ARG << EDI << NL; // perform operation
  emit_store(EAX, INT_VALUE_OFFSET, ESI, s); // store result back into the int object
}

int plus_class::code(ostream &s, CgenNodeP ct) {
  binary_operation(ADD, e1, e2, s, ct);
  return 1;
}

int sub_class::code(ostream &s, CgenNodeP ct) {
  binary_operation(SUB, e1, e2, s, ct);
  return 1;
}

int mul_class::code(ostream &s, CgenNodeP ct) {
  e1->code(s, ct); // emit e1
  emit_push(EAX, s); // push e1 onto the stack
  e2->code(s, ct); // emit e2
  emit_method_call(Object, copy_sym, s); // Copy the object for the result
  emit_pop(ESI, s); // pop the second argument back into ESI
  emit_load(EDI, EAX, INT_VALUE_OFFSET, s); // load the e2 integer value into EDI
  emit_load(ESI, ESI, INT_VALUE_OFFSET, s); // load the e1 integer value into ESI
  emit_push(EAX, s); // save the value of EAX
  emit_push(EDX, s); // save the value of EDX
  emit_move(EAX, ESI, s); // move the multiplicand into EAX for MUL instruction
  emit_mul(EDI, s); // do the multiplication
  emit_move(ESI, EAX, s); // move lower 32-bits into the ESI
  emit_pop(EDX, s); // restore EDX
  emit_pop(EAX, s); // restore EAX
  emit_store(EAX, INT_VALUE_OFFSET, ESI, s); // store the value into the integer object
  return 0;
}

int divide_class::code(ostream &s, CgenNodeP ct) {
  // NOTE: Potential runtime error (division by 0)
  // Cool only has integer division
  e1->code(s, ct); // emit e1
  emit_push(EAX, s); // push e1 onto the stack
  e2->code(s, ct); // emit e2
  emit_method_call(Object, copy_sym, s); // Copy the object for the result
  emit_pop(ESI, s); // pop e1 into ESI
  emit_load(EDI, EAX, INT_VALUE_OFFSET, s); // load the e2 integer value into EDI
  emit_load(ESI, ESI, INT_VALUE_OFFSET, s); // load the e1 integer value into ESI
  emit_push(EAX, s); // save the value of EAX
  emit_push(EDX, s); // save the value of EDX
  emit_xor(EDX, EDX, s); // clear the value of EDX register
  emit_move(EAX, ESI, s); // move the dividend value into EAX
  emit_cdq(s); // extend the sign of EAX into EDX
  emit_div(EDI, s); // do the division (the runtime will catch a division by zero)
  emit_move(ESI, EAX, s); // move the quotient (ignoring the remainder) into ESI
  emit_pop(EDX, s); // restore the value of EDX
  emit_pop(EAX, s); // restore the value of EAX
  emit_store(EAX, INT_VALUE_OFFSET, ESI, s); // move the quotient into the integer object
  return 0;
}

int neg_class::code(ostream &s, CgenNodeP ct) {
  e1->code(s, ct); // Code the expression
  emit_method_call(Object, copy_sym, s); // Copy integer from e1
  emit_load(ESI, EAX, INT_VALUE_OFFSET, s); // Load the value from e1
  emit_neg(ESI, s); // Negate the value
  emit_store(EAX, INT_VALUE_OFFSET, ESI, s); // Put the value back into the integer
  return 0;
}

int lt_class::code(ostream &s, CgenNodeP ct) {
  // Produces a boolean object
  e1->code(s, ct); // emit code for e1
  emit_push(EAX, s); // push e1 onto the stack
  e2->code(s, ct); // emit code for e2
  emit_pop(ESI, s); // pop e1 into ESI
  emit_load(ESI, ESI, INT_VALUE_OFFSET, s); // load esi with value of esi
  emit_load(EDI, EAX, INT_VALUE_OFFSET, s); // load e2 value into EDI
  emit_load_bool(EAX, truebool, s); // load true by default into EAX
  emit_cmp(ESI, EDI, s); // e1 < e2
  emit_jump(JL, label_counter, s); // jump if less than
  emit_load_bool(EAX, falsebool, s); // if not less than it is false
  emit_label_def(label_counter++, s); // emit jump label
  return 0;
}

int eq_class::code(ostream &s, CgenNodeP ct) {
  // Equality produces a boolean object
  //    - Already type safe
  //    - Compare value of integers or booleans
  //    - Compare pointers
  //    - uses runtime equality_test
  e1->code(s, ct); // emit code for e1
  emit_push(EAX, s); // push e1 onto the stack
  e2->code(s, ct); // emit code for e2
  emit_pop(ESI, s); // pop e1 into ESI
  emit_move(EDI, EAX, s); // move e2 into EDI
  emit_load_bool(EAX, truebool, s); // load true by default into EAX
  emit_cmp(ESI, EDI, s); // e1 = e2 (for pointer equality)
  emit_jump(JE, label_counter, s); // jump if equal
  emit_load_bool(EBX, falsebool, s); // load false for call to equality_test
  emit_call("equality_test", s); // Check the equality with the runtime function (puts true or false into eax)
  emit_label_def(label_counter++, s); // emit jump label
  // No need to check value equality
  return 0;
}

int leq_class::code(ostream &s, CgenNodeP ct) {
  // Produces a boolean object
  e1->code(s, ct); // emit code for e1
  emit_push(EAX, s); // push e1 onto the stack
  e2->code(s, ct); // emit code for e2
  emit_pop(ESI, s); // pop e1 into ESI
  emit_load(ESI, ESI, INT_VALUE_OFFSET, s); // load esi with value of esi
  emit_load(EDI, EAX, INT_VALUE_OFFSET, s); // load e2 value into EDI
  emit_load_bool(EAX, truebool, s); // load true by default into EAX
  emit_cmp(ESI, EDI, s); // e1 < e2
  emit_jump(JLE, label_counter, s); // jump if less than
  emit_load_bool(EAX, falsebool, s); // if not less than it is false
  emit_label_def(label_counter++, s); // emit jump label
  return 0;
}

int comp_class::code(ostream &s, CgenNodeP ct) {
  // produces a boolean object (not)
  e1->code(s, ct); // emit code for the expression
  emit_load(ESI, EAX, BOOL_VALUE_OFFSET, s); // move boolean object value into ESI
  emit_load_bool(EAX, truebool, s); // move true into EAX
  emit_cmpi(ESI, 0, s); // check if e1 is false
  emit_jump(JZ, label_counter, s); // if comparison is 0 (i.e. e1 == false) jump
  emit_load_bool(EAX, falsebool, s); // if comparison is 1 (i.e. e1 == true) then assign result to false
  emit_label_def(label_counter++, s); // emit label
  return 0;
}

int int_const_class::code(ostream& s, CgenNodeP ct) {
  emit_load_int(EAX,inttable.lookup_string(token->get_string()), s);
  return 1;
}

int string_const_class::code(ostream& s, CgenNodeP ct) {
  emit_load_string(EAX,stringtable.lookup_string(token->get_string()),s);
  return 1;
}

int bool_const_class::code(ostream& s, CgenNodeP ct) {
  emit_load_bool(EAX, BoolConst(val), s);
  return 1;
}

int new__class::code(ostream &s, CgenNodeP ct) {
  emit_initialize_class(type_name, s);
  return 1;
}

int isvoid_class::code(ostream &s, CgenNodeP ct) {
  e1->code(s, ct); // code the subexpression
  emit_move(ESI, EAX, s); // move result of said subexpression to ESI
  emit_load_bool(EAX, truebool, s); // move true to return
  emit_cmpi(ESI, 0, s);
  emit_jump(JZ, label_counter, s);
  emit_load_bool(EAX, falsebool, s); // move false to return
  emit_label_def(label_counter++, s);
  return 0;
}

int no_expr_class::code(ostream &s, CgenNodeP ct) {
  return -1;
}

int object_class::code(ostream &s, CgenNodeP ct) {
  // cout << name << endl;
  if (name == self) emit_move(EAX, ECX, s); // if the current object is self, then return self ref
  else {
    IdentifierOffset *id = ct->current_scope.lookup(name);
    if (id == nullptr) { return 1; }
    int offset = id->second;
    char *reg = id->first;
    if (strncmp(reg, EBP, 3) == 0) {
      emit_load(EAX, reg, offset + 1, s);
    }
    else {
      emit_load(EAX, reg, offset, s);
    }
  }
  return 1;
}

int method_class::code(ostream &s, CgenNodeP ct) {
  // intialize new scope
  ct->current_scope.enterscope();
  // set up the stack frame
  emit_stackframe(s, 0);
  // Set up the self argument (eax)
  emit_self_arg(s);
  // set up local variables (inject formals)
  for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
    Formal formal = formals->nth(i);
    formal->code(s, ct, formals->len() - i - 1);
  }
  expr->code(s, ct); // Code the expression
  // return value should already by in EAX
  emit_pop(ECX, s); // restore ecx
  // Close the stackframe and return from the function
  emit_stackframe_close(s, formals->len());
  // exit the scope
  ct->current_scope.exitscope();
  return 1;
}

void formal_class::code(ostream &s, CgenNodeP ct, int offset) {
  // add the formal to the scope
  ct->current_scope.addid(name, new IdentifierOffset(EBP, offset + 1));
}