find_package(wxWidgets COMPONENTS core base)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else(wxWidgets_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        wxWidgets
        GIT_REPOSITORY "https://github.com/wxWidgets/wxWidgets"
        GIT_TAG        "v3.2.0"
        GIT_PROGRESS   TRUE
        GIT_SHALLOW    TRUE
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
        -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
    )
    set(FETCHCONTENT_QUIET NO)

    FetchContent_MakeAvailable(wxWidgets)
    set(wxWidgets_LIBRARIES wxcore wxbase)
endif(wxWidgets_FOUND)

add_library(wxWidgets_external INTERFACE)
target_link_libraries(wxWidgets_external INTERFACE ${wxWidgets_LIBRARIES})
