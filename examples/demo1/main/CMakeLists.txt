file(GLOB DISP_IMAGES_SRCS "disp_images/*.c")

idf_component_register(SRCS "app_gui.cpp" "main.cpp" ${DISP_IMAGES_SRCS}
                    INCLUDE_DIRS ".")
