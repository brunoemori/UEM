#!/bin/bash

gcc -pthread -O3 -I utilities/ -I stencils/jacobi-2d/ utilities//polybench.c stencils/jacobi-2d/jacobi-2d-pthreads.c -DPOLYBENCH_TIME
echo
