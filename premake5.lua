-- File: premake5.lua

-- Brief: Build script for the premake build system. Run 'premake5 gmake' to 
--        build GNU specific makefiles. 

-- Author: Alexander DuPree

-- WORKSPACE CONFIGURATION --
workspace "CS300 Term Project"
    configurations { "debug", "release" }

    if _ACTION == "clean" then
        os.rmdir("bin/")
        os.rmdir("lib/")
        os.rmdir("gmake/")
        os.rmdir("gmake2/")
    end

    local project_action = "UNDEFINED"
    if _ACTION ~= nill then project_action = _ACTION end

    location (project_action)

    -- PLATFORM CONFIGURATIONS --

    -- COMPILER/LINKER CONFIG --
    flags "FatalWarnings"
    warnings "Extra"

    filter "configurations:debug*"   
        buildoptions { "-fprofile-arcs", "-ftest-coverage" }
        defines { "DEBUG" }
        links "gcov"
        symbols "On"

    filter "configurations:release*" 
        defines { "NDEBUG" } 
        optimize "On"

    filter "toolset:gcc"
        buildoptions { 
            "-Wall", "-Wextra", "-Werror", "-std=c++17"
        }

    filter {} -- close filter

project "ChocAn-Core"
    kind "StaticLib"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}/"
    targetname "ChocAn-Core"

    local include = "include/"
    local source  = "src/core/"

    files (source .. "*.cpp")
    includedirs (include)

project "ChocAn-Data"
    kind "StaticLib"
    links "ChocAn-Core"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}/"
    targetname "ChocAn-Data"

    local include = "include/"
    local source  = "src/data/"

    files (source .. "*.cpp")
    includedirs (include)

project "ChocAn-App"
    kind "StaticLib"
    links "ChocAn-Core"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}/"
    targetname "ChocAn-App"

    local include = "include/"
    local source  = "src/app/"

    files (source .. "*.cpp")
    includedirs (include)

project "ChocAn-View"
    kind "StaticLib"
    links "ChocAn-Core"
    links "ChocAn-App"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}/"
    targetname "ChocAn-View"

    local include = "include/"
    local source  = "src/view/"

    files (source .. "*.cpp")
    includedirs (include)

project "ChocAn-Exe"
    kind "ConsoleApp"
    language "C++"
    links "ChocAn-Core"
    links "ChocAn-Data"
    links "ChocAn-App"
    links "ChocAn-View"
    targetdir "bin/%{cfg.buildcfg}/"
    targetname  "ChocAn_%{cfg.buildcfg}"

    local source = "src/"
    local include = "include/"

    files (source .. "main.cpp")
    includedirs(include)

project "Tests"
    kind "ConsoleApp"
    language "C++"
    links "ChocAn-Core"
    links "ChocAn-Data"
    links "ChocAn-App"
    links "ChocAn-View"
    targetdir "bin/tests/"
    targetname "%{cfg.buildcfg}_tests"

    local include  = "include/"
    local test_src = "tests/"
    local test_inc = "third_party/"

    files (test_src .. "**.cpp")

    includedirs { test_inc, include }

    --postbuildcommands ".././bin/tests/%{cfg.buildcfg}_tests"

    filter {} -- close filter

