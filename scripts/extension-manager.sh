#!/bin/bash
declare -a s
s=($input_var)
echo ${s[@]//.gpg/}
