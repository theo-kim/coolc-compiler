#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "cool-tree.h"
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;
int curr_lineno;

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
    copy        = idtable.add_string("copy");
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

Symbol class__class::get_filename() { return filename; }
Symbol class__class::get_name() { return name; }
Symbol class__class::get_parent() { return parent; }
Features class__class::get_features() { return features; }
void class__class::register_symbols(ClassTable *class_table) {
    class_table->current_filename = filename;
    for (int i = 0; i < features->len(); ++i) {
        Feature current_feature = features->nth(i);
        current_feature->register_symbol(class_table, this);
    }
}
void class__class::semant(ClassTable *class_table) {
    class_table->current_filename = filename;
    class_table->symbol_table.enterscope();
    // Add scope for SELF_TYPE to the current type
    class_table->symbol_table.addid(SELF_TYPE, &name);
    // Add all the symbols from the inheritted class
    Symbol cursor = name;
    while (cursor != No_class) {
        for (auto& attr : class_table->inheritance_graph[cursor]->attr_table) {
            class_table->symbol_table.addid(attr.first, &(attr.second));
        }
        if (class_table->inheritance_graph[cursor]->parent == nullptr) break;
        cursor = class_table->inheritance_graph[cursor]->parent->s;
    }
    // Perform semantic analysis on each feature
    for (int i = 0; i < features->len(); ++i) {
        features->nth(i)->semant(class_table, this);
    }
    class_table->symbol_table.exitscope();
}
// Formals overloads
Symbol formal_class::get_name() { return name; }
Symbol formal_class::get_type() { return type_decl; }
void formal_class::semant(ClassTable *class_table) { 
    // Ensure that the type of the formal is a declared type
    if (name == self) {
        class_table->semant_error(this) 
            << "'self' cannot be the name of a formal parameter." << std::endl;
    }
    if (type_decl == SELF_TYPE) {
        class_table->semant_error(this)
            << "Formal parameter " << name << " cannot have type SELF_TYPE." << std::endl;
        class_table->symbol_table.addid(name, &Object);
        return;
    }
    if (class_table->inheritance_graph[type_decl] == nullptr) {
        class_table->semant_error(this) 
            << "Class " << type_decl
            << " of formal parameter " << name
            << " is undefined." << std::endl;
        class_table->symbol_table.addid(name, &Object);
        return;
    }
    class_table->symbol_table.addid(name, &type_decl);
}
// Method feature overload
Symbol method_class::get_name() { return name; }
Symbol method_class::get_type() { return return_type; }
void method_class::register_symbol(ClassTable* class_table, Class_ current_class) {
    std::map<Symbol, std::vector<Symbol> > *method_table = &(class_table->inheritance_graph[current_class->get_name()]->method_table);
    // Check if the method already exists
    if (method_table->count(name) > 0) {
        class_table->semant_error(this)
            << "Method " << name
            << " is multiply defined." << std::endl;
        return;
    }
    // If a class inherits a method from an ancestor class, then it may override the inherited
    //      definition provided the number of arguments, the types of the formal parameters, and the return
    //      type are exactly the same in both definitions.
    std::vector<Symbol> *symbol_definition = class_table->inheritance_graph[current_class->get_name()]->find_inheritted_method(name);
    if (symbol_definition != nullptr) {
        // Symbol was previously defined, so check types
        // Check return type
        if ((*symbol_definition)[0] != return_type) {
            class_table->semant_error(this)
                << "In redefined method " << name
                << ", return type " << return_type
                << " is different from original return type " << (*symbol_definition)[0]
                << "." << std::endl;
                return;
        }
        // Check number of formals
        if (symbol_definition->size() - 1 != (size_t)formals->len()) {
            class_table->semant_error(this)
                << "Incompatible number of formal parameters in redefined method " << name
                << "." << std::endl;
            return;
        }
        // Check formal types
        for (size_t i = 1; i < symbol_definition->size(); ++i) {
            if ((*symbol_definition)[i] != formals->nth(i - 1)->get_type()) {
                class_table->semant_error(this)
                    << "In redefined method " << name
                    << ", parameter type " << formals->nth(i)->get_type()
                    << " is different from original type " << (*symbol_definition)[i]
                    << std::endl;
                return;
            }
        }
    }
    // Add method to the method table of the class
    std::pair<std::map<Symbol, std::vector<Symbol> >::iterator, bool> indx;
    indx = method_table->insert(
        std::pair<Symbol, std::vector<Symbol> >(name, std::vector<Symbol>())
    );
    // Add the return type and formals to the method table
    method_table->at(name).push_back(return_type);
    for (int i = 0; i < formals->len(); ++i) {
        method_table->at(name).push_back(formals->nth(i)->get_type());
    }
}
void method_class::semant(ClassTable* class_table, Class_ current_class) {
    Symbol saved_return_type = return_type;
    if (return_type == SELF_TYPE) {
        return_type = *class_table->symbol_table.lookup(SELF_TYPE);
    }
    // Ensure that the type of the feature is a declared type
    if (class_table->inheritance_graph[return_type] == nullptr) {
        class_table->semant_error(this) 
            << "Undefined return type " << return_type
            << " in method " << name
            << "." << std::endl;
    }
    // Put the formals into scope
    class_table->symbol_table.enterscope();
    std::set<Symbol> track;
    for (int i = 0; i < formals->len(); ++i) {
        if (track.count(formals->nth(i)->get_name()) != 0) {
            class_table->semant_error(this) 
                << "Formal parameter " << formals->nth(i)->get_name()
                << " is multiply defined." << std::endl;
            continue;
        }
        track.insert(formals->nth(i)->get_name());
        formals->nth(i)->semant(class_table);
    }
    // Semant the function expression
    expr->semant(class_table);
    // Check if the expression type matches the return type
    Symbol expr_type = expr->get_type();
    if (class_table->inheritance_graph[expr_type] != nullptr && !class_table->leq(return_type, expr_type)) {
        class_table->semant_error(this) 
            << "Inferred return type " << expr_type
            << " of method " << name
            << " does not conform to declared return type " << return_type
            << "." << std::endl;
    }
    class_table->symbol_table.exitscope();
    return_type = saved_return_type;
}
// Attribute feature overloads
Symbol attr_class::get_name() { return name; }
Symbol attr_class::get_type() { return type_decl; }
void attr_class::register_symbol(ClassTable* class_table, Class_ current_class) {
    // It is also illegal to redefine inheritted attribute names
    Symbol symbol_definition = class_table->inheritance_graph[current_class->get_name()]->find_inheritted_attr(name);
    if (symbol_definition != nullptr) {
        // Symbol was previously defined
        class_table->semant_error(this)
            << "Attribute " << name 
            << " is an attribute of an inherited class." << std::endl;
        return;
    }
    // Add symbol to attribute table
    std::map<Symbol, Symbol> *feature_table = &(class_table->inheritance_graph[current_class->get_name()]->attr_table);
    std::pair<std::map<Symbol, Symbol>::iterator, bool> indx;
    if (type_decl == SELF_TYPE) {
        indx = feature_table->insert(
            std::pair<Symbol, Symbol>(name, current_class->get_name())
        );
    }
    else {
        indx = feature_table->insert(
            std::pair<Symbol, Symbol>(name, type_decl)
        );   
    }
    // Check if the attribute already exists
    if (indx.second == false) {
        class_table->semant_error(this)
            << "Attribute " << name
            << " is multiply defined in class." << std::endl;
    }
}
void attr_class::semant(ClassTable* class_table, Class_ current_class) {
    Symbol type_decl_save = type_decl;
    if (type_decl == SELF_TYPE) {
        type_decl = *(class_table->symbol_table.lookup(SELF_TYPE));
    }
    // No need to semant attributes of basic classes
    if (class_table->current_filename->equal_string("<basic class>", 13)) return;
    // Ensure that self is not being declared as an attribute
    if (name == self) {
        class_table->semant_error(this)
            << "'self' cannot be the name of an attribute." << std::endl;
    }
    // Ensure that the type of the feature is a declared type
    if (class_table->inheritance_graph[type_decl] == nullptr) {
        class_table->semant_error(this) 
            << "Class " << type_decl
            << " of attribute " << name
            << " is undefined." << std::endl;
    }
    init->semant(class_table); // Semant the initialization expression
    // Ensure that the expression in the assignment is the same type as the declaration
    if (init->get_type() != No_type && !class_table->leq(type_decl, init->get_type())) {
        class_table->semant_error(this) 
            << "Inferred type " << init->get_type()
            << " of initialization of attribute " << name
            << " does not conform to declared type " << type_decl
            << "." << std::endl;
    }
    type_decl = type_decl_save;
}
// Case branch overloads
Symbol branch_class::get_type() { return expr->get_type(); }
Symbol branch_class::get_name() { return name; }
void branch_class::semant(ClassTable* class_table) {
    int errors = 0;
    // make sure type declaration names declared type
    if (type_decl != SELF_TYPE && class_table->inheritance_graph[type_decl] == nullptr) {
        class_table->semant_error(this)
            << "Class " << type_decl
            << " of case branch is undefined." << std::endl;
        ++errors;
    }
    // make sure that self isn't being bound to
    if (name == self) {
        class_table->semant_error(this)
            << "'self' bound in 'case'." << std::endl;
    }
    // make sure SELF_TYPE is not case branch type
    if (type_decl == SELF_TYPE && errors == 0) {
        class_table->semant_error(this)
            << "Identifier " << name << " declared with type SELF_TYPE in case branch." << std::endl;
    }
    // Open a new scope for the branch and insert the new variable
    class_table->symbol_table.enterscope();
    class_table->symbol_table.addid(name, &type_decl);
    // semant the expression
    expr->semant(class_table);
    // Close the scope
    class_table->symbol_table.exitscope();
}

// EXPRESSION SEMANT OVERLOADS
// An assignment has the form <id> <- <expr>
//      The static type of the expression must conform to the declared type of 
//      the identifier. The value is the value of the expression. The static type 
//      of an assignment is the static type of <expr>
//      It is illegal to assign to self
void assign_class::semant(ClassTable *class_table) {
    // Semant the expression
    expr->semant(class_table);
    // make sure that self isn't being assigned to
    if (name == self) {
        class_table->semant_error(this)
            << "Cannot assign to 'self'." << std::endl;
        set_type(expr->get_type());
        return;
    }
    if (class_table->symbol_table.lookup(name) == NULL) {
        class_table->semant_error(this)
            << "Assignment to undeclared variable " << name
            << "." << std::endl;
        set_type(expr->get_type());
        return;
    }
    // Check the type of the id being assigned to
    Symbol id_type = *class_table->symbol_table.lookup(name);
    if (id_type == SELF_TYPE) {
        if (!class_table->leq(*(class_table->symbol_table.lookup(SELF_TYPE)), expr->get_type())) {
            class_table->semant_error(this) 
                << "Type " << expr->get_type()
                << " of assigned expression does not conform to declared type " << id_type 
                << " of identifier " << name << "." << std::endl;
        }
    }
    else if (expr->get_type() == SELF_TYPE) {
        if (!class_table->leq(id_type, *(class_table->symbol_table.lookup(SELF_TYPE)))) {
            class_table->semant_error(this) 
                << "Type " << expr->get_type()
                << " of assigned expression does not conform to declared type " << id_type 
                << " of identifier " << name << "." << std::endl;
        }
    }
    else {
        if (!class_table->leq(id_type, expr->get_type())) {
            class_table->semant_error(this) 
                << "Type " << expr->get_type()
                << " of assigned expression does not conform to declared type " << id_type 
                << " of identifier " << name << "." << std::endl;
        }
    }
    set_type(expr->get_type());
}
// Expression expr;
// Symbol type_name;
// Symbol name;
// Expressions actual;
void static_dispatch_class::semant(ClassTable *class_table) {
    Symbol return_type = No_type;
    // First resolve type of expr to get the type to find the function for
    expr->semant(class_table);
    // Get the type for expr, check if its SELF_TYPE, otherwise, find method table
    Symbol expr_type = expr->get_type();
    if (expr_type == SELF_TYPE) {
        expr_type = *(class_table->symbol_table.lookup(SELF_TYPE));
    }
    // Check that static dispatch refers to declared type
    if ((*class_table)[type_name] == nullptr) {
        class_table->semant_error(this)
            << "Static dispatch to undefined class " << type_name
            << "." << endl;
        set_type(Object);
        return;
    }
    // Check if the type named in the static dispatch matches that of the expression
    if (!(class_table->leq(type_name, expr_type))) {
        class_table->semant_error(this)
            << "Expression type " << expr_type
            << " does not conform to declared static dispatch type " << type_name
            << "." << std::endl;
    }
    // Check if dispatched function belongs to the dispatch class
    std::vector<Symbol> *method_symbols = class_table->inheritance_graph[type_name]->find_method(name);
    if (method_symbols == nullptr) {
        class_table->semant_error(this)
            << "Static dispatch to undefined method " << name
            << "." << std::endl;
            set_type(Object);
            return;
    }
    // Check that the number of formals provided in the dispatch matches the number from the declaration
    if (method_symbols->size() - 1 != (size_t)actual->len()) {
        class_table->semant_error(this) 
            << "Method " << name
            << " called with wrong number of arguments." << std::endl;
    }
    // Check that the types of the formals provided in the dispatch matches
    for (size_t i = 1; i < method_symbols->size(); ++i) {
        // Semant the expression
        actual->nth(i - 1)->semant(class_table);
        if (!class_table->leq((*method_symbols)[i], actual->nth(i - 1)->get_type())) {
            class_table->semant_error(this) 
                << "In call of method " << name
                << ", type " << actual->nth(i - 1)->get_type() 
                << " of parameter arg does not conform to declared type " << (*method_symbols)[i]
                << "." << std::endl;
        }
    }
    // set the type to the return type
    return_type = (*method_symbols)[0];
    if (return_type == SELF_TYPE) {
        return_type = type_name;
    }
    set_type(return_type);
}
void dispatch_class::semant(ClassTable *class_table) {
    Symbol return_type = No_type;
    // First resolve type of expr to get the type to find the function for
    expr->semant(class_table);
    // Get the type for expr, check if its SELF_TYPE, otherwise, find method table
    Symbol expr_type = expr->get_type();
    if (expr_type == SELF_TYPE) {
        expr_type = *(class_table->symbol_table.lookup(SELF_TYPE));
    }
    if (class_table->inheritance_graph[expr_type] == nullptr) {
        class_table->semant_error(this)
            << "Dispatch to undefined method " << name
            << "." << std::endl;
            set_type(Object);
            return;
    }
    std::vector<Symbol> *method_symbols = class_table->inheritance_graph[expr_type]->find_method(name);
    if (method_symbols == nullptr) {
        class_table->semant_error(this)
            << "Dispatch to undefined method " << name
            << "." << std::endl;
            set_type(Object);
            return;
    }
    // Check that the number of formals provided in the dispatch matches the number from the declaration
    if (method_symbols->size() - 1 != (size_t)actual->len()) {
        class_table->semant_error(this) 
            << "Method " << name
            << " called with wrong number of arguments." << std::endl;
    }
    // Check that the types of the formals provided in the dispatch matches
    for (size_t i = 1; i < method_symbols->size(); ++i) {
        // Semant the expression
        actual->nth(i - 1)->semant(class_table);
        if (!class_table->leq((*method_symbols)[i], actual->nth(i - 1)->get_type())) {
            class_table->semant_error(this) 
                << "In call of method " << name
                << ", type " << actual->nth(i - 1)->get_type() 
                << " of parameter arg does not conform to declared type " << (*method_symbols)[i]
                << "." << std::endl;
        }
    }
    // set the type to the return type
    return_type = (*method_symbols)[0];
    if (return_type == SELF_TYPE) {
        return_type = expr_type;
    }
    set_type(return_type);
}
// A conditional has the form if <expr> then <expr> else <expr> fi 
//      The semantics of conditionals is standard. The predicate is evaluated first. 
//      If the predicate is true,then the then branch is evaluated. If the predicate 
//      is false, then the else branch is evaluated. The value of the conditional is 
//      the value of the evaluated branch. The predicate must have static type Bool.
//      The branches may have any static types. To specify the static type of the conditional, 
//      is A JOIN B where A and B are the static types of the two branch expressions.
void cond_class::semant(ClassTable *class_table) {
    // Semant the predicate, else and then expressions
    pred->semant(class_table);
    then_exp->semant(class_table);
    else_exp->semant(class_table);
    // Make sure that the predicate's static type is Bool
    if (pred->get_type() != Bool) {
        class_table->semant_error(this) 
            << "Predicate of 'if' does not have type Bool." << std::endl;
    }
    // Static type of condition is A JOIN B
    Symbol joined_type = class_table->inheritance_graph.join(then_exp->get_type(), else_exp->get_type())->s;
    set_type(joined_type);
}
// A loop has the form while <expr> loop <expr> pool
//      The predicate is evaluated before each iteration of the loop. If the predicate is false, 
//      the loop terminates and void is returned. If the predicate is true, the body of the 
//      loop is evaluated and the process repeats. The predicate must have static type Bool. 
//      The body may have any static type. The static type of a loop expression is Object.
void loop_class::semant(ClassTable *class_table) {
    // Semant both the predicate and the expression
    pred->semant(class_table);
    body->semant(class_table);
    if (pred->get_type() != Bool) {
        class_table->semant_error(this) 
            << "Loop condition does not have type Bool." << std::endl;
    }
    set_type(Object);
}
// A case expression has the form
//      case <expr0> of
//          <id1> : <type1> => <expr1>;
//          ...
//          <idN> : <typeN> => <exprN>;
//      esac
//      First, expr0 is evaluated and its dynamic type C noted. Next, from among the branches the
//      branch with the least type <typek> such that C <= <typek> is selected. The identifier
//      <idk> is bound to the value of <expr0> and the expression <exprk> is evaluated. The result of 
//      the case is the value of <exprk>.
//      For each branch, let T_i be the static type of <expri>. The static type of a case expression 
//      is JOIN T_1 ... T_N. The identifier id introduced by a branch of a case hides any variable or 
//      attribute definition for id visible in the containing scope.
//      It is illegal to use self as the name of a local variable
void typcase_class::semant(ClassTable *class_table) {
    // semant expr0
    expr->semant(class_table); // No static type check is needed
    // loop and semant each branch of the case
    std::set<Symbol> track;
    Symbol join_type = No_type;
    for (int i = 0; i < cases->len(); ++i) {
        Case branch = cases->nth(i);
        if (track.count(branch->get_decl_type()) > 0) {
            class_table->semant_error(this)
                << "Duplicate branch " << branch->get_decl_type() << " in case statement." << std::endl;
            continue;
        }
        track.insert(branch->get_decl_type());
        branch->semant(class_table);
        Symbol branch_type = branch->get_type();
        // Check type of branch
        if (class_table->inheritance_graph[branch_type] == nullptr) {
            continue;
        }
        else if (join_type == No_type) {
            join_type = branch_type;
            continue;
        }
        join_type = class_table->inheritance_graph.join(join_type, branch_type)->s;
    }
    // Set the type as the joining of all the branch's static types
    set_type(join_type);
}
// A block has the form { <expr>; ... <expr>; }
//      The expressions are evaluated in left-to-right order. Every block has at 
//      least one expression; the value of a block is the value of the last expression.
void block_class::semant(ClassTable *class_table) {
    Symbol block_type;
    for (int i = 0; i < body->len(); ++i) {
        body->nth(i)->semant(class_table);
        block_type = body->nth(i)->get_type();
    }
    set_type(block_type);
}
// A let has the form let <id> : <type> [<- <expr>] in <expr>
//      The type of the let is the type of the body.
//      It is illegal to use self as the name of a local variable
void let_class::semant(ClassTable *class_table) {
    int errors = 0;
    // New scope for the let statement locals
    class_table->symbol_table.enterscope();
    // if the initialization type is SELF_TYPE, reassign
    Symbol type_decl_save = type_decl;
    if (type_decl == SELF_TYPE) {
        type_decl = *(class_table->symbol_table.lookup(SELF_TYPE));
    }
    // Ensure that the intialization type is a valid type
    if (class_table->inheritance_graph[type_decl] == nullptr) {
        class_table->semant_error(this) 
            << "Class " << type_decl 
            << " of let-bound identifier " << identifier
            << " is undefined." << std::endl;
        class_table->symbol_table.addid(identifier, &Object);
        errors = 1;
    }
    else {
        class_table->symbol_table.addid(identifier, &type_decl);
    }
    // Semant the initialization of the variable
    init->semant(class_table);
    // Check that the type of the initialization expression matches the type declaration
    if (errors == 0 && init->get_type() != No_type && !class_table->leq(type_decl, init->get_type())) {
        class_table->semant_error(this) 
            << "Inferred type " << init->get_type()
            << " of initialization of " << identifier
            << " does not conform to identifier's declared type " << type_decl
            << "." << std::endl;
    }
    if (identifier == self) {
        class_table->semant_error(this)
            << "'self' cannot be bound in a 'let' expression." << std::endl;
    }
    // Semant the body
    body->semant(class_table);
    // the type of the let is the type of the body
    set_type(body->get_type());
    class_table->symbol_table.exitscope();
    type_decl = type_decl_save; // restore type declaration if changed
}
// Binary operators semantic analysis:
//      1. analyze sub expressions
//      2. ensure that they are of the proper type
//      3. set output type
// Arithmetic operators + - / * must have subexpressions of type Int
void plus_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    if (e1->get_type() != Int || e2->get_type() != Int) {
        class_table->semant_error(this) 
            << "non-Int arguments: " << e1->get_type() 
            << " + " << e2->get_type() << std::endl;
    }
    set_type(Int);
}
void sub_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    if (e1->get_type() != Int || e2->get_type() != Int) {
        class_table->semant_error(this) 
            << "non-Int arguments: " << e1->get_type() 
            << " - " << e2->get_type() << std::endl;
    }
    set_type(Int);
}
void mul_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    if (e1->get_type() != Int || e2->get_type() != Int) {
        class_table->semant_error(this) 
            << "non-Int arguments: " << e1->get_type() 
            << " * " << e2->get_type() << std::endl;
    }
    set_type(Int);
}
void divide_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    if (e1->get_type() != Int || e2->get_type() != Int) {
        class_table->semant_error(this) 
            << "non-Int arguments: " << e1->get_type() 
            << " / " << e2->get_type() << std::endl;
    }
    set_type(Int);
}
// or < and <= the rules are exactly the same as for the binary arithmetic operations, 
//      except that the result is a Bool.
void lt_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    if (e1->get_type() != Int || e2->get_type() != Int) {
        class_table->semant_error(this) 
            << "non-Int arguments: " << e1->get_type() 
            << " < " << e2->get_type() << std::endl;
    }
    set_type(Bool);
}
void leq_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    if (e1->get_type() != Int || e2->get_type() != Int) {
        class_table->semant_error(this) 
            << "non-Int arguments: " << e1->get_type() 
            << " <= " << e2->get_type() << std::endl;
    }
    set_type(Bool);
}
// The comparison = is a specialcase. 
//      If either <expr1> or <expr2> has static type Int, Bool, or String, 
//      then the other must have the same static type. Any other types, including
//      SELFTYPE, may be freely compared. On non-basic objects, equality simply 
//      checks for pointer equality (i.e., whether the memory addresses of the 
//      objects are the same). Equality is defined for void.
void eq_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    e2->semant(class_table);
    set_type(Bool);
    if (e1->get_type() == Str || e1->get_type() == Bool || e1->get_type() == Int) {
        if (e1->get_type() != e2->get_type()) {
            class_table->semant_error(this) 
                << "Illegal comparison with a basic type." << std::endl;
            return;
        }
    }
    if (e2->get_type() == Str || e2->get_type() == Bool || e2->get_type() == Int) {
        if (e1->get_type() != e2->get_type()) {
            class_table->semant_error(this) 
                << "Illegal comparison with a basic type." << std::endl;
            return;
        }
    }
}
// The expression ~<expr> is the integer complement of <expr>. 
//      The expression <expr> must have static type Int and the entire expression 
//      has static type Int.
void neg_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    if (e1->get_type() != Int) {
        class_table->semant_error(this)
            << "Argument of '~' has type " << e1->get_type()
            << " instead of Int." << std::endl;
    }
    set_type(Int);
}
// The expression not <expr> is the boolean complement of <expr>.
//      The expression <expr> must have static type Bool and the entire expression 
//      has static type Bool.
void comp_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    if (e1->get_type() != Bool) {
        class_table->semant_error(this)
            << "Argument of 'not' has type " << e1->get_type()
            << " instead of Bool." << std::endl;
    }
    set_type(Bool);
}
// Constant expressions have already been verified in early compiler steps
void int_const_class::semant(ClassTable *class_table) {
    // No error can happen here, just set the type
    set_type(Int);
}
void bool_const_class::semant(ClassTable *class_table) {
    // No error can happen here, just set the type
    set_type(Bool);
}
void string_const_class::semant(ClassTable *class_table) {
    // No error can happen here, just set the type
    set_type(Str);
}
// A new expression has the form new <type>
//      The value is a fresh object of the appropriate class. 
//      If the type is SELF_TYPE, then the value is a fresh object of the class of
//      self in the current scope. The static type is <type>.
void new__class::semant(ClassTable *class_table) {
    if (type_name == SELF_TYPE) {
        set_type(*(class_table->symbol_table.lookup(SELF_TYPE)));
        return;
    }
    if (class_table->inheritance_graph[type_name] == nullptr) {
        // Class not declared
        class_table->semant_error(this)
            << "'new' used with undefined class " << type_name
            << "." << std::endl;
        set_type(Object);
        return;
    }
    set_type(type_name);
}
// The expression isvoid expr evaluates to true if expr is void and evaluates to false 
//      if expr is not void.
void isvoid_class::semant(ClassTable *class_table) {
    e1->semant(class_table);
    set_type(Bool);
}
// No type
void no_expr_class::semant(ClassTable *class_table) {
    set_type(No_type);
}
// Calling objects by id will check the scope. Error if the id is not in scope
// Can also call "self" identifier which refers to SELF_TYPE in current scope
void object_class::semant(ClassTable *class_table) {
    // Check if refering to self
    if (name == self) {
        // set expression type to SELF_TYPE in current scope
        set_type(SELF_TYPE);
        return;
    }
    // Check if object id is in scope
    if (class_table->symbol_table.lookup(name) == NULL) {
        // Not in scope
        class_table->semant_error(this)
            << "Undeclared identifier " << name
            << "." << std::endl;
        set_type(Object);
        return;
    }
    Symbol object_type = *class_table->symbol_table.lookup(name);
    set_type(object_type);
}

ClassTable::InheritanceGraph::Node::Node(Symbol symbol) : s(symbol), parent(nullptr), resolved(false) {}

ClassTable::InheritanceGraph::InheritanceGraph(ClassTable *table, Classes classes) : errors(0) {
    // Pass ONE, add all classes to the graph
    std::set<Symbol> basic_classes { Object, Int, Str, Bool, IO };
    for (int i = 0; i < classes->len(); ++i) {
        Class_ current = classes->nth(i);
        // If the name is the same as a basic class and it is NOT the basic class, an error
        if (basic_classes.count(current->get_name()) > 0 && !current->get_filename()->equal_string("<basic class>", 13)) {
            table->semant_error(current) << "Redefinition of basic class " << current->get_name() << "." << std::endl;
            ++errors;
            continue;
        }
        // If redefining SELF_TYPE
        if (current->get_name() == SELF_TYPE) {
           table->semant_error(current) << "Redefinition of basic class SELF_TYPE." << std::endl;
            ++errors;
            continue;
        }
        // If already added to the table, an error
        if ((*this)[current->get_name()] != nullptr) {
            table->semant_error(current->get_filename(), current) << "Class " << current->get_name() 
                << " was previously defined." << std::endl;
            ++errors;
            continue;
        }
        nodes.push_back(new Node(current->get_name()));
    }
    // Pass TWO, add all edges to the graph (i.e. inheritance)
    for (int i = 0; i < classes->len(); ++i) {
        Class_ current = classes->nth(i); 
        if (current->get_name() == Object) continue; // Object does not inherit, no need to check
        // Check if the parent is in the graph, if not, error
        ClassTable::InheritanceGraph::Node *parentNode = (*this)[current->get_parent()];
        if (parentNode == nullptr) {
            table->semant_error(current->get_filename(), current) << "Class " << current->get_name() 
                << " inherits from an undefined class " << current->get_parent() << "." << std::endl;
            ++errors;
            continue;
        }
        else if (current->get_parent() == Bool) {
            table->semant_error(current->get_filename(), current) << "Class " << current->get_name() 
                << " cannot inherit class Bool." << std::endl;
            ++errors;
            continue;
        }
        else if (current->get_parent() == Int) {
            table->semant_error(current->get_filename(), current) << "Class " << current->get_name() 
                << " cannot inherit class Int." << std::endl;
            ++errors;
            continue;
        }
        else if (current->get_parent() == Str) {
            table->semant_error(current->get_filename(), current) << "Class " << current->get_name() 
                << " cannot inherit class String." << std::endl;
            ++errors;
            continue;
        }
        else if (current->get_parent() == current->get_name()) {
            table->semant_error(current->get_filename(), current) << "Class " << current->get_name() 
                << ", or an ancestor of " << current->get_name() << ", is involved in an inheritance cycle." << std::endl;
            ++errors;
            continue;
        }
        // Assign edge to the graph
        (*this)[current->get_name()]->parent = parentNode;
    }
    // Pass THREE, make sure no cycles exist, halt if errors found previously
    if (errors == 0) {
        for (int i = classes->len() - 1; i >= 0; --i) {
            Class_ current = classes->nth(i);
            std::set<Symbol> trace;
            ClassTable::InheritanceGraph::Node *cursor = (*this)[current->get_name()];
            while (cursor != nullptr) {
                // If the cursor's symbol is in the graph, then there is a cycle
                if (trace.count(cursor->s) > 0) {
                    table->semant_error(current->get_filename(), current) << "Class " << cursor->s 
                        << ", or an ancestor of " << cursor->s << ", is involved in an inheritance cycle." << std::endl;
                    errors++;
                    break;
                }
                trace.insert(cursor->s);
                cursor = cursor->parent;
            }
        }
    }
}

ClassTable::InheritanceGraph::Node *ClassTable::InheritanceGraph::join(Symbol a, Symbol b) const {
    ClassTable::InheritanceGraph::Node *aNode = (*this)[a];
    ClassTable::InheritanceGraph::Node *bNode = (*this)[b];
    ClassTable::InheritanceGraph::Node *cursor = bNode;
    ClassTable::InheritanceGraph::Node *output = nullptr;

    while (aNode != nullptr) {
        cursor = bNode;
        while (cursor != nullptr) {
            if (cursor->s == aNode->s) {
                return cursor;
            }
            cursor = cursor->parent;
        }
        aNode = aNode->parent;
    }
    
    return nullptr;
}

std::vector<Symbol> *ClassTable::InheritanceGraph::Node::find_inheritted_method(Symbol a) {
    Node *cursor = parent;
    while (cursor != nullptr) {
        if (cursor->method_table.count(a) == 0) {
            cursor = cursor->parent;
            continue;
        }
        return &(cursor->method_table.at(a));
    }
    return nullptr;
}

std::vector<Symbol> *ClassTable::InheritanceGraph::Node::find_method(Symbol a) {
    if (method_table.count(a) == 0) {
        return find_inheritted_method(a);
    }
    return &(method_table.at(a));
}

Symbol ClassTable::InheritanceGraph::Node::find_inheritted_attr(Symbol a) {
    Node *cursor = this->parent;
    while (cursor != nullptr) {
        if (cursor->attr_table.count(a) == 0) {
            cursor = cursor->parent;
            continue;
        }
        return cursor->attr_table.at(a);
    }
    return nullptr;
}

Symbol ClassTable::InheritanceGraph::Node::find_attr(Symbol a) {
    if (attr_table.count(a) == 0) {
        return find_inheritted_attr(a);
    }
    return attr_table.at(a);
}

bool ClassTable::InheritanceGraph::Node::operator<=(Node * right) {
    if (this == right) {
        return true;
    }
    else {
        if (right->parent == nullptr) {
            return false;
        }
        return *(this) <= right->parent;
    }
}

ClassTable::InheritanceGraph::~InheritanceGraph() {
    for (size_t i = 0; i < nodes.size(); ++i) {
        delete nodes[i];
    }
}

ClassTable::InheritanceGraph::Node *ClassTable::InheritanceGraph::operator[](const Symbol s) const {
    for (size_t i = 0; i < nodes.size(); ++i) { 
        if (nodes[i]->s == s) {
            return nodes[i];
        }
    }
    return nullptr;
}

// initialize inheritance graph
ClassTable::ClassTable(Classes classes) : 
    semant_errors(0),
    error_stream(cerr),
    inheritance_graph(this, insert_basic_classes(classes)) {}

Classes ClassTable::insert_basic_classes(Classes user_classes) {
    // The tree package uses these globals to annotate the classes built below.
    curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");

    Classes basic_classes = nil_Classes();

    basic_classes = append_Classes(basic_classes, single_Classes(
        class_(Object, 
               No_class,
               append_Features(
                               append_Features(
                                               single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
                                               single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
                               single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
               filename)));

    basic_classes = append_Classes(basic_classes, single_Classes(
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
               filename)));  

    basic_classes = append_Classes(basic_classes, single_Classes(
        class_(Int, 
               Object,
               single_Features(attr(val, prim_slot, no_expr())),
               filename)));

    basic_classes = append_Classes(basic_classes, single_Classes(
        class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename)));
    
    basic_classes = append_Classes(basic_classes, single_Classes(
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
               filename)));

    
    return append_Classes(basic_classes, user_classes);
}

void ClassTable::install_basic_class_methods() {
    Classes basic_classes = insert_basic_classes(nil_Classes());
    for (int i = 0; i < basic_classes->len(); ++i) {
        Class_ basic_class = basic_classes->nth(i);
        basic_class->register_symbols(this);
        inheritance_graph[basic_class->get_name()]->resolved = true;
    }
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename
//
//    All versions return an output stream to which you should write
//    an appropriate human-readable error message.
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{                                                             
    return semant_error(c->get_filename(),c);
}    

ostream& ClassTable::semant_error(tree_node *t)
{
    error_stream << current_filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 

void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    // Performs THREE passes to ensure inheritance correctness
    ClassTable *class_table = new ClassTable(classes);

    // If there were no issues with the inheritance processing, proceed
    if (class_table->inheritance_graph.errors == 0) {
        // First populate the method tables with the built in functions
        class_table->install_basic_class_methods();
        // Pass FOUR, register the class symbols
        int unregistered = classes->len();
        while (unregistered > 0) {
            for (int i = 0; i < classes->len(); ++i) {
                Class_ current_class = classes->nth(i);
                // Skip classes whose direct parent is not yet registered
                if (class_table->inheritance_graph[current_class->get_name()]->parent->resolved == false) {
                    continue;
                }
                if (class_table->inheritance_graph[current_class->get_name()]->resolved == false) {
                    current_class->register_symbols(class_table);
                    class_table->inheritance_graph[current_class->get_name()]->resolved = true;
                    if (unregistered-- == 0) {
                        break;
                    }
                }
            }
        }
        // Just check to make sure that a main class exists
        if (class_table->inheritance_graph[Main] == nullptr) {
            class_table->semant_error() << "Class Main is not defined." << std::endl;
        }
        else {
            for (int i = 0; i < classes->len(); ++i) {
                if (classes->nth(i)->get_name() == Main) {
                    if (class_table->inheritance_graph[Main]->method_table.count(main_meth) == 0) {
                        class_table->semant_error(classes->nth(i)) << "No 'main' method in class Main." << std::endl;
                    }
                    else if (class_table->inheritance_graph[Main]->method_table[main_meth].size() > 1) {
                        class_table->semant_error(classes->nth(i)) << "'main' method in class Main should have no arguments." << std::endl;
                    }
                }
            }
        }
        // Pass FIVE, semant the class list
        for (int i = 0; i < classes->len(); ++i) {
            classes->nth(i)->semant(class_table);
        }
    }

    if (class_table->errors()) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}