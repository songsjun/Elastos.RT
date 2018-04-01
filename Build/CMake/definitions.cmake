
set(XDK_DEFINITIONS)

if(DEFINED ENV{XDK_TARGET_PRODUCT})
    list(APPEND XDK_DEFINITIONS -D_$ENV{XDK_TARGET_PRODUCT})
endif()
if(DEFINED ENV{XDK_TARGET_BOARD})
    list(APPEND XDK_DEFINITIONS -D_$ENV{XDK_TARGET_BOARD})
endif()
if(DEFINED ENV{_ELASTOS64})
    list(APPEND XDK_DEFINITIONS -D_ELASTOS64)
else()
    list(APPEND XDK_DEFINITIONS -D_ELASTOS32)
endif()
list(APPEND XDK_DEFINITIONS -DELASTOS -D_GNUC -D_$ENV{XDK_TARGET_CPU} -D_$ENV{XDK_TARGET_PLATFORM})

if(APPLE)
    list(APPEND XDK_DEFINITIONS -D_apple)
endif()

list(APPEND XDK_DEFINITIONS -D_cmake)

add_definitions(${XDK_DEFINITIONS})
