#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include <vector>
#include <set>
#include <string>
#include <map>
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"

#define TRUE 1
#define FALSE 0

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
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

class ClassTable;
typedef ClassTable *ClassTableP;

class Class__class;
typedef Class__class *Class_;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;

ClassTable *class_table;

class ClassTable {
private:
  int semant_errors;
  Classes insert_basic_classes(Classes classes);
  ostream& error_stream;

  class InheritanceGraph { // Implementation of an inheritance directed graph
  public:
    class Node {
    public:
      Node(Symbol symbol);
      Symbol s;
      Node *parent;
      // This symbol table tracks methods associated with the class
      // The lookup name is the method's name, while the value is a vector of symbols
      //    where the first value of the vector is the return type and subsequent values
      //    are formals.
      //    There is only ONE scope, inheritance lookups are performed by graph traversal
      std::map<Symbol, std::vector<Symbol> > method_table;
      std::map<Symbol, Symbol> attr_table;
      std::vector<Symbol> *find_inheritted_method(Symbol);
      std::vector<Symbol> *find_method(Symbol);
      Symbol find_inheritted_attr(Symbol);
      Symbol find_attr(Symbol);
      bool operator<=(Node *);
      bool resolved;
    };
    InheritanceGraph(ClassTable *table, Classes classes);
    ~InheritanceGraph();
    Node* operator[](const Symbol s) const; // Get node for symbol
    // Node* operator[](const Entry* s) const { return operator[]((Symbol)s); }; // Get node for symbol
    Node* join(Symbol a, Symbol b) const; // Get the JOIN operation for two types
    int errors;
  private:
    std::vector<Node *> nodes;
  };

public:
  ClassTable(Classes);
  InheritanceGraph inheritance_graph;
  SymbolTable<Symbol, Symbol> symbol_table;
  Symbol current_filename;
  void install_basic_class_methods();
  int errors() { return semant_errors; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(tree_node *t);
  ostream& semant_error(Symbol filename, tree_node *t);
  bool leq(Symbol a, Symbol b) { 
    if (a == SELF_TYPE) {
      a = *(symbol_table.lookup(SELF_TYPE));
    }
    if (b == SELF_TYPE) {
      b = *(symbol_table.lookup(SELF_TYPE));
    }
    return *(inheritance_graph[a]) <= inheritance_graph[b];
  }
  InheritanceGraph::Node* operator[](Symbol s) const { return inheritance_graph[s]; }
};

// bool operator<=(const Entry& a, const Entry& b) { return class_table->inheritance_graph[&a] <= class_table->inheritance_graph[&b]; }

#endif

