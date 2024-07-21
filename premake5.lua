workspace "MyOpenGl"
    configurations { "Debug", "Release" }
    platforms { "x64" }  -- Specify x64 platform
    location "Build"  -- Generated files will go here

    -- Platform-specific settings
    filter "system:windows"
        architecture "x64"
        systemversion "latest"
        debugdir ""

    filter "system:linux"
        architecture "x64"
        toolset "gcc"

    filter {}

project "MyOpenGl"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.cpp", "src/**.h", "src/**.hpp", "res/**" }  -- Source and header files

    cppdialect "C++20" -- Choose C++17, or C++20

    includedirs { "include", "Dependencies/glfw/include", "Dependencies/glew/include", "src/vendor", "res/**" }

    -- Link GLFW and GLEW libraries
    filter "system:windows"                             
        libdirs { "Dependencies/glfw/lib-vc2022", "Dependencies/glew/lib/Release/x64" }
        links { "glfw3", "glew32s", "opengl32" }  -- OpenGL library for Windows
        defines {"GLEW_STATIC"}

    filter "system:linux"
        links { "glfw", "GLEW", "GL" }  -- OpenGL library for Linux

    -- Debug and Release configuration
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
