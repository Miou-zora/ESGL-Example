add_requires("entt", "vulkan-headers", "vulkansdk", "vulkan-hpp", "gtest", "glm >=1.0.1", "glfw >=3.4", "vcpkg::gl3w >=v2018-05-31#5")

set_project("ESGL")
set_languages("c++20")

includes("../../Github/EngineSquared/xmake.lua")

target("ESGL")
    set_kind("binary")
    set_default(true)
    add_deps("EngineSquared")

    add_files("src/**.cpp")


    add_includedirs("$(projectdir)/src/")
    add_includedirs("$(projectdir)/src/MyGlWindow/")
    add_includedirs("$(projectdir)/src/Material/")
    add_includedirs("$(projectdir)/src/Model/Torus/")
    add_includedirs("$(projectdir)/src/Model/Quad/")
    add_includedirs("$(projectdir)/src/Viewer/")
    add_includedirs("$(projectdir)/src/Mesh/")
    add_includedirs("$(projectdir)/src/ShaderManager/")
    add_headerfiles("$(projectdir)/src/**.h")


    add_packages("entt", "vulkansdk", "glm", "glfw", "vcpkg::gl3w")

    set_rundir("$(projectdir)")


if is_mode("debug") then
    add_defines("DEBUG")
    set_symbols("debug")
    set_optimize("none")
end

if is_mode("release") then
    set_optimize("fastest")
end