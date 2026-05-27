cmake_minimum_required(VERSION 3.24)
project(civ6AccessibilityIntegration VERSION 0.3.0 LANGUAGES C CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(FATAL_ERROR "This project is only supported on 64-bit platforms.")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
fetchContent_Declare(
	MinHook
	GIT_REPOSITORY
	https://github.com/TsudaKageyu/MinHook.git
	GIT_TAG
	v1.3.4
)
fetchContent_MakeAvailable(MinHook)

fetchContent_Declare(
	spdlog
	GIT_REPOSITORY
	https://github.com/gabime/spdlog.git
	GIT_TAG
	v1.15.3
)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
fetchContent_MakeAvailable(spdlog)
target_compile_definitions(spdlog PUBLIC SPDLOG_WCHAR_FILENAMES)

fetchContent_Declare(
	tolk
	GIT_REPOSITORY
	https://github.com/ahicks92/tolk
	GIT_TAG
	44ae67d4c72ede93afbf2eaacfd1736f29653538
)
fetchContent_MakeAvailable(tolk)

add_library(tolkStatic STATIC
	"${tolk_SOURCE_DIR}/src/Tolk.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverJAWS.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverNVDA.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverSA.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverSNova.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverWE.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverZT.cpp"
	"${tolk_SOURCE_DIR}/src/ScreenReaderDriverSAPI.cpp"
	"${tolk_SOURCE_DIR}/src/fsapi.c"
	"${tolk_SOURCE_DIR}/src/wineyes.c"
	"${tolk_SOURCE_DIR}/src/zt.c"
)
target_compile_definitions(tolkStatic PUBLIC UNICODE _UNICODE _EXPORTING)
target_include_directories(tolkStatic PUBLIC "${tolk_SOURCE_DIR}/src")
target_link_libraries(tolkStatic PUBLIC user32 ole32 oleaut32)

add_library(${PROJECT_NAME} SHARED
	src/charInput.cpp
	src/entry.cpp
	src/hooks.cpp
	src/log.cpp
	src/luaApi.cpp
	src/utils.cpp
	src/luaBindings.cpp
	src/luaMethods.cpp
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
	UNICODE
	_UNICODE
	WIN32_LEAN_AND_MEAN
	NOMINMAX
	$<$<CONFIG:Debug>:CAI_DEV_LOGGING=1>
)
target_link_libraries(${PROJECT_NAME} PRIVATE minhook spdlog::spdlog tolkStatic)

file(GLOB TOLK_LIBS "${tolk_SOURCE_DIR}/libs/x64/*.dll")
if(TOLK_LIBS)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TOLK_LIBS}
			"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	)
endif()

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION "${CIV6_GAME_BIN_DIR}"
)

set_target_properties(${PROJECT_NAME}
	PROPERTIES OUTPUT_NAME "LightFX"
)
