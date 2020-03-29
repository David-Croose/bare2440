#!/bin/bash

find . -type f -name *.o   -exec rm -f {} \;
find . -type f -name *.bin -exec rm -f {} \;
find . -type f -name *.dis -exec rm -f {} \;
find . -type f -name *.elf -exec rm -f {} \;
