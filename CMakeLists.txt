cmake_minimum_required(VERSION 3.31)

# CMake 4.x removed compatibility with policy versions older than 3.5.
# Some third-party subprojects still declare very old minima, so raise the
# policy floor for subdirectory processing.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0")
    set(CMAKE_POLICY_VERSION_MINIMUM "3.5")
endif ()

file(READ "${CMAKE_SOURCE_DIR}/version.json" version_json)
string(JSON LUASTG_PACKAGE_NAME GET ${version_json} "package")
string(JSON LUASTG_ORGNIZATION_NAME GET ${version_json} "organization")
string(JSON LUASTG_VERSION_MAJOR GET ${version_json} "major")
string(JSON LUASTG_VERSION_MINOR GET ${version_json} "minor")
string(JSON LUASTG_VERSION_PATCH GET ${version_json} "patch")
message(STATUS "LuaSTG Retro v${LUASTG_VERSION_MAJOR}.${LUASTG_VERSION_MINOR}.${LUASTG_VERSION_PATCH}")

project(LuaSTGRetro
    VERSION      "${LUASTG_VERSION_MAJOR}.${LUASTG_VERSION_MINOR}.${LUASTG_VERSION_PATCH}"
    DESCRIPTION  "LuaSTG Retro: Game engine for bullet hell games, using Lua as the scripting language"
    HOMEPAGE_URL "https://github.com/Hoshiruna/LuaSTG-Retro"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Although this project doesn't make use of C++20 Modules,
# the module scanning process runs during every build, this increases the build time.
# See: https://cmake.org/cmake/help/latest/manual/cmake-cxxmodules.7.html#scanning-without-modules
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(cmake/options.cmake)
include(cmake/architecture.cmake)
include(cmake/luastg_options.cmake)
include(cmake/TargetCommonOptions.cmake)
include(cmake/msvc.crt.cmake)

msvc_static_runtime_library()

include(cmake/import/all.cmake)
include(cmake/build/all.cmake)
add_subdirectory(external)
include(cmake/packages.lua.cmake)
add_subdirectory(imgui)

add_subdirectory(engine)
target_link_libraries(lua_imgui PUBLIC lua_plus) # TODO: 丑不拉几的
add_subdirectory(tool)
add_subdirectory(LuaSTG)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    message(STATUS "Current CMake generator is Visual Studio, set startup project to LuaSTG")
    set_directory_properties(PROPERTY VS_STARTUP_PROJECT LuaSTG)
endif ()
