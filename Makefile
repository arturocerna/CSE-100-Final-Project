# Original by Sander
# Edited by Jonathan, Carlos
# A simple makefile for CSE 100 P3

# DIRECTORIES
#================================================================================
SRC=src
TEST=test
BUILD=build
OBJ=$(BUILD)/obj

# COMPILER OPTIONS
#================================================================================
CC=g++ 

# MMD: automatically resolve included files
# g: include debugging symbols in the generated object file
# Wall: enable all warnings
# std: language standard to compile for
CXXFLAGS=-MMD -g -Wall -Wextra -pedantic -std=c++11

# if passed "type=opt" at command-line, compile with "-O3" flag
# e.g. make pathfinder type=opt
#      make type=opt
ifeq ($(type),opt)
	CXXFLAGS += -O3
endif

# g: include debugging symbols in the generated executable
LDFLAGS=-g
LDFLAGS_NO_DEBUG=

# BUILD TARGETS
#==============================================================================
default: all 

all: clean pathfinder awardsceremony linkpredictor

# ATTN: The following are the _REQUIRED_ targets. You may rewrite this Makefile
# from scratch, but yours MUST export the following targets:

# REQUIRED target for Checkpoint AND Final Submission
#------------------------------------------------------------------------------
pathfinder: actor_graph src_object.pathfinder src_object.ActorVector src_object.Movie src_object.ActorVectorS src_object.MovieS
	$(call make_executable_from_object_file,pathfinder)

# REQUIRED targets for Final Submission only
# -----------------------------------------------------------------------------
#  Uncomment and set them up as needed.
#
awardsceremony: actor_graph src_object.awardsceremony src_object.ActorVector src_object.Movie src_object.ActorVectorS src_object.MovieS
	$(call make_executable_from_object_file,awardsceremony)
linkpredictor: actor_graph src_object.linkpredictor src_object.ActorVector src_object.Movie src_object.ActorVectorS src_object.MovieS
	$(call make_executable_from_object_file,linkpredictor)


# In our solution we used the following build config for all required targets.
# Feel free to change this as you see fit.
actor_graph: init src_object.ActorGraph

# Testing targets.
#-----------------------------------------------------------------------------
#   By this point you should have already learned how valuable writing
#     your own tests is. For this PA we do not provide you with any initial
#     tests: but you are strongly encouraged to do so. 
#  
#   The targets below are boilerplate for test files you should write.
#     Uncomment the following lines and add any additional necessary 
#     dependencies, like you did in PA2.
#
# test_pathfinder: init test_object.test_pathfinder
# 	$(call make_executable_from_object_file_and_test,$(TEST)/test_pathfinder)
# test_awardsceremony: init test_object.test_awardsceremony
# 	$(call make_executable_from_object_file_and_test,$(TEST)/test_awardsceremony)
# test_linkpredictor: init test_object.test_linkpredictor
# 	$(call make_executable_from_object_file_and_test,$(TEST)/test_linkpredictor)

# Basic setup
#-----------------------------------------------------------------------------
clean:
	rm -rf $(BUILD)

init:
	mkdir -p $(BUILD)/$(TEST)
	mkdir -p $(OBJ)/$(SRC)
	mkdir -p $(OBJ)/$(TEST)

# Utils
#-----------------------------------------------------------------------------
src_object.%:
	# if build target is src_object.xyz then $@ == src_object.xyz, we want to extract filename xyz.
	$(call make_object_file_from_cpp_file,$(SRC)/$(call get_filename_from_object_target,$@))

test_object.%:
	$(call make_object_file_from_cpp_file,$(TEST)/$(call get_filename_from_object_target,$@))

# === Utility functions ===
make_object_file_from_cpp_file=$(CC) $(CXXFLAGS) -c $1.cpp -o $(OBJ)/$1.o

make_executable_from_object_file=$(shell $(CC) $(LDFLAGS) -o $(BUILD)/$1 $(wildcard $(OBJ)/$(SRC)/*.o))
make_executable_from_object_file_and_test=$(shell $(CC) $(LDFLAGS) -o $(BUILD)/$1 $(wildcard $(OBJ)/$(SRC)/*.o) $(wildcard $(OBJ)/$(TEST)/*.o))
make_executable_from_object_file_no_debug=$(shell $(CC) $(LDFLAGS_NO_DEBUG) -o $(BUILD)/$1 $(wildcard $(OBJ)/$(SRC)/*.o))

get_filename_from_object_target=$(shell echo $1 | cut -d'.' -f2)
