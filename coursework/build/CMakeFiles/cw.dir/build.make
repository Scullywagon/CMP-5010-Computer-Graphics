# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/logan/University/CMP-5010-Computer-Graphics/coursework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/logan/University/CMP-5010-Computer-Graphics/coursework/build

# Include any dependencies generated for this target.
include CMakeFiles/cw.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cw.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cw.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cw.dir/flags.make

CMakeFiles/cw.dir/codegen:
.PHONY : CMakeFiles/cw.dir/codegen

CMakeFiles/cw.dir/src/Scene.cpp.o: CMakeFiles/cw.dir/flags.make
CMakeFiles/cw.dir/src/Scene.cpp.o: /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/Scene.cpp
CMakeFiles/cw.dir/src/Scene.cpp.o: CMakeFiles/cw.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/logan/University/CMP-5010-Computer-Graphics/coursework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cw.dir/src/Scene.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cw.dir/src/Scene.cpp.o -MF CMakeFiles/cw.dir/src/Scene.cpp.o.d -o CMakeFiles/cw.dir/src/Scene.cpp.o -c /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/Scene.cpp

CMakeFiles/cw.dir/src/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cw.dir/src/Scene.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/Scene.cpp > CMakeFiles/cw.dir/src/Scene.cpp.i

CMakeFiles/cw.dir/src/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cw.dir/src/Scene.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/Scene.cpp -o CMakeFiles/cw.dir/src/Scene.cpp.s

CMakeFiles/cw.dir/src/main.cpp.o: CMakeFiles/cw.dir/flags.make
CMakeFiles/cw.dir/src/main.cpp.o: /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/main.cpp
CMakeFiles/cw.dir/src/main.cpp.o: CMakeFiles/cw.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/logan/University/CMP-5010-Computer-Graphics/coursework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/cw.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cw.dir/src/main.cpp.o -MF CMakeFiles/cw.dir/src/main.cpp.o.d -o CMakeFiles/cw.dir/src/main.cpp.o -c /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/main.cpp

CMakeFiles/cw.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cw.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/main.cpp > CMakeFiles/cw.dir/src/main.cpp.i

CMakeFiles/cw.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cw.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/logan/University/CMP-5010-Computer-Graphics/coursework/src/main.cpp -o CMakeFiles/cw.dir/src/main.cpp.s

# Object files for target cw
cw_OBJECTS = \
"CMakeFiles/cw.dir/src/Scene.cpp.o" \
"CMakeFiles/cw.dir/src/main.cpp.o"

# External object files for target cw
cw_EXTERNAL_OBJECTS =

/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: CMakeFiles/cw.dir/src/Scene.cpp.o
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: CMakeFiles/cw.dir/src/main.cpp.o
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: CMakeFiles/cw.dir/build.make
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: /Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/System/Library/Frameworks/OpenGL.framework
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: /usr/local/lib/libGLEW.2.2.0.dylib
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: /usr/local/lib/libglfw.3.4.dylib
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: /usr/local/lib/libassimp.5.4.3.dylib
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: /Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk/usr/lib/libz.tbd
/Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw: CMakeFiles/cw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/logan/University/CMP-5010-Computer-Graphics/coursework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cw.dir/build: /Users/logan/University/CMP-5010-Computer-Graphics/coursework/output/cw
.PHONY : CMakeFiles/cw.dir/build

CMakeFiles/cw.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cw.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cw.dir/clean

CMakeFiles/cw.dir/depend:
	cd /Users/logan/University/CMP-5010-Computer-Graphics/coursework/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/logan/University/CMP-5010-Computer-Graphics/coursework /Users/logan/University/CMP-5010-Computer-Graphics/coursework /Users/logan/University/CMP-5010-Computer-Graphics/coursework/build /Users/logan/University/CMP-5010-Computer-Graphics/coursework/build /Users/logan/University/CMP-5010-Computer-Graphics/coursework/build/CMakeFiles/cw.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cw.dir/depend

