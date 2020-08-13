#!/bin/bash

vim -u NORC $1 -c "vert diffsplit $2" -c "%!xxd" -c "wincmd l" -c "%!xxd"
