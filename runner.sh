#!/bin/sh
# Builds and runs Makefile and main.
clear;
printf "%s\n" "Cleaning..."
make clean

printf "\n%s\n" "Making..."
make
clear

printf "%s\n\n" "Running!"
./main
