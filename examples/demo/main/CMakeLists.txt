file(GLOB DISP_IMAGES_SRCS "disp_images/*.c")

file(GLOB_RECURSE COMMON_SRCS "../../common/*.cpp")

idf_component_register(SRCS "app_gui.cpp" "main.cpp" ${DISP_IMAGES_SRCS} ${COMMON_SRCS}
                    INCLUDE_DIRS "." "../../common")

# Suppress missing-field-initializers warning
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
