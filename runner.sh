#!/bin/sh
# Builds and runs Makefile and main.
clear;
$printf "%s\n" "Cleaning..."
make clean

$printf "%s\n" "Making..."
make
clear

$printf "%s\n" "Running!"
./main
