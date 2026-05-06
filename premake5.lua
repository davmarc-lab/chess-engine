workspace "Chess-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "App"

    OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

    libdirs = {
        "bin/" .. OutputDir .. "/",
    }

    group "Engine"
        include "./Engine/BuildEngine.lua"

    group "App"
        include "./App/BuildApp.lua"

    group "Test"
        include "./Test/BuildTest.lua"

