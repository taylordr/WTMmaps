# WTMmaps
This folder contains c++ and Matlab scripts that implement WTM maps for a given network and a given WTM threshold T. Please cite the following paper for any usage of this code:

D. Taylor, F. Klimm, H. A. Harrington, M. Kramar, K. Mischaikow, M. A. Porter, and P. J. Mucha (2015) "Topological data analysis of contagion maps for examining spreading processes on networks", Nature Communications 6, 7723 
http://www.nature.com/articles/ncomms8723

The following scripts are included:

1. compute_WTM_map_v3.cpp - the actual code that computes the WTM map
2. Makefile - an example Makefile to compile compute_WTM_map_v3.cpp on your computer (Note that compute_WTM_map_v3.m also writes a Makefile and compiles compute_WTM_map_v3.cpp... that is, if you want it too.)
3. compute_WTM_map.m - a Matlab wrapper to interface with compute_WTM_map_v3.cpp
4. Example1.m - an example experiment in Matlab that demos how to use compute_WTM_map.m
5. LinkList.tsv  - an edge list for a ring lattice, which is used by Example1.m

If you have any questions about the code, please email Dane Taylor at danet@buffalo.edu
