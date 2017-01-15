#!/bin/bash

compile_script_name="compile"
run_script_name="run"

declare -a solution_dirs=("csharp" "java" "clang")

for dir in "${solution_dirs[@]}"
do
    rm results-$dir.txt
done

mkdir solutions

cd referent_model
make clean &> /dev/null
make &> /dev/null
cd ..

for i in {1..6}
do
  echo "Generating $i. solution..."
  cgmemtime -t ./referent_model/is tests/test-$i.txt solutions/solution-$i.txt 2> /dev/null &>> results-orig.txt
done
 
echo "Solutions generated..."

for d in {2..4}
do
    for i in {1..6}
        do
            for dir in "${solution_dirs[@]}"
            do
                cd $dir
                cgmemtime -t make d=$d in=../tests/test-$i.txt out=../out.txt &>> \
                    ../results-$dir.txt
                cd ..
                cmp --silent out.txt solutions/solution-$i.txt > /dev/null
                verify=$?
                if [ $verify -eq 0 ]; then
                    echo "Verification of $dir for d=$d for test-$i successful!"
                else
                    echo "Verification of $dir for d=$d for test-$i failed!"
                fi
            done
        done
done

rm out.txt
rm -rf solutions
echo "Verification ended"
