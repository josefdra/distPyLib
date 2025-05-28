# CMake generated Testfile for 
# Source directory: /Users/josefdra/dev/projects/distPyLib/tests/cc
# Build directory: /Users/josefdra/dev/projects/distPyLib/build/tests/cc
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[MPI_Tests]=] "mpirun" "-n" "2" "./test_runner")
set_tests_properties([=[MPI_Tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/josefdra/dev/projects/distPyLib/tests/cc/CMakeLists.txt;18;add_test;/Users/josefdra/dev/projects/distPyLib/tests/cc/CMakeLists.txt;0;")
