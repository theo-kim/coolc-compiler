#!/bin/bash

echoerr() { echo -e "\e[31m$@\e[0m" 1>&2; }
function compare {
    count=0
    for var in "$@"
    do
        if [ "$count" -eq "0" ]; then
            :
        else if [ "$count" -eq "1" ]; then
            $var $1 2>/tmp/myerr > /tmp/my
        else if [ "$count" -lt "5" ]; then
            mv /tmp/my /tmp/myin
            $var < /tmp/myin > /tmp/my 2>>/tmp/myerr
            rm /tmp/myin
        else if [ "$count" -eq "5" ]; then
            $var $1 2>/tmp/referr > /tmp/ref
        else
            mv /tmp/ref /tmp/refin
            $var < /tmp/refin > /tmp/ref 2>>/tmp/referr
            rm /tmp/refin
        fi fi fi fi
        ((count=count+1))
    done
    echo -e "\033[0;31mDifferent error messages\033[0m"
    
    diff /tmp/myerr /tmp/referr
    rm /tmp/myerr /tmp/referr

    echo -e "\033[0;32mDifferent output\033[0m"
    diff /tmp/my /tmp/ref
    rm /tmp/my /tmp/ref
}

function run_test {
    # check version
    if [ "$1" = "my" ]; then
        lexer="/compiler/myimpl/lexer/lexer"
        parser="/compiler/myimpl/parser/parser"
        semant="/compiler/myimpl/semant/semant"
        cgen="/compiler/myimpl/cgen/cgen"
    else
        lexer="/compiler/refimpl/lexer"
        parser="/compiler/refimpl/parser"
        semant="/compiler/refimpl/semant"
        cgen="/compiler/refimpl/cgen"
    fi
    # check input file
    if [ ! -f "/compiler/${3}" ]; then
        echoerr "[ERROR] Input file does not exist: ${3}"
    else
        # run the file with the compiler
        input_file="/compiler/${3}"
        if [ "$2" = "lexer" ]; then
            compare $input_file "/compiler/myimpl/lexer/lexer" "echo" "echo" "echo" "/compiler/refimpl/lexer" "echo" "echo" "echo"
        else if [ "$2" = "parser" ]; then
            compare $input_file $lexer "/compiler/myimpl/parser/parser" "echo" "echo" $lexer "/compiler/refimpl/parser" "echo" "echo"
        else if [ "$2" = "semant" ]; then
            compare $input_file $lexer $parser "/compiler/myimpl/semant/semant" "echo" $lexer $parser "/compiler/refimpl/semant" "echo"
        else if [ "$2" = "cgen" ]; then
            compare $input_file $lexer $parser $semant "/compiler/myimpl/cgen/cgen" $lexer $parser $semant "/compiler/refimpl/cgen"
        else 
            echoerr "[ERROR] Unknown compiler step: ${2}"
        fi fi fi fi
    fi
}

function run {
    # check version
    if [ "$1" = "my" ]; then
        last_impl="/compiler/myimpl/${2}/${2}"
    else
        last_impl="/compiler/refimpl/${2}"
    fi
    # check input file
    if [ ! -f "/compiler/${3}" ]; then
        echoerr "[ERROR] Input file does not exist: ${3}"
    else
        # run the file with the compiler
        input_file="/compiler/${3}"
        if [ "$2" = "lexer" ]; then
            $last_impl $input_file
        else if [ "$2" = "parser" ]; then
            /compiler/refimpl/lexer $input_file | $last_impl
        else if [ "$2" = "semant" ]; then
            /compiler/refimpl/lexer $input_file | /compiler/refimpl/parser | $last_impl
        else if [ "$2" = "cgen" ]; then
            /compiler/refimpl/lexer $input_file | /compiler/refimpl/parser | /compiler/refimpl/semant | $last_impl
        else 
            echoerr "[ERROR] Unknown compiler step: ${2}"
        fi fi fi fi
    fi
}

check_stage="^(lexer|parser|semant|cgen)$"
pipeline_pattern="^(my|ref)-(lexer|parser|semant|cgen)$"
exec_pattern="^(my|ref)$"
if [ "$1" = "test" ]; then
    if [[ $2 =~ $check_stage ]]; then
        run_test "ref" $2 $3
    else 
        echoerr "[ERROR]: Unknown compiler stage: ${2}"
    fi
else if [ "$1" = "mytest" ]; then
    if [[ $2 =~ $check_stage ]]; then
        run_test "my" $2 $3
    else 
        echoerr "[ERROR]: Unknown compiler stage: ${2}"
    fi
else if [[ $1 =~ $pipeline_pattern ]]; then
    run ${BASH_REMATCH[1]} ${BASH_REMATCH[2]} $2
else if [[ $1 =~ $exec_pattern ]]; then
    if [ ! -f "/compiler/${2}" ]; then
        echoerr "[ERROR] Input file does not exist: ${2}"
    else
        # run the file with the compiler
        input_file="/compiler/${2}"
    fi
    if [ "${BASH_REMATCH[1]}" = "ref" ]; then
        cd /compiler/refimpl 
        ./coolc-x86 -r $input_file
    else 
        echo "my"
    fi
else
    echoerr "[ERROR] Unknown argument format"
fi fi fi fi