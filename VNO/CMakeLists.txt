INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/json-cpp/include
    ${CMAKE_SOURCE_DIR}/third_party/lemon
    ${CMAKE_BINARY_DIR}/third_party/lemon
    )
LINK_DIRECTORIES(
    ${CMAKE_BINARY_DIR}/bin
    )


FILE(GLOB_RECURSE VNO_APP_INCS "*.h")

SET(VNO_APP_SRCS
    DataUtils.cpp
    GraphUtils.cpp
    GraphData.cpp
    VentNetwork.cpp
    IndependentCycle.cpp
    AdjustPathMethod.cpp
    MaxResistancePath.cpp
    AirFlowDistribution.cpp
    InitFixQ.cpp
    DFSAllPath.cpp
    )

ADD_LIBRARY(vno STATIC ${VNO_APP_SRCS} ${VNO_APP_INCS})
TARGET_LINK_LIBRARIES(vno lemon)
SET(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
