# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build

# Include any dependencies generated for this target.
include path/CMakeFiles/pathlib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include path/CMakeFiles/pathlib.dir/compiler_depend.make

# Include the progress variables for this target.
include path/CMakeFiles/pathlib.dir/progress.make

# Include the compile flags for this target's objects.
include path/CMakeFiles/pathlib.dir/flags.make

path/CMakeFiles/pathlib.dir/src/pathtest.cpp.o: path/CMakeFiles/pathlib.dir/flags.make
path/CMakeFiles/pathlib.dir/src/pathtest.cpp.o: ../path/src/pathtest.cpp
path/CMakeFiles/pathlib.dir/src/pathtest.cpp.o: path/CMakeFiles/pathlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object path/CMakeFiles/pathlib.dir/src/pathtest.cpp.o"
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT path/CMakeFiles/pathlib.dir/src/pathtest.cpp.o -MF CMakeFiles/pathlib.dir/src/pathtest.cpp.o.d -o CMakeFiles/pathlib.dir/src/pathtest.cpp.o -c /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/path/src/pathtest.cpp

path/CMakeFiles/pathlib.dir/src/pathtest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pathlib.dir/src/pathtest.cpp.i"
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/path/src/pathtest.cpp > CMakeFiles/pathlib.dir/src/pathtest.cpp.i

path/CMakeFiles/pathlib.dir/src/pathtest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pathlib.dir/src/pathtest.cpp.s"
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/path/src/pathtest.cpp -o CMakeFiles/pathlib.dir/src/pathtest.cpp.s

# Object files for target pathlib
pathlib_OBJECTS = \
"CMakeFiles/pathlib.dir/src/pathtest.cpp.o"

# External object files for target pathlib
pathlib_EXTERNAL_OBJECTS =

path/libpathlib.a: path/CMakeFiles/pathlib.dir/src/pathtest.cpp.o
path/libpathlib.a: path/CMakeFiles/pathlib.dir/build.make
path/libpathlib.a: path/CMakeFiles/pathlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libpathlib.a"
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path && $(CMAKE_COMMAND) -P CMakeFiles/pathlib.dir/cmake_clean_target.cmake
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pathlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
path/CMakeFiles/pathlib.dir/build: path/libpathlib.a
.PHONY : path/CMakeFiles/pathlib.dir/build

path/CMakeFiles/pathlib.dir/clean:
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path && $(CMAKE_COMMAND) -P CMakeFiles/pathlib.dir/cmake_clean.cmake
.PHONY : path/CMakeFiles/pathlib.dir/clean

path/CMakeFiles/pathlib.dir/depend:
	cd /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/path /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path /home/david/dhbw/programmieren1/c++/Programmentwurf/jsoncpp/build/path/CMakeFiles/pathlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : path/CMakeFiles/pathlib.dir/depend
