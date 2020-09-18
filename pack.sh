#!/bin/bash

LibDir=$PWD"/mylib"
Target=$1

lib_array=($(ldd $Target | grep -o "/.*" | grep -o "/.*/[^[:space:]]*"))

$(mkdir $LibDir)

for Variable in ${lib_array[@]}
do
    cp "$Variable" $LibDir
done
