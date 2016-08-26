#!/bin/bash
if [[ $# -eq 1 ]]; then 
./featur $1; 
./res theta.txt feature.txt; 
fi

