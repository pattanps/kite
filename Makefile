# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jlopes/tmp/kpm_transport/kpm_transport/Src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jlopes/tmp/kpm_transport/kpm_transport

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/local/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/local/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/jlopes/tmp/kpm_transport/kpm_transport/CMakeFiles /Users/jlopes/tmp/kpm_transport/kpm_transport/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/jlopes/tmp/kpm_transport/kpm_transport/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named KITEx

# Build rule for target.
KITEx: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 KITEx
.PHONY : KITEx

# fast build rule for target.
KITEx/fast:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/build
.PHONY : KITEx/fast

ComplexTraits.o: ComplexTraits.cpp.o

.PHONY : ComplexTraits.o

# target to build an object file
ComplexTraits.cpp.o:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/ComplexTraits.cpp.o
.PHONY : ComplexTraits.cpp.o

ComplexTraits.i: ComplexTraits.cpp.i

.PHONY : ComplexTraits.i

# target to preprocess a source file
ComplexTraits.cpp.i:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/ComplexTraits.cpp.i
.PHONY : ComplexTraits.cpp.i

ComplexTraits.s: ComplexTraits.cpp.s

.PHONY : ComplexTraits.s

# target to generate assembly for a file
ComplexTraits.cpp.s:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/ComplexTraits.cpp.s
.PHONY : ComplexTraits.cpp.s

Global.o: Global.cpp.o

.PHONY : Global.o

# target to build an object file
Global.cpp.o:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/Global.cpp.o
.PHONY : Global.cpp.o

Global.i: Global.cpp.i

.PHONY : Global.i

# target to preprocess a source file
Global.cpp.i:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/Global.cpp.i
.PHONY : Global.cpp.i

Global.s: Global.cpp.s

.PHONY : Global.s

# target to generate assembly for a file
Global.cpp.s:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/Global.cpp.s
.PHONY : Global.cpp.s

Random.o: Random.cpp.o

.PHONY : Random.o

# target to build an object file
Random.cpp.o:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/Random.cpp.o
.PHONY : Random.cpp.o

Random.i: Random.cpp.i

.PHONY : Random.i

# target to preprocess a source file
Random.cpp.i:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/Random.cpp.i
.PHONY : Random.cpp.i

Random.s: Random.cpp.s

.PHONY : Random.s

# target to generate assembly for a file
Random.cpp.s:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/Random.cpp.s
.PHONY : Random.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/main.cpp.s
.PHONY : main.cpp.s

myHDF5.o: myHDF5.cpp.o

.PHONY : myHDF5.o

# target to build an object file
myHDF5.cpp.o:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/myHDF5.cpp.o
.PHONY : myHDF5.cpp.o

myHDF5.i: myHDF5.cpp.i

.PHONY : myHDF5.i

# target to preprocess a source file
myHDF5.cpp.i:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/myHDF5.cpp.i
.PHONY : myHDF5.cpp.i

myHDF5.s: myHDF5.cpp.s

.PHONY : myHDF5.s

# target to generate assembly for a file
myHDF5.cpp.s:
	$(MAKE) -f CMakeFiles/KITEx.dir/build.make CMakeFiles/KITEx.dir/myHDF5.cpp.s
.PHONY : myHDF5.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... KITEx"
	@echo "... ComplexTraits.o"
	@echo "... ComplexTraits.i"
	@echo "... ComplexTraits.s"
	@echo "... Global.o"
	@echo "... Global.i"
	@echo "... Global.s"
	@echo "... Random.o"
	@echo "... Random.i"
	@echo "... Random.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... myHDF5.o"
	@echo "... myHDF5.i"
	@echo "... myHDF5.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

