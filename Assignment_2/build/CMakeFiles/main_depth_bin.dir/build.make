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
include CMakeFiles/main_depth_bin.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main_depth_bin.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main_depth_bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_depth_bin.dir/flags.make

CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj: CMakeFiles/main_depth_bin.dir/flags.make
CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj: CMakeFiles/main_depth_bin.dir/includes_CXX.rsp
CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj: ../extra/main_depth.cpp
CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj: CMakeFiles/main_depth_bin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\VSCWorkspace\base4\Assignment_2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj -MF CMakeFiles\main_depth_bin.dir\extra\main_depth.cpp.obj.d -o CMakeFiles\main_depth_bin.dir\extra\main_depth.cpp.obj -c D:\VSCWorkspace\base4\Assignment_2\extra\main_depth.cpp

CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\VSCWorkspace\base4\Assignment_2\extra\main_depth.cpp > CMakeFiles\main_depth_bin.dir\extra\main_depth.cpp.i

CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\VSCWorkspace\base4\Assignment_2\extra\main_depth.cpp -o CMakeFiles\main_depth_bin.dir\extra\main_depth.cpp.s

CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj: CMakeFiles/main_depth_bin.dir/flags.make
CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj: CMakeFiles/main_depth_bin.dir/includes_CXX.rsp
CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj: ../src/Helpers.cpp
CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj: CMakeFiles/main_depth_bin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\VSCWorkspace\base4\Assignment_2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj -MF CMakeFiles\main_depth_bin.dir\src\Helpers.cpp.obj.d -o CMakeFiles\main_depth_bin.dir\src\Helpers.cpp.obj -c D:\VSCWorkspace\base4\Assignment_2\src\Helpers.cpp

CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\VSCWorkspace\base4\Assignment_2\src\Helpers.cpp > CMakeFiles\main_depth_bin.dir\src\Helpers.cpp.i

CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\VSCWorkspace\base4\Assignment_2\src\Helpers.cpp -o CMakeFiles\main_depth_bin.dir\src\Helpers.cpp.s

# Object files for target main_depth_bin
main_depth_bin_OBJECTS = \
"CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj" \
"CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj"

# External object files for target main_depth_bin
main_depth_bin_EXTERNAL_OBJECTS =

main_depth_bin.exe: CMakeFiles/main_depth_bin.dir/extra/main_depth.cpp.obj
main_depth_bin.exe: CMakeFiles/main_depth_bin.dir/src/Helpers.cpp.obj
main_depth_bin.exe: CMakeFiles/main_depth_bin.dir/build.make
main_depth_bin.exe: libglfw3.a
main_depth_bin.exe: libglew.a
main_depth_bin.exe: CMakeFiles/main_depth_bin.dir/linklibs.rsp
main_depth_bin.exe: CMakeFiles/main_depth_bin.dir/objects1.rsp
main_depth_bin.exe: CMakeFiles/main_depth_bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\VSCWorkspace\base4\Assignment_2\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable main_depth_bin.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\main_depth_bin.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_depth_bin.dir/build: main_depth_bin.exe
.PHONY : CMakeFiles/main_depth_bin.dir/build

CMakeFiles/main_depth_bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\main_depth_bin.dir\cmake_clean.cmake
.PHONY : CMakeFiles/main_depth_bin.dir/clean

CMakeFiles/main_depth_bin.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\VSCWorkspace\base4\Assignment_2 D:\VSCWorkspace\base4\Assignment_2 D:\VSCWorkspace\base4\Assignment_2\build D:\VSCWorkspace\base4\Assignment_2\build D:\VSCWorkspace\base4\Assignment_2\build\CMakeFiles\main_depth_bin.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_depth_bin.dir/depend
