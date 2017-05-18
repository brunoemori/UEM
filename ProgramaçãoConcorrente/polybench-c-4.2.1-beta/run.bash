#!/bin/bash

gcc -I utilities/ -I stencils/jacobi-2d/ utilities/polybench.c stencils/jacobi-2d/jacobi-2d.c -DPOLYBENCH_TIME -o jacobi2d-sequential
gcc -pthread -I utilities/ -I stencils/jacobi-2d/ utilities/polybench.c stencils/jacobi-2d/jacobi-2d-pthreads.c -DPOLYBENCH_TIME -o jacobi2d-pthread
gcc -fopenmp -I utilities/ -I stencils/jacobi-2d/ utilities/polybench.c stencils/jacobi-2d/jacobi-2d-openmp.c -DPOLYBENCH_TIME -o jacobi2d-openmp
echo
