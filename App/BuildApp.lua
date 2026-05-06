project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

    files { "include/**.hpp", "src/**.cpp" }

    includedirs {
        "include",
        "../Engine/include/"
    }

    links {
        "Engine",
    }

    dependson { "Engine" }

    filter "configurations:Debug"
        defines({ "DEBUG" })
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines({ "NDEBUG" })
        runtime "Release"
        optimize "On"
        symbols "On"

