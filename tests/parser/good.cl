(* correct class definition *)
Class A {
  correctAttr : String <- "Hello World";
  correctUninitAttr : String;
  correctMethod ( correctFormal1 : Int, correctFormal2 : String ) : String{
    "What's up?"
  };
};

(* correct inheritance *)
Class B inherits A {
};


Class D {
  caseTest (object : Object) : String {
    case (object) of
      s : String => "String";
      o : Object => "Object"; 
    esac
  };

  branchTest (value : Bool) : Bool {
    if value 
      then true 
      else if false 
        then true 
        else false 
      fi 
    fi
  };

  loopTest (value : Int) : Int {
    while (value < 0) loop
      value <- value - 1
    pool
  };

  operatorBlockTest (input : Int) : Int {
    {
      1 + 1;
      2 - 2;
      10 / 2;
      5 * 2;
      input <- 0;
      10 < 5;
      11 <= 5;
      11 = 11;
    }
  };

  letStatementTest () : String {
    let i : Int, j : Int, g : Int in i + j + g
  };

  confusingLetTest() : Int {
    let x : Int <- let y : Int <- 5 in y + 3 in x + 2
  };

  dispatchStatementTest() : String { 
    (new A).correctMethod(1, "Hello")
  };

  staticDispatchTest(b : B) : String {
    b@A.correctMethod(1, "Hello")
  };
  
  impliedSelfDispatchTest() : String {
    dispatchStatementTest()
  };
};