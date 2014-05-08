# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.cc max_flow.cc chemin.cc common.cc
champion-dists =
champion-cxxflags = -O3 -ggdb3 -Wall -std=c++11

# Evite de toucher a ce qui suit
champion-dists += prologin.hh
include ../includes/rules.mk
