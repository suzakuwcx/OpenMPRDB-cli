include_directories(${PROJECT_SOURCE_DIR}/lib)

set( HEADERS
        GPGTool.h
        HttpConnection.h
        shell.hpp
        DataBase.h
        Config.h
        )

set( SOURCES
        GPGTool.cpp
        HttpConnection.cpp
        main.cpp
        value.cpp
        DataBase.cpp
        Config.cpp
        MPRDBC.cpp
        )

add_executable(mprdbc-bin ${HEADERS} ${SOURCES})
message(STATUS "generate mprdbc-bin in ${PROJECT_SOURCE_DIR}/bin")