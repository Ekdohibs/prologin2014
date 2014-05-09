# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.cc max_flow.cc chemin.cc common.cc
champion-dists = max_flow.hh chemin.hh common.hh
champion-cxxflags = -O3 -ggdb3 -Wall -Wextra -std=c++11 -pedantic

# Evite de toucher a ce qui suit
champion-dists += prologin.hh
include ../includes/rules.mk
