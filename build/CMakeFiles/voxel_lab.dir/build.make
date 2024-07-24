# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_SOURCE_DIR = /home/silva/projects/voxel_lab/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/silva/projects/voxel_lab/build

# Include any dependencies generated for this target.
include CMakeFiles/voxel_lab.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/voxel_lab.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/voxel_lab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/voxel_lab.dir/flags.make

CMakeFiles/voxel_lab.dir/main.cpp.o: CMakeFiles/voxel_lab.dir/flags.make
CMakeFiles/voxel_lab.dir/main.cpp.o: /home/silva/projects/voxel_lab/src/main.cpp
CMakeFiles/voxel_lab.dir/main.cpp.o: CMakeFiles/voxel_lab.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/silva/projects/voxel_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/voxel_lab.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/voxel_lab.dir/main.cpp.o -MF CMakeFiles/voxel_lab.dir/main.cpp.o.d -o CMakeFiles/voxel_lab.dir/main.cpp.o -c /home/silva/projects/voxel_lab/src/main.cpp

CMakeFiles/voxel_lab.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/voxel_lab.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/silva/projects/voxel_lab/src/main.cpp > CMakeFiles/voxel_lab.dir/main.cpp.i

CMakeFiles/voxel_lab.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/voxel_lab.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/silva/projects/voxel_lab/src/main.cpp -o CMakeFiles/voxel_lab.dir/main.cpp.s

# Object files for target voxel_lab
voxel_lab_OBJECTS = \
"CMakeFiles/voxel_lab.dir/main.cpp.o"

# External object files for target voxel_lab
voxel_lab_EXTERNAL_OBJECTS =

voxel_lab: CMakeFiles/voxel_lab.dir/main.cpp.o
voxel_lab: CMakeFiles/voxel_lab.dir/build.make
voxel_lab: libterrain.so
voxel_lab: libchunk.so
voxel_lab: libshader.so
voxel_lab: libimage.so
voxel_lab: libglad.so
voxel_lab: /usr/lib/libglfw.so.3.4
voxel_lab: /usr/lib/libGLX.so
voxel_lab: /usr/lib/libOpenGL.so
voxel_lab: CMakeFiles/voxel_lab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/silva/projects/voxel_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable voxel_lab"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/voxel_lab.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy_if_different /usr/x86_64-w64-mingw32/lib/libglfw3.a /home/silva/projects/voxel_lab/build
	/usr/bin/cmake -E copy_if_different /usr/lib/libGL.so /home/silva/projects/voxel_lab/build

# Rule to build all files generated by this target.
CMakeFiles/voxel_lab.dir/build: voxel_lab
.PHONY : CMakeFiles/voxel_lab.dir/build

CMakeFiles/voxel_lab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/voxel_lab.dir/cmake_clean.cmake
.PHONY : CMakeFiles/voxel_lab.dir/clean

CMakeFiles/voxel_lab.dir/depend:
	cd /home/silva/projects/voxel_lab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/silva/projects/voxel_lab/src /home/silva/projects/voxel_lab/src /home/silva/projects/voxel_lab/build /home/silva/projects/voxel_lab/build /home/silva/projects/voxel_lab/build/CMakeFiles/voxel_lab.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/voxel_lab.dir/depend

