
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Type {
  a : Int <- 10;
  get() : Int { a };
  add(a: Int, b: Int, c: Int) : Int { a };
};

class A {
  a : Int <- 1 + 3;
  w : Int <- 1 + (2 + 2);
  f : String;
  test(): Int { 0 };
  copy() : SELF_TYPE { self };
};

class B inherits A {
  b : Type <- new Type;
  c : String <- "Hello World";
  d : Bool <- isvoid(b);
  g : Int <- b.add(1, 2, 3);
  test(): Int { b.get() };
};

class Main inherits IO {
  main(): Object {
    out_string("Hello World\n")
  };
};

