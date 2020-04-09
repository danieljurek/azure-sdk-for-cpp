# Downloads and builds MSAL for C++.

if(CMAKE_VERSION VERSION_LESS 3.11)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")

    include(DownloadProject)
    download_project(
             PROJ                msal
             GIT_REPOSITORY      https://github.com/AzureAD/microsoft-authentication-library-for-cpp.git
             GIT_TAG             master
             UPDATE_DISCONNECTED 1
             QUIET
    )

    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
    add_subdirectory(${msal_SOURCE_DIR} ${msal_SOURCE_DIR} EXCLUDE_FROM_ALL)
    unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
else()
    include(FetchContent)
    FetchContent_Declare(msal
        GIT_REPOSITORY      https://github.com/AzureAD/microsoft-authentication-library-for-cpp.git
        GIT_TAG             master)

    FetchContent_GetProperties(msal)

    if(NOT msal_POPULATED)
    #    FetchContent_Populate(msal)
    #    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
    #    add_subdirectory(${msal_SOURCE_DIR} ${msal_BINARY_DIR} EXCLUDE_FROM_ALL)
    #    unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    endif()
endif()


#include_directories(${msal_SOURCE_DIR}/include)
include_directories("C:/Users/antk/source/repos/microsoft-authentication-library-for-cpp/source/zgenerated/cpp")

## More modern way to do the last line, less messy but needs newish CMake:
##target_include_directories(msal INTERFACE ${msal_SOURCE_DIR}/include)
#
#set_target_properties(msal msal_main
#    PROPERTIES FOLDER "Extern")
#    