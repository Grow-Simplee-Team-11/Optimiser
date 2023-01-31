cmake_minimum_required(VERSION 3.18)

project(Optimizer C CXX)

include(./cmake/common.cmake)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(ortools CONFIG REQUIRED)
find_package(CURL REQUIRED)


get_filename_component(dp_proto "./proto/main.proto" ABSOLUTE)
get_filename_component(dp_proto_path "${dp_proto}" PATH)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Generated sources
set(dp_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/main.pb.cc")
set(dp_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/main.pb.h")
set(dp_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/main.grpc.pb.cc")
set(dp_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/main.grpc.pb.h")
add_custom_command(
      OUTPUT "${dp_proto_srcs}" "${dp_proto_hdrs}" "${dp_grpc_srcs}" "${dp_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${dp_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${dp_proto}"
      DEPENDS "${dp_proto}")

MACRO(HEADER_DIRECTORIES return_list)
      FILE(GLOB_RECURSE new_list "${CMAKE_SOURCE_DIR}/include/*.hpp")
      SET(dir_list "")
      FOREACH(file_path ${new_list})
          GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
          SET(dir_list ${dir_list} ${dir_path})
      ENDFOREACH()
      LIST(REMOVE_DUPLICATES dir_list)
      SET(${return_list} "${dir_list}")
ENDMACRO()  

MACRO(SRC_DIRECTORIES return_list)
      FILE(GLOB_RECURSE new_list 
      "${CMAKE_SOURCE_DIR}/src/*.cpp"
      "${CMAKE_SOURCE_DIR}/src/*.c")
      SET(dir_list "")
      FOREACH(file_path ${new_list})
          GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
          SET(dir_list ${dir_list} ${dir_path})
      ENDFOREACH()
      LIST(REMOVE_DUPLICATES dir_list)
      SET(${return_list} ${dir_list})
ENDMACRO() 

HEADER_DIRECTORIES(header_dir_list)
SRC_DIRECTORIES(src_dir_list)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(
  ${header_dir_list}
  ${src_dir_list}
)

add_library(dp_grpc_proto
    ${dp_grpc_srcs}
    ${dp_grpc_hdrs}
    ${dp_proto_srcs}
    ${dp_proto_hdrs})    

    target_link_libraries(dp_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

file (GLOB_RECURSE ALL_SRCS
"${CMAKE_SOURCE_DIR}/include/*.hpp"
"${CMAKE_SOURCE_DIR}/src/*.cpp"
"${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(server server.cpp ${ALL_SRCS} )
target_link_libraries(server
    dp_grpc_proto
    grpc++
    ${CURL_LIBRARIES}
    ortools::ortools
)
