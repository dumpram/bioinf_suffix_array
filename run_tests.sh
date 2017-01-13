#!/bin/bash

compile_script_name="compile"
run_script_name="run"

declare -a solution_dirs=("csharp" "java")

for dir in "${solution_dirs[@]}"
do
    rm results-$dir.txt
done

for d in {2..10}
do
    for i in {1..10}
        do
            for dir in "${solution_dirs[@]}"
            do
                cd $dir
                make d=$d in=../tests/test-$i.txt out=../out.txt >> \
                    ../results-$dir.txt
                cd ..
                diff out.txt solutions/solution-$i.txt
                verify=$?
                if [ $verify -eq 0 ]; then
                    echo "Verification of $dir for d=$d for test-$i successful!"
                else
                    echo "Verification of $dir for d=$d for test-$i failed!"
                fi
            done
        done
done
