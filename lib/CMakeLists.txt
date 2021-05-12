set(PROJECT_NAME linda-lib)

option(BUILD_CLIENT "Build client" OFF)

################################################################################
# Source groups
################################################################################
file(GLOB ALL_FILES "*.cpp")
list(FILTER ALL_FILES EXCLUDE REGEX ".*_test.cpp")

file(GLOB TEST_FILES "*_test.cpp")

source_group("Archivos de origen" FILES ${ALL_FILES})


################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME}-temp STATIC ${ALL_FILES})
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-temp ${CONAN_LIBS})

if(COMPILE_TESTS)
  add_library(Catch INTERFACE)
  target_include_directories(Catch INTERFACE)

  add_executable(tests ${TEST_FILES})
  target_link_libraries(tests PRIVATE Catch2::Catch2 linda-lib)
  add_test(NAME TESTS COMMAND tests)

  enable_testing(true)
endif()