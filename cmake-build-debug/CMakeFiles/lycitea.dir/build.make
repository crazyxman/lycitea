# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wangjinxi/dockerWorkspace/lycitea_new

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lycitea.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lycitea.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lycitea.dir/flags.make

CMakeFiles/lycitea.dir/lycitea.c.o: CMakeFiles/lycitea.dir/flags.make
CMakeFiles/lycitea.dir/lycitea.c.o: ../lycitea.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lycitea.dir/lycitea.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lycitea.dir/lycitea.c.o   -c /Users/wangjinxi/dockerWorkspace/lycitea_new/lycitea.c

CMakeFiles/lycitea.dir/lycitea.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lycitea.dir/lycitea.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/wangjinxi/dockerWorkspace/lycitea_new/lycitea.c > CMakeFiles/lycitea.dir/lycitea.c.i

CMakeFiles/lycitea.dir/lycitea.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lycitea.dir/lycitea.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/wangjinxi/dockerWorkspace/lycitea_new/lycitea.c -o CMakeFiles/lycitea.dir/lycitea.c.s

CMakeFiles/lycitea.dir/lycitea_exception.c.o: CMakeFiles/lycitea.dir/flags.make
CMakeFiles/lycitea.dir/lycitea_exception.c.o: ../lycitea_exception.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lycitea.dir/lycitea_exception.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lycitea.dir/lycitea_exception.c.o   -c /Users/wangjinxi/dockerWorkspace/lycitea_new/lycitea_exception.c

CMakeFiles/lycitea.dir/lycitea_exception.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lycitea.dir/lycitea_exception.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/wangjinxi/dockerWorkspace/lycitea_new/lycitea_exception.c > CMakeFiles/lycitea.dir/lycitea_exception.c.i

CMakeFiles/lycitea.dir/lycitea_exception.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lycitea.dir/lycitea_exception.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/wangjinxi/dockerWorkspace/lycitea_new/lycitea_exception.c -o CMakeFiles/lycitea.dir/lycitea_exception.c.s

CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.o: CMakeFiles/lycitea.dir/flags.make
CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.o: ../helpers/lycitea_helpers_common.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.o   -c /Users/wangjinxi/dockerWorkspace/lycitea_new/helpers/lycitea_helpers_common.c

CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/wangjinxi/dockerWorkspace/lycitea_new/helpers/lycitea_helpers_common.c > CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.i

CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/wangjinxi/dockerWorkspace/lycitea_new/helpers/lycitea_helpers_common.c -o CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.s

CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.o: CMakeFiles/lycitea.dir/flags.make
CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.o: ../helpers/lycitea_helpers_lru.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.o   -c /Users/wangjinxi/dockerWorkspace/lycitea_new/helpers/lycitea_helpers_lru.c

CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/wangjinxi/dockerWorkspace/lycitea_new/helpers/lycitea_helpers_lru.c > CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.i

CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/wangjinxi/dockerWorkspace/lycitea_new/helpers/lycitea_helpers_lru.c -o CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.s

CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.o: CMakeFiles/lycitea.dir/flags.make
CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.o: ../caches/lycitea_cache_lru.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.o   -c /Users/wangjinxi/dockerWorkspace/lycitea_new/caches/lycitea_cache_lru.c

CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/wangjinxi/dockerWorkspace/lycitea_new/caches/lycitea_cache_lru.c > CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.i

CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/wangjinxi/dockerWorkspace/lycitea_new/caches/lycitea_cache_lru.c -o CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.s

# Object files for target lycitea
lycitea_OBJECTS = \
"CMakeFiles/lycitea.dir/lycitea.c.o" \
"CMakeFiles/lycitea.dir/lycitea_exception.c.o" \
"CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.o" \
"CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.o" \
"CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.o"

# External object files for target lycitea
lycitea_EXTERNAL_OBJECTS =

lycitea: CMakeFiles/lycitea.dir/lycitea.c.o
lycitea: CMakeFiles/lycitea.dir/lycitea_exception.c.o
lycitea: CMakeFiles/lycitea.dir/helpers/lycitea_helpers_common.c.o
lycitea: CMakeFiles/lycitea.dir/helpers/lycitea_helpers_lru.c.o
lycitea: CMakeFiles/lycitea.dir/caches/lycitea_cache_lru.c.o
lycitea: CMakeFiles/lycitea.dir/build.make
lycitea: CMakeFiles/lycitea.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable lycitea"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lycitea.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lycitea.dir/build: lycitea

.PHONY : CMakeFiles/lycitea.dir/build

CMakeFiles/lycitea.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lycitea.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lycitea.dir/clean

CMakeFiles/lycitea.dir/depend:
	cd /Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wangjinxi/dockerWorkspace/lycitea_new /Users/wangjinxi/dockerWorkspace/lycitea_new /Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug /Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug /Users/wangjinxi/dockerWorkspace/lycitea_new/cmake-build-debug/CMakeFiles/lycitea.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lycitea.dir/depend

