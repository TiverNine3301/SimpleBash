#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

declare -a tests=(
"-e 'pattern' ./test_files/*.txt"
"-i 'pattern' ./test_files/*.txt"
"-v 'pattern' ./test_files/*.txt"
"-c 'pattern' ./test_files/*.txt"
"-l 'pattern' ./test_files/*.txt"
"-n 'pattern' ./test_files/*.txt"
"-h 'pattern' ./test_files/*.txt"
"-s 'pattern' ./test_files/*.txt"
"-f ./test_files/keywords.txt ./test_files/fruits.txt"
"-o 'pattern' ./test_files/*.txt"
"-iv ./test_files/*.txt"
"-in apple ./test_files/*.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    leaks -quiet -atExit -- ./s21_grep $t > test_s21_grep.log
    leak=$(grep -A100000 leaks test_s21_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]
    then
      (( SUCCESS++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
#        echo "$leak"
    fi
    rm test_s21_grep.log
}

for test_case in "${tests[@]}"
do
    testing $test_case
done

echo "Total tests: $COUNTER"
echo "Success: $SUCCESS"
echo "Fail: $FAIL"