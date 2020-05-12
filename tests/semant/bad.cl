(*
These errors will halt the semantic analysis, hence why they are commented
out.


	(* Inheritance from undeclared class

	class Main inherits A {

	};

	*)

	(* Inheritance from a basic class

	class Main inherits Int {

	};

	*)

	(* Redefinition of a basic class

	class Int {

	};

	class Main {

	};

	*)

	(* Multiple Class Definition

	class A {

	};

	class A {

	};

	class Main {

	};

	*)

	(* Circular Inheritance

	class A inherits B {

	};

	class B inherits A {

	};

	class Main {

	};

	*)

	(* Redefine SELF_TYPE
	
	class SELF_TYPE {
	
	};

	*)

*)

-- Type mismatch between attribute and declaration
class AttrTypeMismatch {
	a : Int <- "String";
};

-- Type mismatch between method and expression
class MethodTypeMismatch {
	a() : Int {
		"String"
	};
};

-- Binary arithmetic and comparison operator not Int
class BinaryOperatorTypeMismatch {
	a() : Int {
		1 + "String"
	};
	b() : Bool {
		1 < "String"
	};
};

-- Equality operator type mismatch
class EqualityOperatorTypeMismatch {
	obj : BinaryOperatorTypeMismatch;
	a() : Int {
		obj = 1
	};
	b() : Bool {
		1 = "String"
	};
};

-- Unary operators wrong type
class UniaryOperatorWrongMismatch {
	a() : Int {
		~"String"
	};
	b() : Bool {
		not 1
	};
};

-- Assignment operator type mismatch
class AssigmentTypeMismatch {
	a(a : String) : Int {
		a <- 1
	};
};

-- Formal declared type not defined
class FormalTypeUndefined {
	a(a : Potato) : Int {
		a <- 1
	};
};

-- Object ID not in scope
class ObjectNotInScope {
	a() : Int {
		a <- 1
	};
};

-- New operator on an undeclared type
class NewUndefined {
	a() : Int {
		new Potato
	};
};
-- Let operator errors
class LetExpressionError {
	undeclaredType() : Object {
		let i : Potato in i
	};
	mistmatchType() : Object {
		let i : Int <- "String" in i
	};
	selfAssignment() : Object {
		let self : Int in i
	};
};
-- Case tests errors :
class CaseExpressionError {
	undefinedType(x : Int) : Object {
		case x of
			y : Potato => 1;
		esac
	};
	selfAssignment(x : Int) : Object {
		case x of
			self : SELF_TYPE => 1;
		esac
	};
};
-- Incorrect Dispatch
class IncorrectDispatchError {
	staticDispatch() : Int {
		self@Potato.wrong()
	};
	normalDispatch() : Int {
		staticDispatch(1)
	};
};
-- Multiple Formals
class MultipleFormalsError {
	a(x: Int, x: Int) : Int {
		1
	};
};
-- Redefinition of a feature
class FeatureRedefinition {
	a : Int;
	a : Int;
	a() : Object {
		1
	};
	a() : Object {
		1
	};
};
-- self name of formal 
class FormalSelfName {
	a(self: Int) : Int{
		1
	};
	b(i : SELF_TYPE) : Int {
		1
	};
};
-- Incorrect Redefinition of an inherited feature
class Parent {
	doNotRedefine : Int;
	myMethodWrongFormals(a : Int) : Int {
		1
	};
	myMethodWrongFormalType(a : Int) : Int {
		1
	};
	myMethodWrongReturnType(a : Int) : Int {
		1
	};
};
class ChildA {
	doNotRedefine : Int;
	myMethodWrongFormals() : Int {
		1
	};
	myMethodWrongFormalType(a : String) : Int {
		1
	};
	myMethodWrongReturnType(a : Int) : String {
		1
	};
};

-- No main class

class Test {
	x : SELF_TYPE <- new Test;
};