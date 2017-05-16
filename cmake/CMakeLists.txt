cmake_minimum_required(VERSION 2.8)

project(prpc CXX)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "debug")
endif()



SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb -pthread")                                         
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall -pthread")  


find_path(GLOG_INCLUDE_DIR GlogWrapper.h ../include/glog)
INCLUDE_DIRECTORIES(${GLOG_INCLUDE_DIR})

include_directories(../examples/protocol)
include_directories(../include/base)
#include_directories(../include/glog)
include_directories(../include/net)
include_directories(../include/protorpc)
include_directories(../include/protorpc/proto)
include_directories(../include/protorpc/include_protobuf)
include_directories(../examples)

#find_path(LIBEVENT_LIB_DIR libevent.a ../lib)
#find_path(MYSQLCPP_LIB_DIR libmysqlpp.so ../lib)
#find_path(ZOOKEEPER_LIB_DIR libBSRZookeeper.so ../../common/ZookeeperLib/BSZookeeper/cmake)
#find_path(ZOOKEEPER_LIB_DIR_BASE libzookeeper_mt.so ../../common/ZookeeperLib/BSZookeeper/lib)


LINK_DIRECTORIES(../lib)


aux_source_directory(../src/glog GLOG_SRC)
aux_source_directory(../src/protorpc PROTORPC_SRC)
aux_source_directory(../include/protorpc/proto PROTO_SRC)
aux_source_directory(../src/base BASE_SRC)
aux_source_directory(../src/net NET_SRC)
aux_source_directory(../examples/protocol EXAMPLES_PROTO_SRC)

#GLOB_RECURSE
#file(GLOB HEADERS "./*.h") 
#file(GLOB HEADERS "./glog/glog/*.h")
#file(GLOB HEADERS "./protorpc/include_protobuf/*.h")

SET(RPC_BASE_LIST 
${GLOG_SRC} 
${PROTORPC_SRC} 
${PROTO_SRC} 
${BASE_SRC} 
${NET_SRC} 
${EXAMPLES_PROTO_SRC}
)

SET (LIBS libglog.a libprotobuf.a pthread)

SET(PROJECT_ROOT_PATH ${CMAKE_SOURCE_DIR})                 #工程的根目录，即test
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")    #可执行生成后存放的目录(CMAKE_SOURCE_DIR是cmake目录)
#SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")             #静态库生成后存放的目录

add_executable(prpc_server ${RPC_BASE_LIST} ../examples/TaskRunningRpc.cpp ../examples/exam_server.cpp)
TARGET_LINK_LIBRARIES(prpc_server ${LIBS})


add_executable(prpc_client ${RPC_BASE_LIST} ../examples/SchedulerRpc.cpp ../examples/exam_client.cpp)
#TARGET_LINK_LIBRARIES(PlanService gmock gtest glog protobuf pthread)
TARGET_LINK_LIBRARIES(prpc_client ${LIBS})

