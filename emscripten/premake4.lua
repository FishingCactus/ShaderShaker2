
solution "ShaderShaker"
    configurations { "Release", "Debug" }

    targetname  "shader_shaker"
    language    "C++"
    flags       { "ExtraWarnings" }
    includedirs { "../include", "../contrib/catch", "../contrib/antlr", "../src" }

    buildoptions{ "-std=c++11", "-Wno-warn-absolute-paths" }
    linkoptions{ "--bind" }
    files
    {
        "*.cpp", "../src/**.h", "../src/**.cpp"
    }

    configuration "Debug"
        defines     { "_DEBUG" }
        flags       { "Symbols" }

    configuration "Release"
        defines     { "NDEBUG" }
        flags       { "OptimizeSize" }

project "ShaderShaker"

    kind        "ConsoleApp"
    targetextension ".js"

    excludes{ "../src/main.cpp" }

    configuration "Debug"
        targetdir   "bin/debug"

    configuration "Release"
        targetdir   "bin/release"

project "Test"

    kind "ConsoleApp"
    targetname  "test"
    targetextension ".js"

    files{ "../test/src/**.cpp" }
    excludes{ "../src/main.cpp" }

    configuration "Debug"
        targetdir   "bin/debug"

    configuration "Release"
        targetdir   "bin/release"

if _ACTION == "clean" then
    os.rmdir("bin")
    os.rmdir("build")
end
