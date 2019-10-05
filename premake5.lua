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
            "-Wall", "-Wextra", "-Werror", "-std=c++11"
        }

    filter {} -- close filter

project "ChocAn"
    kind "StaticLib"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}/"
    targetname "ChocAn"

    local include = "include/"
    local source  = "src/"

    files (source .. "*.cpp")
    includedirs (include)

project "ChocAn-App"
    kind "ConsoleApp"
    language "C++"
    links "ChocAn"
    targetdir "bin/%{cfg.buildcfg}/"
    targetname  "ChocAn_%{cfg.buildcfg}"

    local source = "src/"
    local include = "include/"

    files (source .. "main.cpp")
    includedirs(include)

project "Tests"
    kind "ConsoleApp"
    language "C++"
    links "ChocAn"
    targetdir "bin/tests/"
    targetname "%{cfg.buildcfg}_tests"

    local include  = "include/"
    local test_src = "tests/"
    local test_inc = "third_party/"

    files (test_src .. "**.cpp")

    includedirs { test_inc, include }

    --postbuildcommands ".././bin/tests/%{cfg.buildcfg}_tests"

    filter {} -- close filter

