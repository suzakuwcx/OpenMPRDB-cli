set( HEADERS
        GPGTool.h
        HttpConnection.h
        MPRDBC.h
        shell.h
        value.h)

set( SOURCES
        GPGTool.cpp
        HttpConnection.cpp
        MPRDBC.cpp
        value.cpp
        main.cpp)

add_executable(mprdbc-bin ${HEADERS} ${SOURCES})
message(STATUS "generate mprdbc-bin in ${PROJECT_SOURCE_DIR}/bin")