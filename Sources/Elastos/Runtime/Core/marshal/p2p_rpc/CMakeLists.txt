
set(SOURCES
    metainfos.cpp
    rot.cpp
    mshproc.cpp
    imarshal.cpp
    proxy.cpp
    stub.cpp
    CRemoteParcel.cpp
    CMessage.cpp
    session.cpp
    CSessionManager.cpp
)

add_definitions(-D_UNDEFDLLEXP)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../Library/inc/elasys)
include_directories(${CMAKE_BINARY_DIR}/ThirdParty/ElaCarrier/build/_dist/Android-arm/debug/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../carrier)

add_library(marshal STATIC ${SOURCES})
