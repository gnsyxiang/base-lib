# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/uos/test/skills/base-lib/src/d-bus/test/demo5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build

# Include any dependencies generated for this target.
include CMakeFiles/ExampleClientlib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ExampleClientlib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ExampleClientlib.dir/flags.make

CMakeFiles/ExampleClientlib.dir/libClient.c.o: CMakeFiles/ExampleClientlib.dir/flags.make
CMakeFiles/ExampleClientlib.dir/libClient.c.o: ../libClient.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ExampleClientlib.dir/libClient.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ExampleClientlib.dir/libClient.c.o   -c /home/uos/test/skills/base-lib/src/d-bus/test/demo5/libClient.c

CMakeFiles/ExampleClientlib.dir/libClient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ExampleClientlib.dir/libClient.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/uos/test/skills/base-lib/src/d-bus/test/demo5/libClient.c > CMakeFiles/ExampleClientlib.dir/libClient.c.i

CMakeFiles/ExampleClientlib.dir/libClient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ExampleClientlib.dir/libClient.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/uos/test/skills/base-lib/src/d-bus/test/demo5/libClient.c -o CMakeFiles/ExampleClientlib.dir/libClient.c.s

CMakeFiles/ExampleClientlib.dir/libClient.c.o.requires:

.PHONY : CMakeFiles/ExampleClientlib.dir/libClient.c.o.requires

CMakeFiles/ExampleClientlib.dir/libClient.c.o.provides: CMakeFiles/ExampleClientlib.dir/libClient.c.o.requires
	$(MAKE) -f CMakeFiles/ExampleClientlib.dir/build.make CMakeFiles/ExampleClientlib.dir/libClient.c.o.provides.build
.PHONY : CMakeFiles/ExampleClientlib.dir/libClient.c.o.provides

CMakeFiles/ExampleClientlib.dir/libClient.c.o.provides.build: CMakeFiles/ExampleClientlib.dir/libClient.c.o


CMakeFiles/ExampleClientlib.dir/Example-generated.c.o: CMakeFiles/ExampleClientlib.dir/flags.make
CMakeFiles/ExampleClientlib.dir/Example-generated.c.o: Example-generated.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ExampleClientlib.dir/Example-generated.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ExampleClientlib.dir/Example-generated.c.o   -c /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/Example-generated.c

CMakeFiles/ExampleClientlib.dir/Example-generated.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ExampleClientlib.dir/Example-generated.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/Example-generated.c > CMakeFiles/ExampleClientlib.dir/Example-generated.c.i

CMakeFiles/ExampleClientlib.dir/Example-generated.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ExampleClientlib.dir/Example-generated.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/Example-generated.c -o CMakeFiles/ExampleClientlib.dir/Example-generated.c.s

CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.requires:

.PHONY : CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.requires

CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.provides: CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.requires
	$(MAKE) -f CMakeFiles/ExampleClientlib.dir/build.make CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.provides.build
.PHONY : CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.provides

CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.provides.build: CMakeFiles/ExampleClientlib.dir/Example-generated.c.o


# Object files for target ExampleClientlib
ExampleClientlib_OBJECTS = \
"CMakeFiles/ExampleClientlib.dir/libClient.c.o" \
"CMakeFiles/ExampleClientlib.dir/Example-generated.c.o"

# External object files for target ExampleClientlib
ExampleClientlib_EXTERNAL_OBJECTS =

libExampleClientlib.so: CMakeFiles/ExampleClientlib.dir/libClient.c.o
libExampleClientlib.so: CMakeFiles/ExampleClientlib.dir/Example-generated.c.o
libExampleClientlib.so: CMakeFiles/ExampleClientlib.dir/build.make
libExampleClientlib.so: CMakeFiles/ExampleClientlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared library libExampleClientlib.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ExampleClientlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ExampleClientlib.dir/build: libExampleClientlib.so

.PHONY : CMakeFiles/ExampleClientlib.dir/build

CMakeFiles/ExampleClientlib.dir/requires: CMakeFiles/ExampleClientlib.dir/libClient.c.o.requires
CMakeFiles/ExampleClientlib.dir/requires: CMakeFiles/ExampleClientlib.dir/Example-generated.c.o.requires

.PHONY : CMakeFiles/ExampleClientlib.dir/requires

CMakeFiles/ExampleClientlib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ExampleClientlib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ExampleClientlib.dir/clean

CMakeFiles/ExampleClientlib.dir/depend:
	cd /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/uos/test/skills/base-lib/src/d-bus/test/demo5 /home/uos/test/skills/base-lib/src/d-bus/test/demo5 /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build /home/uos/test/skills/base-lib/src/d-bus/test/demo5/build/CMakeFiles/ExampleClientlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ExampleClientlib.dir/depend
