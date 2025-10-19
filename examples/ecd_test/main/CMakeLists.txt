file(GLOB_RECURSE COMMON_SRCS "../../common/*.cpp")

idf_component_register(SRCS "main.cpp" ${COMMON_SRCS}
                    INCLUDE_DIRS "." "../../common")

# Suppress missing-field-initializers warning
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
