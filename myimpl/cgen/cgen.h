#include <assert.h>
#include <stdio.h>
#include <map>
#include <tuple>
#include <vector>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy_sym,
       Int,
       in_int,
       in_string,
       IO,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;

int accumulator_level, label_counter = 0, stack_offset = 0;
std::vector<int> stack_history;

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

typedef std::tuple<Symbol, Symbol, int> DispatchTableEntry;
typedef std::map<Symbol, DispatchTableEntry > DispatchTable;
typedef std::map<Symbol, int> AttributeOffsets;
typedef std::pair<Symbol, int> AttributeOffsetEntry;
typedef std::pair<char *, int> IdentifierOffset;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;

// The following methods emit code for
// constants and global declarations.

   void code_extern();
   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();

   void code_name_tab();
   void code_obj_tab();

   void code_dispatch_tables();
   void code_object_prototypes();
   void code_object_initializers();
   void code_object_methods();

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
public:
   CgenClassTable(Classes, ostream& str);
   void code();
   CgenNodeP root();

   Symbol self_reference;
   SymbolTable<Symbol, IdentifierOffset> scope;
};


class CgenNode : public class__class {

private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise
   DispatchTable dispatch_table;
   AttributeOffsets attributes;
   
   int tag;
public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
   
   void resolve_attributes(CgenNodeP node, std::stringstream& str, int *len);
   int resolve_dispatches(CgenNodeP node);

   void code_obj_tab_entry(ostream& str);
   void code_name_tab_entry(ostream& str);
   void code_dispatch_table(ostream& str);
   int code_prototype(ostream &str, int tag);
   void code_initializer(ostream &str);
   void code_methods(ostream &str);

   int get_tag() { return tag; }
   DispatchTableEntry *get_dispatch_offset(Symbol methodName) { 
      if (dispatch_table.find(methodName) == dispatch_table.end()) return nullptr;
      return &dispatch_table[methodName];
   }

   CgenClassTableP table;
   SymbolTable<Symbol, IdentifierOffset> current_scope;
};

class BoolConst {
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};

