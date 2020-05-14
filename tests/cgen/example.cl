
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Type {
  a : Int <- 10;
  get() : Int { a };
  add(a: Int, b: Int, c: Int) : Object { let i : Int <- 1 + 2 in i };
};

class A {
  a : Int <- 3;
  w : Int <- 1 + (2 + 2);
  f : String;
  test(): Object { 0 };
  copy() : SELF_TYPE { self };
};

class B inherits A {
  b : Type <- new Type;
  c : String <- "Hello World";
  d : Bool <- not (1 = 1);
  g : Int;
  test(): Object { b.add(1, 2, 3) };
};

class Main inherits IO {
  a : B <- new B;
  main(): Object {
    {
      a@A.test();
      out_string("Hello World\n");
    }
  };
};

