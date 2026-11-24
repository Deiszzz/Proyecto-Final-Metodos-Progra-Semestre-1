
set_project("JuegoProyectoFinal")
set_version("1.0.0")
set_languages("cxx20")

add_rules("mode.debug", "mode.release")

target("JuegoProyectoFinal")
    set_kind("binary")

    -- Compilar todos los archivos .cpp
    add_files("src/*.cpp")

    -- Permitir encontrar los archivos .hpp
    add_includedirs("include")
