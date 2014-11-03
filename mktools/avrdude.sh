#!/bin/bash

avrdude -cavr910 -patmega32 -P/dev/ttyUSB0 -b115200 $*
