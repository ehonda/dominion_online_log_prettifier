WORKING WITH THE UNITTESTS
If you want to work on this project yourself and run the unit tests provided in dominion_online_log_prettifier_2_test you will need
the latest version of the googletest unittesting framework. To set that up, follow these steps:

1. Downloading and building googletest:
The latest version of googletest can be obtained at
https://github.com/google/googletest

Download as .zip and extract googletest-master to directory of choice. Go to directory
googletest-master/googletest/msvc
and build gtest-md.sln.

2. Environment variables:
GTEST_MD_INCLUDE_DIR			-	Should point to the /include dir of googletest
GTEST_MD_LIB_32_DEBUG_DIR		-	Should point to the googletest-md msvc Debug build
GTEST_MD_LIB_32_RELEASE_DIR		-	Should point to the googletest-md msvc Release build


PORTABILITY
For the application itself, no special libraries are needed. It should be portable, though it is only tested on windows so far, but
it uses no windows specific stuff, so it should run just fine on any other platform if the project is setup in the right way.