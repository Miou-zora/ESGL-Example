add_requires("entt", "vulkan-headers", "vulkansdk", "vulkan-hpp", "gtest", "glm >=1.0.1", "glfw >=3.4", "vcpkg::gl3w >=v2018-05-31#5")

set_project("ESGL-App")
set_languages("c++20")

includes("../EngineSquared/xmake.lua")
includes("src/plugin/ESGL/xmake.lua")

target("ESGL-App")
    set_kind("binary")
    set_default(true)
    add_deps("EngineSquared")
    add_deps("ESGL")

    add_files("src/app/**.cpp")
    add_includedirs("$(projectdir)/src/")

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