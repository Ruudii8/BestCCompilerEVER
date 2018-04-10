#!/bin/bash

# build dhbwcc

rm -R bin
mkdir -p bin && cd bin && cmake .. && make
