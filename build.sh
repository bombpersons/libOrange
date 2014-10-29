#!/bin/sh

rm -r release
mkdir release
cd release
cmake ../

make
