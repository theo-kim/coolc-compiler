-- Correct Inheritance
class A {
	a : Int;
	b : String;
	c (x: String) : String {
		x
	};
};

class B inherits A {
	c (x : String) : String {
		b
	};
};

-- Correct Expression

class CorrectExpressions {
	caseExpression (x : Int) : Int {
		case x of
			y : Int => y;
		esac
	};
	letExpression (x : Int) : Int {
		let i : Int in i
	};
	condExpression () : String{
		if true then "Me" else "You" fi
	};
	loopExpression () : Object {
		while true loop 1 pool
	};
	assignmentExpression (x : Int) : Object {
		x <- 1
	};
	operatorsInBlock (x : Int, y : Int) : Int {
		{
			x + y;
			y - x;
			y * 1;
			x / 1;
		}
	};
};

-- Correct Dispatch

class CorrectDispatch inherits A {
	staticDispatch() : String {
		self@A.c("hello")
	};
	dispatch() : String {
		staticDispatch()
	};
};

Class Main inherits IO {
	main() : Object {
	  out_string("Hello World")
	};
};
