newoption
{
    trigger = "ios",
    description = "generates ios project without native target"
}

solution "ShaderShaker"
    configurations { "Release", "Debug" }

    targetname  "shader_shaker"
    language    "C++"
    flags       { "ExtraWarnings" }
    includedirs { "include", "contrib", "src" }

    files
    {
        "src/**.h", "src/**.cpp"
    }

    if _OPTIONS[ "ios" ] then
        platforms { "ios" }

        if deploymenttarget then
            deploymenttarget "5.1"
        end

    end

    configuration "Debug"
        defines     { "_DEBUG" }
        flags       { "Symbols" }

    configuration "Release"
        defines     { "NDEBUG" }
        flags       { "OptimizeSize" }

    configuration "vs*"
        defines     { "_CRT_SECURE_NO_WARNINGS" }

project "ShaderShaker"

    kind        "ConsoleApp"

    configuration "Debug"
        targetdir   "bin/debug"

    configuration "Release"
        targetdir   "bin/release"

project "ShaderShakerDll"
    defines{ "SHADERSHAKER_IN_DLL" }

    kind "SharedLib"

    configuration "Debug"
        targetdir   "bin/debug_dll"

    configuration "Release"
        targetdir   "bin/release_dll"

project "ShaderShakerLib"

    defines{ "SHADERSHAKER_IN_LIB" }
    kind "StaticLib"

    configuration "Debug"
        targetdir   "bin/debug_lib"

    configuration "Release"
        targetdir   "bin/release_lib"

if _ACTION == "clean" then
    os.rmdir("bin")
    os.rmdir("build")
end
