#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>


// Familias de funciones
inline constexpr uint32_t VARIANTES_POR_FAMILIA = 10;
inline constexpr size_t FAMILIAS_CON_VARIANTES = 15;
inline constexpr size_t TOTAL_FUNCIONES = VARIANTES_POR_FAMILIA * FAMILIAS_CON_VARIANTES + 1;

// Configuraciones de ataque
inline constexpr uint64_t ATAQUE_INICIAL = 10;
inline constexpr uint64_t CAMBIO_MINIMO = -8;
inline constexpr uint64_t CAMBIO_MAXIMO = 12;

inline constexpr uint32_t ANALISIS_PUNTAJE = 20;

// Representa una función matematica
struct Funcion {
    size_t familia;
    uint32_t variante;
    
    std::string nombre() const;
    int64_t evaluar (uint32_t turno) const;
    std::string formula() const;

    int64_t cambio_ataque(uint32_t turno) const;

    int64_t ataque(
        int64_t ataque_inicial,
        uint32_t turno
    ) const;
};

// Genera todas las funciones
std::vector<Funcion> generar_funciones();