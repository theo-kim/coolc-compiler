
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Type inherits IO {
  a : Int <- 10;
  add(a: Int, b: Int, c: Int) : Int { 
    {
      {
        out_string("\ta = ");
        out_int(a);
        out_string(" = 1\n");
      };
      {
        out_string("\tb = ");
        out_int(b);
        out_string(" = 2\n");
      };
      {
        out_string("\tc = ");
        out_int(c);
        out_string(" = 4\n");
      };
      out_string("\ta + b + c = ");
      let x : Int <- a, -- check let statement
        y : Int <- b, 
        z : Int <- c in x + y + z; -- check addition
    }
  };
};

class A {
  a : Int <- 3;
  w : Int <- 1 + (2 + 2);
  f : String;
  test(): String { "Static Dispatch passed!\n" };
};

class B inherits A {
  b : Type <- new Type;
  c : String <- "Hello World";
  d : Bool <- not (1 = 1);
  g : Int;
  test(): String { "Dispatch passed!\n" };
};

class C inherits A {};

class D {};

class Main inherits IO {
  a : B <- new B;
  b : Int <- 3;
  t : Type <- new Type;
  w : Type;
  -- This basically tests everything
  main(): Object {
    { -- test block
      out_string(a.test()); -- Check normal dispatch
      out_string(a@A.test()); -- Check static dispatch
      while 0 < b loop -- Check loop
        {
          out_string("Looping: ");
          out_int(b);
          out_string("\n");
          b <- b - 1; -- Check subtraction
        }
      pool;
      {
        out_string("Testing multiplication, division and let:\n\t7 = ");
        let x : Int <- (12 * 2) / 3 in out_int(x - 1);
      };
      { 
        out_string("\nChecking addition, parameters, and let statement: \n");
        out_int(t.add(1, 2, 4)); -- Check dispatch
        out_string(" = 7\n");
      };
      out_string("Testing case: ");
      case self of -- case statement
        v : Int => out_string("Case failed\n");
        w : Bool => out_string("Case failed\n");
        x : String => out_string("Case failed\n");
        y : A => out_string("Case failed\n");
        z : Object => out_string("Case passed\n");
      esac;
      out_string("Testing conditional: ");
      if ( 18 + 7 ) < ( b + 2 ) then -- less than test
        out_string("Conditional failed\n") -- conditional test
      else 
        out_string("Conditional passed\n")
      fi;
      -- isvoid()
      if isvoid(w) then out_string("isvoid() passed\n") else out_string("isvoid() failed\n") fi;
      {
        out_string("Testing assignment: "); -- assignment
        b <- 4;
        out_int(b + 2);
        out_string(" = 6\n");
      };
      { -- void dispatch runtime error
        out_string("This should generate and error!\n");
        w.add(1, 2, 3);
      };
    }
  };
};

