add_executable(check check.cpp)
target_link_libraries(check PUBLIC project_options project_warnings)
target_link_libraries(check PUBLIC fmt::fmt absl::random_random)

add_executable(test_protocol test_protocol.cpp cache.proto)
target_include_directories(test_protocol PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(test_protocol fmt::fmt protobuf::libprotobuf protobuf::libprotoc)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS TARGET test_protocol)
protobuf_generate(LANGUAGE cpp TARGET test_protocol PROTOS cache.proto)


add_executable(summarize_inputs summarize_inputs.cpp)
target_link_libraries(summarize_inputs PRIVATE project_options project_warnings)
target_link_libraries(summarize_inputs PRIVATE fmt::fmt re2::re2
   absl::flat_hash_set)
