# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build

# Include any dependencies generated for this target.
include CMakeFiles/gxtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gxtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gxtest.dir/flags.make

CMakeFiles/gxtest.dir/src/main.cc.o: CMakeFiles/gxtest.dir/flags.make
CMakeFiles/gxtest.dir/src/main.cc.o: ../src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gxtest.dir/src/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gxtest.dir/src/main.cc.o -c /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/main.cc

CMakeFiles/gxtest.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gxtest.dir/src/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/main.cc > CMakeFiles/gxtest.dir/src/main.cc.i

CMakeFiles/gxtest.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gxtest.dir/src/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/main.cc -o CMakeFiles/gxtest.dir/src/main.cc.s

CMakeFiles/gxtest.dir/src/gXThreadPool.cc.o: CMakeFiles/gxtest.dir/flags.make
CMakeFiles/gxtest.dir/src/gXThreadPool.cc.o: ../src/gXThreadPool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gxtest.dir/src/gXThreadPool.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gxtest.dir/src/gXThreadPool.cc.o -c /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/gXThreadPool.cc

CMakeFiles/gxtest.dir/src/gXThreadPool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gxtest.dir/src/gXThreadPool.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/gXThreadPool.cc > CMakeFiles/gxtest.dir/src/gXThreadPool.cc.i

CMakeFiles/gxtest.dir/src/gXThreadPool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gxtest.dir/src/gXThreadPool.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/gXThreadPool.cc -o CMakeFiles/gxtest.dir/src/gXThreadPool.cc.s

CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.o: CMakeFiles/gxtest.dir/flags.make
CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.o: ../src/gXDataTransmissionServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.o -c /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/gXDataTransmissionServer.cc

CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/gXDataTransmissionServer.cc > CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.i

CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/src/gXDataTransmissionServer.cc -o CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.s

# Object files for target gxtest
gxtest_OBJECTS = \
"CMakeFiles/gxtest.dir/src/main.cc.o" \
"CMakeFiles/gxtest.dir/src/gXThreadPool.cc.o" \
"CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.o"

# External object files for target gxtest
gxtest_EXTERNAL_OBJECTS =

gxtest: CMakeFiles/gxtest.dir/src/main.cc.o
gxtest: CMakeFiles/gxtest.dir/src/gXThreadPool.cc.o
gxtest: CMakeFiles/gxtest.dir/src/gXDataTransmissionServer.cc.o
gxtest: CMakeFiles/gxtest.dir/build.make
gxtest: CMakeFiles/gxtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable gxtest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gxtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gxtest.dir/build: gxtest

.PHONY : CMakeFiles/gxtest.dir/build

CMakeFiles/gxtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gxtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gxtest.dir/clean

CMakeFiles/gxtest.dir/depend:
	cd /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build /home/jcjuarez/GanymedeXpediaDataTransmissionProtocol/build/CMakeFiles/gxtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gxtest.dir/depend

