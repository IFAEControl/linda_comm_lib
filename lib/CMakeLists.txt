set(PROJECT_NAME linda-lib)

option(BUILD_CLIENT "Build client" OFF)

################################################################################
# Source groups
################################################################################
file(GLOB Archivos_de_origen *.cpp)

source_group("Archivos de origen" FILES ${Archivos_de_origen})

set(ALL_FILES
    ${Archivos_de_origen}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME}-temp STATIC ${ALL_FILES})
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-temp ${CONAN_LIBS})
