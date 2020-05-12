(* 1A. The class TYPEID has a lexer error. *)
class a {
};

(* 1B. The feature list has an error *)
class A { 
  imAnError;
};

(* 1C. Both the TYPEID and feature list have errors *)
class b {
  imAnError;
};

(* 2A. Correct class opening, incorrect feature definition *)
class C {
  BadAttribute : String;
  BadMethod () : String { "Hello" }; (* this should not be hit *)
};

(* 2B. The method body has an error *)
class D {
  goodFunction (formal1 : Int) : String {
    THIS_IS_A_MISTAKE
  };
};

(* 2C. The method formal list has an error *)
class E {
  badFormals (formal1 : wrongType) : String {
    "Ok"
  };
};

(* 2D. Formal list begins with a comma *)
class F { 
  badFormals (, formal1 : Type) : String {
    "Expression"
  };
};

(* 3A. Nonassociativity of Boolean operators *) 
class G {
  booleanOpTest () : Bool {
    1 <= 1 <= 2
  };
};

(* 3B. Non-associativity of isvoid operator *) 
class H {
  isvoidOpTest () : Bool {
    isvoid isvoid B
  };
};

(* 3C. Incorrect dispatch  *) 
class I {
  dispatchTest () : Bool {
    {
      A.test(); (* cannot dispatch from typename *)
      b@C@d.method(); (* cannot static dispatch from an instance *)
    }
  };
};

(* 5. Expression List errors *)
class J {
  blockTest () : String {
    {
      "A";
      //; (* This is an error *)
      "B";
    }
  };
  methodTest (a : A) : String {
    a.goodFunction(1, 2, **, 3)
  };
};

(* 6A. Missing Let punctuation *)
class K {
  letPuncTest () : String {
    let i : Int j : String in y
  };
};

(* 6B. Error in expression following IN *)
class L {
  letExpr () : String {
    let i : Int, j : String in Potato
  };
};

(* 6C. Error in expression in assignment to binding list element *)
class M {
  letExpr () : String {
    let i : Int <- 1, j : Int <- WRONG! in i + j
  };
};