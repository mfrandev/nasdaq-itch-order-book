cmake_minimum_required(VERSION 3.15...3.25)

project(
    TrexquantTakeHome
    DESCRIPTION "VWAP from Binary ITCH 5.0 File"
    LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

include_directories(
    "."
    "./algorithm" 
    # "./lib" bring back if adding fmt::fmt into the directory
    "./messages"
    "./utils"
)

# The compiled library code is here
add_executable(vwap_generator main.cpp)
target_link_libraries(vwap_generator fmt::fmt)