(*
 *  CS164 Fall 94
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Skeleton file
 *)

 -- This class is parent class for all actions that can be pushed 
 -- onto the Stack
class StackCommand inherits IO {
   next : StackCommand;

    getNext() : StackCommand {
        next
    };

    setNext(node : StackCommand) : StackCommand {
        { next <- node; self; }
    };

    display() : String {
       "Empty Command"
    };
};

-- This is a dummy class for the bottom of the stack
class StackEnd inherits StackCommand {
   display() : String {
       "END"
   };
};

-- Abstract class
class StackOperation inherits StackCommand { };

-- This class is used for additions on the stack
class StackAddition inherits StackOperation {
    arg2 : StackCommand;
    sum : Int;
    converter : A2I <- new A2I;

    do(arg : StackCommand) : StackCommand {
        { 
            arg2 <- arg.getNext();
    	    sum <- (converter.a2i(arg.display()) + converter.a2i(arg2.display()));
	        (new StackValue.setValue(converter.i2a(sum))).setNext(arg2.getNext()); 
        }
    };
    
    display() : String {
        "+"
    };
};

class StackSwap inherits StackOperation {
    temp : StackCommand;

    do(arg : StackCommand) : StackCommand {
        { 
            temp <- arg.getNext();
	        arg.setNext(temp.getNext());
	        temp.setNext(arg);
        }
    };
      
    display() : String {
        "s"
    };
};

class StackValue inherits StackCommand { -- This is used to define integer values for the Stack
    value : String;

    setValue(newValue : String) : StackCommand {
    	{ value <- newValue; self; }
    };

    display() : String {
        value
    };
};

class StackMachine inherits IO { -- This class keeps track of the Stack Machine
    -- a pointer to the end of the stack
    bottom : StackCommand <- new StackEnd;
    -- the top of the stack
    stack : StackCommand <- bottom;
    -- temporary variable for iteration
    cursor : StackCommand;

    -- push new command to the stack based on the input character
    push(symbol : String) : StackCommand {
        if symbol = "+"
	        then stack <- (new StackAddition).setNext(stack)
	    else if symbol = "s"
	        then stack <- (new StackSwap).setNext(stack)
	    else if symbol = "d"
	        then { displayStack(); stack; }
	    else if symbol = "e"
	        then { evaluate(); stack; }
	    else
	        stack <- (new StackValue).setValue(symbol).setNext(stack)
	    fi fi fi fi
    };

    -- perform the function of the d command
    displayStack() : Object {
        {
            cursor <- stack;
            out_string(cursor.display());
            while (not ((cursor <- cursor.getNext()) = bottom))
            loop
                { out_string(","); out_string(cursor.display()); }
            pool;
            out_string("\n");
        }
    };

    -- perform the evaluation based on top of the stack
    evaluate() : Object {
        case stack of
            -- for the addition
      	    a : StackAddition => stack <- a.do(stack.getNext());
	        -- for the swap
            s : StackSwap => stack <- s.do(stack.getNext());
            -- otherwise do nothing
            default : Object => self;
        esac
    };
};

class Main inherits IO {
    machine : StackMachine <- new StackMachine;
    command : String;

    -- Display prompt
    promptLine() : String {
        self.out_string(">").in_string()
    };

    main() : Object {
        while (not ((command <- self.promptLine()) = "x"))
        loop
            { out_string("\n"); machine.push(command); }
        pool
    };
};