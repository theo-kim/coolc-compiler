# Cool Compiler
This is my implementation of the Cool Compiler for the Compiler Design and Construction Course at New York University Tandon School of Engineering (Professor Jeffrey Epstein, Spring 2020).

## Cool and Acknowledgements
**COOL** (**C**lassroom **O**bject **O**riented **L**anguage) is a language developed for undergraduate Compiler Theory courses. A full description of the language and its origin can be found on the Stanford University Computer Science website [here](https://theory.stanford.edu/~aiken/software/cool/cool.html).

## Repo Format
This repository is composed for the following components:

- **./cool-examples**: example Cool programs, downloaded from the course documentation. This is used in testing the compiler
- **./src**: header and source files used in the compilation process. This is also from the course documentation
- **./refimpl**: a reference implementation of the Cool compiler, provided by the course documentation, this is often used in testing and is described further in the testing section. NOTE!! Because I used x86 for my target architecture and not MIPS, the MIPS code generator was renamed cgen-mips in the reference implementation and the cgen-x86 was renamed to cgen.
- **./myimpl**: my implementation of the compiler based upon the language behavior described in the Cool Manual. It is written in C++ using Flex (for the lexer) and Bison (for the parser). Composed of four layers: 
  - *Lexical Analysis (Lexer)* (`lexer`): tokenized input source code.
  - *Parser* (`parser`): takes the tokenized input and a previously defined grammar and produces an abstract syntax tree.
  - *Semantic Analyzer* (`semant`): Takes the AST from the previous step and performs static type analysis to ensure runtime safety and produces an AST annotated with type information.
  - *Code Generator* (`cgen`): Takes the annotated AST and produces executable machine code in x86.
- **./tests**: test files to run using each layer.
- **README.md**: this file.
- **docker.sh**: a driver file for running the Docker image.
- **Dockerfile**: describes the docker image to tests the file.

## Running with Docker
This repo comes with a Dockerfile to create an image to test the compiler. Make sure you have Docker 2 installed. It uses ubuntu:18.08 as the base image.

### Building the Docker Image
To build the image (which includes compiling the source for the compiler) and tag it with the label `coolc` run the following command:

```
docker build --tag coolc .
```

Building the image will FAIL if any portion of my implementation fails to build. So if I (or anyone who forks this repo) alters the source to a state where it no longer compiles without errors, you will be unable to run the docker image.

### Running and Testing the Docker Image
After building the Docker image (remember to rebuild after updating the source), use the following command to run the compiler using Cool programs located in the test folder (test programs are passed as a volume to the docker image so that rebuilding the container is not needed when updating the test files). Furthermore, make sure to run the following command from the project root.

```
docker run --rm -v "$PWD/tests":/compiler/tests coolc <args>
```
Where `<args>` is either a `pipeline` argument followed by an `input file` argument (as described below) or is the name of a `test` to run and an `input file` (also described below). 

#### `pipeline`

`pipeline` describes how to run the compiler using either the reference implementation or my implementation. It is written in the following format:

- `<lexer>-><parser>-><semant>-><cgen>` where each section is replaced with either a `ref` or `my`, so a pipeline value of `ref->ref->my->ref` will run the compiler with the reference implementation but replace the semantic analysis with my implementation. 
  - You can also omit sections to change the compiler output, so running `ref->ref->my` will omit the code generator from the compiler pipeline (note, for obvious reasons you cannot omit an intermediary step of the compiler, i.e. you cannot omit the `parser` and include `semant` or `cgen` as they are both dependent on the parser's output).
  - You can also use the following shorthand: `[my/ref]-<section>` (i.e. `my-parser` or `ref-cgen`) to identify the last step in the compiler you want to run and the implementation you want to run it with (doing this will automatically use the reference implementation for earlier steps of the compiler). Hence, the command `my-cgen` is equivalent to `ref->ref->ref->my`.
  - Finally, if you want to run the whole compiler and then run the compiled Cool program omit the previous pipeline arguments and either use the argument `my` to compile the program entirely with my implementation, or `ref` to compile it entirely with the reference implementation. This will NOT show the intermediate output of the compiler steps, just the output of the Cool program.

#### `input file`

The `input file` is the name of the Cool source file to compile. If the input file being used is in the `tests` folder, you would use the path: `tests/<file>`. If the input file being used is in the `cool-examples` folder, prepend the file name with `cool-examples/` instead of `tests`.

#### `test`

Alternatively, you can test the difference between my implementation and the reference implementation at any stage of the compiler. It will require the name of the step to test followed by an `input file` argument. It will run the compiler twice, once using the reference implementation for all the steps of the compiler until the specified step, and then again using my implementation for the specified step of the compiler. Then it will compare the two files and report back the differences.

An example usage of the test functionality is below:

```
$ docker run --rm -v "$PWD/tests":/compiler/tests coolc test parser mytest.cl
```

This command will test the difference between the abstract syntax tree of my implementation of the parser versus the reference implementation. NOTE: for consistency, for earlier steps of the compiler, the reference implementation is used (in this case, the reference implementation of the lexer is used). You can change this behavior with the command `mytest` which will use my implementation for earlier steps of the compiler for both tests, so the following command: 

```
$ docker run --rm -v "$PWD/tests":/compiler/tests coolc mytest semant mytest.cl
```

Will compare my implementation of the semantic analyzer and the reference implementation of the semantic analyzer, when both are fed the AST generated after running my implementation of the lexer and parser on the input file.

### Quick Start

I provided a bash environment file to make the above commands easier. Load it into your bash shell with the command `source .bash_profile` (from the repo root). Then you can use the command aliases:

- `coolc-build` to build the image (instead of `docker build...`).
- `coolc-run` to run the image (instead of `docker run ...`), you still need to pass arguments like defined above. NOTE: this will attach stdin, stdout and stderr for you.
- `coolc-test` to test the files (instead of `docker run coolc test`), you still need to pass arguments like defined above, but you can omit the `test`.
- `coolc-mytest` to test the files (instead of `docker run coolc mytest`), you still need to pass arguments like defined above, but you can omit the `mytest`.

### Troubleshooting

> I keep getting `I/O error` when running the compiled Cool program.

This is because Docker does not automatically bind and allocate an input stream for the running process. You will need to manually attach STDIN and STDOUT and signify it as interactive. Add the following flags: `-a stdin -a stdout -a stderr -it`. For example, to run the `cool-examples/arith.cl` file, use the following command: 

```
$ docker run --rm -v "$PWD/tests":/compiler/tests -a stdin -a stdout -a stderr -it coolc ref cool-examples/arith.cl
```

> Changes aren't appearing in the compiler after altering the source

Did you rebuild the docker image before running it? Yeah... that's usually why it isn't working...

