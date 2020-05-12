alias coolc-build='docker build --tag coolc .'
alias coolc-run='docker run --rm -v "$PWD/tests":/compiler/tests -a stdin -a stdout -a stderr -it coolc'
alias coolc-test='docker run --rm -v "$PWD/tests":/compiler/tests -a stdin -a stdout -a stderr -it coolc test'
alias coolc-mytest='docker run --rm -v "$PWD/tests":/compiler/tests -a stdin -a stdout -a stderr -it coolc mytest'