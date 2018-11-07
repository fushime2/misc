#!/bin/bash
gcc -std=c11 -Wall -o lang lang.c

runtest() {
    output=$(./lang "$1")
    if [ "$output" != "$2" ]; then
        echo "$1: $2 expected, but got $output"
        exit 1
    fi
    echo "$1 => $output"
}

echo "=== basic ==="
runtest 0 0
runtest 1 1
runtest 99 99
runtest '1 2 3' '1
2
3'

echo "=== op ==="
runtest "+ 0 0" 0
runtest "+ 100 2" 102 
runtest "- 11 1" 10
runtest "- 1 11" -10
runtest "* 10 10" 100
runtest "/ 10 5" 2
runtest "+ + + 1 2 3 4" 10
runtest "+ 2 * 4 3" 14

echo "=== functions ==="
runtest 'F[+ a a] F(1)' 2
runtest 'F[* a 2] F(5)' 10
runtest 'F[* a a] F(F(2))' 16
runtest 'F[* a a] F(F(F(2)))' 256
runtest 'F[* a b] F(3 5)' 15


echo "=== functions ==="
runtest 'P(5)' '5
5'

echo "=== fibonacchi ==="
./lang 'F[P(a) F(b + a b)] F(1 1)' | head -20


echo ok
