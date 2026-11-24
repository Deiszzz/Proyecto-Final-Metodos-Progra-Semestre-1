#include "funciones.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {

const array<string, 16> nombres_familias = {
    "Lineal lenta",
    "Cuadratica",
    "Cubica",
    "Raiz cuadrada",
    "Logaritmica",
    "Racional",
    "Exponencial Limitada",
    "Seno",
    "Coseno",
    "Tangente controlada",
    "Lineal + Seno",
    "Raiz + Coseno",
    "Seno + Coseno",
    "Onda Triangular",
    "Diente de Sierra",
    "Tangente cuadratica absoluta"
};

// Emula la conversión saturada de f64 a i64 que utilizaba Rust.
int64_t convertir_a_entero(double valor) {
    if (isnan(valor)) return 0;

    // 2^63: es representable exactamente como double.
    constexpr double LIMITE = 0x1p63;
    if (valor >= LIMITE) return numeric_limits<int64_t>::max();
    if (valor <= -LIMITE) return numeric_limits<int64_t>::min();

    return static_cast<int64_t>(valor);
}

// Las operaciones saturadas evitan desbordamientos de enteros de 64 bits.
int64_t resta_saturada(int64_t a, int64_t b) {
    constexpr auto MINIMO = numeric_limits<int64_t>::min();
    constexpr auto MAXIMO = numeric_limits<int64_t>::max();

    if (b > 0 && a < MINIMO + b) return MINIMO;
    if (b < 0 && a > MAXIMO + b) return MAXIMO;
    return a - b;
}

int64_t suma_saturada(int64_t a, int64_t b) {
    constexpr auto MINIMO = numeric_limits<int64_t>::min();
    constexpr auto MAXIMO = numeric_limits<int64_t>::max();

    if (b > 0 && a > MAXIMO - b) return MAXIMO;
    if (b < 0 && a < MINIMO - b) return MINIMO;
    return a + b;
}

} // namespace

string Funcion::nombre() const {
    if (familia >= nombres_familias.size()) {
        throw out_of_range("Familia no valida");
    }
    return nombres_familias[familia];
}

int64_t Funcion::evaluar(int turno) const {
    // t es el turno y k es el número de variante de la función.
    double t = turno;
    double k = variante;

    double amplitud = 2 + variante % 4;
    double angulo1 = t * (0.14 + 0.012 * k) + 0.37 * k;
    double angulo2 = t * (0.29 + 0.006 * k) + 0.51 * k;
    double resultado = 0.0;

    switch (familia) {
        case 0: // Lineal lenta
            resultado = floor(t / (6.0 + k));
            break;

        case 1: // Cuadrática
            resultado = floor(t * t / (80.0 + 12.0 * k));
            break;

        case 2: // Cúbica
            resultado = floor(t * t * t / (600.0 + 150.0 * k));
            break;

        case 3: // Raíz cuadrada
            resultado = floor((0.55 + 0.10 * k) * sqrt(t));
            break;

        case 4: // Logarítmica
            resultado = floor((1.2 + 0.2 * k) * log(t + 1.0));
            break;

        case 5: // Racional
            resultado = round((4.0 + k) * t / (t + 8.0 + k));
            break;

        case 6: // Exponencial limitada
            resultado = round((4.0 + k) * (1.0 - exp(-t / (4.0 + k))));
            break;

        case 7: // Seno
            resultado = round(amplitud * sin(angulo1));
            break;

        case 8: // Coseno
            resultado = round(amplitud * cos(angulo1));
            break;

        case 9: { // Tangente controlada
            int periodo = 22 + variante;
            int posicion = (turno % periodo + 2 * variante) % periodo;

            // En el repositorio se usa 11.30 aquí y 1.30 en formula().
            // Lo conservamos para no cambiar los ataques del juego.
            double angulo = -0.65 + 11.30 * posicion / (periodo - 1);
            resultado = round(amplitud * tan(angulo));
            break;
        }

        case 10: // Lineal + seno
            resultado = floor(t / (9.0 + k)) + round(3.0 * sin(angulo1));
            break;

        case 11: // Raíz + coseno
            resultado = floor(0.65 * sqrt(t)) + round(amplitud * cos(angulo1));
            break;

        case 12: // Seno + coseno
            resultado = round(3.0 * sin(angulo1) + 2.0 * cos(angulo2));
            break;

        case 13: { // Onda triangular
            int periodo = 8 + 2 * variante;
            int posicion = (turno % periodo + variante) % periodo;
            double mitad = (periodo - 1) / 2.0;
            resultado = round(5.0 - 10.0 * abs(posicion - mitad) / mitad);
            break;
        }

        case 14: { // Diente de sierra
            int periodo = 7 + variante;
            int posicion = (turno % periodo + variante) % periodo;
            resultado = round(posicion - (periodo - 1) / 2.0);
            break;
        }

        case 15: // Función especial: |tan(t² - 1)|
            resultado = round(abs(tan(t * t - 1.0)));
            break;

        default:
            throw out_of_range("Familia no valida");
    }

    return convertir_a_entero(resultado);
}

string Funcion::formula() const {
    double k = variante;
    int amplitud = 2 + variante % 4;

    double a1 = 0.14 + 0.012 * k;
    double b1 = 0.37 * k;
    double a2 = 0.29 + 0.006 * k;
    double b2 = 0.51 * k;

    ostringstream texto;

    switch (familia) {
        case 0:
            texto << "f(t) = ⌊t / " << 6 + variante << "⌋";
            break;

        case 1:
            texto << "f(t) = ⌊t² / " << 80 + 12 * variante << "⌋";
            break;

        case 2:
            texto << "f(t) = ⌊t³ / " << 600 + 150 * variante << "⌋";
            break;

        case 3:
            texto << "f(t) = ⌊" << fixed << setprecision(2)
                  << 0.55 + 0.10 * k << "√t⌋";
            break;

        case 4:
            texto << "f(t) = ⌊" << fixed << setprecision(2)
                  << 1.2 + 0.2 * k << "·ln(t + 1)⌋";
            break;

        case 5:
            texto << "f(t) = redondear(" << 4 + variante << "t / (t + "
                  << 8 + variante << "))";
            break;

        case 6:
            texto << "f(t) = redondear(" << 4 + variante
                  << "·(1 - e^(-t / " << 4 + variante << ")))";
            break;

        case 7:
            texto << "f(t) = redondear(" << amplitud << "·sen("
                  << fixed << setprecision(3) << a1 << "t + " << b1 << "))";
            break;

        case 8:
            texto << "f(t) = redondear(" << amplitud << "·cos("
                  << fixed << setprecision(3) << a1 << "t + " << b1 << "))";
            break;

        case 9: {
            int periodo = 22 + variante;
            texto << "r(t) = (t + " << 2 * variante << ") mod " << periodo << '\n'
                  << "f(t) = redondear(" << amplitud
                  << "·tan(-0.65 + 1.30r(t) / " << periodo - 1 << "))";
            break;
        }

        case 10:
            texto << "f(t) = ⌊t / " << 9 + variante
                  << "⌋ + redondear(3·sen(" << fixed << setprecision(3)
                  << a1 << "t + " << b1 << "))";
            break;

        case 11:
            texto << "f(t) = ⌊0.65√t⌋ + redondear(" << amplitud
                  << "·cos(" << fixed << setprecision(3)
                  << a1 << "t + " << b1 << "))";
            break;

        case 12:
            texto << "f(t) = redondear(3·sen(" << fixed << setprecision(3)
                  << a1 << "t + " << b1 << ") + 2·cos("
                  << a2 << "t + " << b2 << "))";
            break;

        case 13: {
            int periodo = 8 + 2 * variante;
            double mitad = (periodo - 1) / 2.0;
            texto << "r(t) = (t + " << variante << ") mod " << periodo << '\n'
                  << "f(t) = redondear(5 − 10·|r(t) − " << fixed
                  << setprecision(1) << mitad << "| / " << mitad << ")";
            break;
        }

        case 14: {
            int periodo = 7 + variante;
            double mitad = (periodo - 1) / 2.0;
            texto << "r(t) = (t + " << variante << ") mod " << periodo << '\n'
                  << "f(t) = redondear(r(t) − " << fixed
                  << setprecision(1) << mitad << ")";
            break;
        }

        case 15:
            texto << "g(t) = |tan(t² − 1)|\nf(t) = redondear(g(t))";
            break;

        default:
            throw out_of_range("Familia matemática desconocida");
    }

    return texto.str();
}

int64_t Funcion::cambio_ataque(int turno) const {
    int64_t diferencia = resta_saturada(evaluar(turno), evaluar(0));
    return clamp(diferencia, CAMBIO_MINIMO, CAMBIO_MAXIMO);
}

int64_t Funcion::ataque(int64_t ataque_inicial, int turno) const {
    return max<int64_t>(1, suma_saturada(ataque_inicial, cambio_ataque(turno)));
}

vector<Funcion> generar_funciones() {
    vector<Funcion> funciones;
    funciones.reserve(TOTAL_FUNCIONES);

    for (size_t familia = 0; familia < FAMILIAS_CON_VARIANTES; ++familia) {
        for (int variante = 1; variante <= VARIANTES_POR_FAMILIA; ++variante) {
            funciones.push_back({familia, variante});
        }
    }

    funciones.push_back({15, 1}); // Función especial #151.
    return funciones;
}
