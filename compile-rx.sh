#!/bin/bash

gcc funcs.c receive_serial.c -o Rx

./Rx foo.txt /dev/cu.usbmodem142431
