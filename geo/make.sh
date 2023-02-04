#!/bin/bash

gcc ./*.c -o run -O1

./run && ./run > geo.log