workspace "arkanoid"
    configurations { "Debug", "Release" }
    architecture "x86_64"

project "arkanoid"
    kind "ConsoleApp"
    language "C"
    cdialect "C11"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/obj/%{cfg.buildcfg}"

    files {"raylib.h"}

    files { "**.c", "**.h" }

    includedirs { "." }

    links { "raylib" }

    filter "system:macosx"
        links {
            "IOKit.framework",
            "Cocoa.framework",
            "OpenGL.framework"
        }

    filter "system:linux"
        links { "m", "pthread", "dl" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"