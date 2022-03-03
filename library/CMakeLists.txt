add_library(mem INTERFACE)

add_library(mem::mem ALIAS mem)

target_link_libraries(mem INTERFACE fmt::fmt)

target_include_directories(mem
    INTERFACE
        $<INSTALL_INTERFACE:include>
    	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
