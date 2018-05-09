#!/bin/bash

# build dhbwcc

./build.sh
cd testing/tests
./test.sh
cd ../..
