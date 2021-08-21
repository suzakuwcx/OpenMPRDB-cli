set( HEADERS
        GPGTool.h
        HttpConnection.h
        MPRDBC.h
        shell.hpp
        )

set( SOURCES
        GPGTool.cpp
        HttpConnection.cpp
        MPRDBC.cpp
        main.cpp
        value.cpp
        )

add_executable(mprdbc-bin ${HEADERS} ${SOURCES})
message(STATUS "generate mprdbc-bin in ${PROJECT_SOURCE_DIR}/bin")