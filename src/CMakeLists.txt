add_executable(check check.cpp)
target_link_libraries(check PUBLIC project_options project_warnings)
target_link_libraries(check PUBLIC CONAN_PKG::fmt CONAN_PKG::abseil)
