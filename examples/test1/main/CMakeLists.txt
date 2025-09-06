file(GLOB HAL_FILES "../../common/*.cpp")

idf_component_register(SRCS "main.cpp" ${HAL_FILES}
                    INCLUDE_DIRS "." "../../common/")

# Suppress missing-field-initializers warning
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)

