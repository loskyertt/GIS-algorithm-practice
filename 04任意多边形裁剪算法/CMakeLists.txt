cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 设置默认构建类型为 Debug/Release
set(CMAKE_BUILD_TYPE Debug)
project(04任意多边形裁剪算法)

aux_source_directory(src SRC_SUB)
aux_source_directory(. SRC_CUR)

add_executable(test.out ${SRC_SUB} ${SRC_CUR})
target_include_directories(test.out PRIVATE include)

set(OpenGL_GL_PREFERENCE "GLVND")

# 添加 OpenGL 和 FreeGLUT 的查找和链接
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)  # FreeGLUT 包含在这里

target_include_directories(test.out PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(test.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})