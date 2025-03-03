add_requires("entt", "vulkan-headers", "vulkansdk", "vulkan-hpp", "gtest", "glm >=1.0.1", "glfw >=3.4", "vcpkg::gl3w >=v2018-05-31#5")

set_project("ESGL")
set_languages("c++20")

includes("../../../../EngineSquared/xmake.lua")

target("ESGL")
    set_kind("static")
    set_languages("cxx20")
    set_policy("build.warning", true)
    add_packages("entt", "vulkansdk", "glm", "glfw", "vcpkg::gl3w")

    add_deps("EngineSquared")

    add_files("src/**.cpp")

    add_includedirs("src/", {public = true})
    add_includedirs("src/utils/Material/", {public = true})
    add_includedirs("src/utils/Viewer/", {public = true})
    add_includedirs("src/utils/Loader/", {public = true})
    add_includedirs("src/utils/Mesh/", {public = true})
    add_includedirs("src/component/Model/", {public = true})
    add_includedirs("src/resource/ShaderManager/", {public = true})
    add_includedirs("src/resource/ESGLFWWINDOW/", {public = true})
    add_includedirs("src/resource/MaterialCache/", {public = true})
    add_includedirs("src/resource/Camera/", {public = true})
    add_includedirs("src/system/", {public = true})
    add_headerfiles("src/**.hpp")


for _, file in ipairs(os.files("tests/**.cpp")) do
    local name = path.basename(file)
    if name == "main" then
        goto continue
    end
    target(name)
        set_kind("binary")
        if is_plat("linux") then
            add_cxxflags("--coverage", "-fprofile-arcs", "-ftest-coverage", {force = true})
            add_ldflags("--coverage")
        end
        set_default(false)
        set_languages("cxx20")
        add_links("gtest")
        add_tests("default")
        add_packages("entt", "vulkansdk", "glm", "glfw", "vcpkg::gl3w")
        
        add_deps("ESGL")

        add_files(file)
        add_files("tests/main.cpp")
        if is_mode("debug") then
            add_defines("DEBUG")
        end
    ::continue::
end
