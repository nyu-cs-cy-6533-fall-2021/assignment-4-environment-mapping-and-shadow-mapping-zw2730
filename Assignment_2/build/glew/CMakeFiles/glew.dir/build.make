# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\cmake-3.21.3-windows-x86_64\bin\cmake.exe

# The command to remove a file.
RM = D:\cmake-3.21.3-windows-x86_64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\VSCWorkspace\base4\Assignment_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\VSCWorkspace\base4\Assignment_2\build

# Include any dependencies generated for this target.
include glew/CMakeFiles/glew.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include glew/CMakeFiles/glew.dir/compiler_depend.make

# Include the progress variables for this target.
include glew/CMakeFiles/glew.dir/progress.make

# Include the compile flags for this target's objects.
include glew/CMakeFiles/glew.dir/flags.make

glew/CMakeFiles/glew.dir/src/glew.c.obj: glew/CMakeFiles/glew.dir/flags.make
glew/CMakeFiles/glew.dir/src/glew.c.obj: glew/CMakeFiles/glew.dir/includes_C.rsp
glew/CMakeFiles/glew.dir/src/glew.c.obj: D:/VSCWorkspace/base4/ext/glew/src/glew.c
glew/CMakeFiles/glew.dir/src/glew.c.obj: glew/CMakeFiles/glew.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\VSCWorkspace\base4\Assignment_2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glew/CMakeFiles/glew.dir/src/glew.c.obj"
	cd /d D:\VSCWorkspace\base4\Assignment_2\build\glew && D:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT glew/CMakeFiles/glew.dir/src/glew.c.obj -MF CMakeFiles\glew.dir\src\glew.c.obj.d -o CMakeFiles\glew.dir\src\glew.c.obj -c D:\VSCWorkspace\base4\ext\glew\src\glew.c

glew/CMakeFiles/glew.dir/src/glew.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glew.dir/src/glew.c.i"
	cd /d D:\VSCWorkspace\base4\Assignment_2\build\glew && D:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\VSCWorkspace\base4\ext\glew\src\glew.c > CMakeFiles\glew.dir\src\glew.c.i

glew/CMakeFiles/glew.dir/src/glew.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glew.dir/src/glew.c.s"
	cd /d D:\VSCWorkspace\base4\Assignment_2\build\glew && D:\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\VSCWorkspace\base4\ext\glew\src\glew.c -o CMakeFiles\glew.dir\src\glew.c.s

# Object files for target glew
glew_OBJECTS = \
"CMakeFiles/glew.dir/src/glew.c.obj"

# External object files for target glew
glew_EXTERNAL_OBJECTS =

libglew.a: glew/CMakeFiles/glew.dir/src/glew.c.obj
libglew.a: glew/CMakeFiles/glew.dir/build.make
libglew.a: glew/CMakeFiles/glew.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\VSCWorkspace\base4\Assignment_2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ..\libglew.a"
	cd /d D:\VSCWorkspace\base4\Assignment_2\build\glew && $(CMAKE_COMMAND) -P CMakeFiles\glew.dir\cmake_clean_target.cmake
	cd /d D:\VSCWorkspace\base4\Assignment_2\build\glew && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\glew.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glew/CMakeFiles/glew.dir/build: libglew.a
.PHONY : glew/CMakeFiles/glew.dir/build

glew/CMakeFiles/glew.dir/clean:
	cd /d D:\VSCWorkspace\base4\Assignment_2\build\glew && $(CMAKE_COMMAND) -P CMakeFiles\glew.dir\cmake_clean.cmake
.PHONY : glew/CMakeFiles/glew.dir/clean

glew/CMakeFiles/glew.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\VSCWorkspace\base4\Assignment_2 D:\VSCWorkspace\base4\ext\glew D:\VSCWorkspace\base4\Assignment_2\build D:\VSCWorkspace\base4\Assignment_2\build\glew D:\VSCWorkspace\base4\Assignment_2\build\glew\CMakeFiles\glew.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : glew/CMakeFiles/glew.dir/depend

