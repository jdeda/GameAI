#!/bin/sh
# Makes.
clear;
printf "\n%s\n" "Cleaning..."
make clean;

printf "\n%s\n" "Making..."
make;

printf "\n\n%s\n\n" "Running!"
./main