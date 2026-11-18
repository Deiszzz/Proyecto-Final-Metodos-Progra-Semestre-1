
use std::io::{self, Write};

const NOMBRES: [&str; 16] = [
    "Lineal lenta",
    "Cuadrática",
    "Cúbica",
    "Raíz cuadrada",
    "Logarítmica",
    "Racional",
    "Exponencial limitada",
    "Seno",
    "Coseno",
    "Tangente controlada",
    "Lineal + seno",
    "Raíz + coseno",
    "Seno + coseno",
    "Onda triangular",
    "Diente de sierra",
    "Tangente cuadrática absoluta",
];

const VARIANTES: u32 = 10;
const FAMILIAS_CON_VARIANTES: usize = 15;
const CAMBIO_MINIMO: i64 = -8;
const CAMBIO_MAXIMO: i64 = 12;
const ATAQUE_INICIAL: i64 = 10;
const TURNOS_ANALISIS: u32 = 20;

#[derive(Clone, Copy)]
struct Funcion {
    familia: usize,
    variante: u32,
}

impl Funcion {
    fn nombre(&self) -> &'static str {
        NOMBRES[self.familia]
    }

    // valor de funcion en turno t
    fn evaluar(&self, turno: u32) -> i64 {
        let t = turno as f64;
        let k = self.variante as f64;

        let angulo = t * (0.14 + 0.012 * k) + 0.37 * k;
        let angulo2 = t * (0.29 + 0.006 * k) + 0.51 * k;
        let amplitud = (2 + self.variante % 4) as f64;

        let valor = match self.familia {
            0 => (t / (6.0 + k)).floor(),

            1 => (t * t / (80.0 + 12.0 * k)).floor(),

            2 => (t * t * t / (600.0 + 150.0 * k)).floor(),

            3 => ((0.55 + 0.10 * k) * t.sqrt()).floor(),

            4 => ((1.2 + 0.2 * k) * (t + 1.0).ln()).floor(),

            5 => ((4.0 + k) * t / (t + 8.0 + k)).round(),

            6 => ((4.0 + k)
                * (1.0 - (-t / (4.0 + k)).exp()))
                .round(),

            7 => (amplitud * angulo.sin()).round(),

            8 => (amplitud * angulo.cos()).round(),

            9 => {
                let periodo = 22 + u64::from(self.variante);

                let r = (
                    u64::from(turno)
                    + 2 * u64::from(self.variante)
                ) % periodo;

                let a = -0.65
                    + 1.30 * r as f64
                    / (periodo - 1) as f64;

                (amplitud * a.tan()).round()
            }

            10 => (t / (9.0 + k)).floor()
                + (3.0 * angulo.sin()).round(),

            11 => (0.65 * t.sqrt()).floor()
                + (amplitud * angulo.cos()).round(),

            12 => (3.0 * angulo.sin()
                + 2.0 * angulo2.cos())
                .round(),

            13 => {
                let periodo = 8 + 2 * u64::from(self.variante);

                let r = (
                    u64::from(turno)
                    + u64::from(self.variante)
                ) % periodo;

                let mitad = (periodo - 1) as f64 / 2.0;

                (5.0
                    - 10.0 * (r as f64 - mitad).abs() / mitad)
                    .round()
            }

            14 => {
                let periodo = 7 + u64::from(self.variante);

                let r = (
                    u64::from(turno)
                    + u64::from(self.variante)
                ) % periodo;

                (r as f64 - (periodo - 1) as f64 / 2.0)
                    .round()
            }

            // funcion de la profe julia sheeeesh
            15 => (t * t - 1.0).tan().abs().round(),

            _ => unreachable!(),
        };

        // conversion a i64 para valores extremos
        valor as i64
    }

    // representacion matematica para q la consola tonta lo lea bien
    fn formula(&self) -> String {
        let k = self.variante as f64;
        let amplitud = 2 + self.variante % 4;

        let a1 = 0.14 + 0.012 * k;
        let b1 = 0.37 * k;
        let a2 = 0.29 + 0.006 * k;
        let b2 = 0.51 * k;

        match self.familia {
            0 => format!(
                "f(t) = ⌊t / {}⌋",
                6 + self.variante
            ),

            1 => format!(
                "f(t) = ⌊t² / {}⌋",
                80 + 12 * self.variante
            ),

            2 => format!(
                "f(t) = ⌊t³ / {}⌋",
                600 + 150 * self.variante
            ),

            3 => format!(
                "f(t) = ⌊{:.2}√t⌋",
                0.55 + 0.10 * k
            ),

            4 => format!(
                "f(t) = ⌊{:.2}·ln(t + 1)⌋",
                1.2 + 0.2 * k
            ),

            5 => format!(
                "f(t) = redondear({}t / (t + {}))",
                4 + self.variante,
                8 + self.variante
            ),

            6 => format!(
                "f(t) = redondear({}·(1 − e^(−t / {})))",
                4 + self.variante,
                4 + self.variante
            ),

            7 => format!(
                "f(t) = redondear({}·sen({a1:.3}t + {b1:.3}))",
                amplitud
            ),

            8 => format!(
                "f(t) = redondear({}·cos({a1:.3}t + {b1:.3}))",
                amplitud
            ),

            9 => {
                let p = 22 + self.variante;

                format!(
                    "r(t) = (t + {}) mod {p}\n\
                     f(t) = redondear({amplitud}·tan(−0.65 + 1.30r(t) / {}))",
                    2 * self.variante,
                    p - 1
                )
            }

            10 => format!(
                "f(t) = ⌊t / {}⌋ + redondear(3·sen({a1:.3}t + {b1:.3}))",
                9 + self.variante
            ),

            11 => format!(
                "f(t) = ⌊0.65√t⌋ + redondear({amplitud}·cos({a1:.3}t + {b1:.3}))"
            ),

            12 => format!(
                "f(t) = redondear(3·sen({a1:.3}t + {b1:.3}) + 2·cos({a2:.3}t + {b2:.3}))"
            ),

            13 => {
                let p = 8 + self.variante * 2;
                let mitad = (p - 1) as f64 / 2.0;

                format!(
                    "r(t) = (t + {}) mod {p}\n\
                     f(t) = redondear(5 − 10·|r(t) − {mitad:.1}| / {mitad:.1})",
                    self.variante
                )
            }

            14 => {
                let p = 7 + self.variante;
                let mitad = (p - 1) as f64 / 2.0;

                format!(
                    "r(t) = (t + {}) mod {p}\n\
                     f(t) = redondear(r(t) − {mitad:.1})",
                    self.variante
                )
            }

            15 => {
                "g(t) = |tan(t² − 1)|\n\
                 f(t) = redondear(g(t))"
                    .to_string()
            }

            _ => unreachable!(),
        }
    }

    // calcula el cambio del ataque
    fn cambio(&self, turno: u32) -> i64 {
        self.evaluar(turno)
            .saturating_sub(self.evaluar(0))
            .clamp(CAMBIO_MINIMO, CAMBIO_MAXIMO)
    }

    // calcual el sheesh
    fn ataque(&self, ataque_inicial: i64, turno: u32) -> i64 {
        ataque_inicial
            .saturating_add(self.cambio(turno))
            .max(1)
    }
}

// info de jugador wwwww speed
struct Jugador {
    nombre: String,
    escuadrones: Vec<u32>,
}

// 151 sheeesh funciones
fn crear_funciones() -> Vec<Funcion> {
    let mut funciones = Vec::with_capacity(151);

    for familia in 0..FAMILIAS_CON_VARIANTES {
        for variante in 1..=VARIANTES {
            funciones.push(Funcion {
                familia,
                variante,
            });
        }
    }

    // funcion julia sheeesh
    funciones.push(Funcion {
        familia: 15,
        variante: 1,
    });

    funciones
}

// sistewma de puntos segun la funcionb aletaroiro
fn calcular_puntos(
    funcion: &Funcion,
    ataque_inicial: i64,
) -> u32 {
    let suma: i64 = (0..TURNOS_ANALISIS)
        .map(|turno| funcion.ataque(ataque_inicial, turno))
        .sum();

    let promedio = suma as f64 / TURNOS_ANALISIS as f64;

    let puntos = (
        100.0 * promedio / ataque_inicial.max(1) as f64
    ).round() as i64;

    puntos.clamp(60, 180) as u32
}

// leer  avlor y validaerlo
fn leer_entero(
    mensaje: &str,
    minimo: u32,
    maximo: u32,
) -> u32 {
    loop {
        print!("{mensaje}");

        io::stdout()
            .flush()
            .expect("Error al imprimir");

        let mut entrada = String::new();

        io::stdin()
            .read_line(&mut entrada)
            .expect("Error de lectura");

        match entrada.trim().parse::<u32>() {
            Ok(numero)
                if (minimo..=maximo).contains(&numero) =>
            {
                return numero;
            }

            _ => {
                println!(
                    "Introduce un número entre {minimo} y {maximo}."
                );
            }
        }
    }
}

// creacion de equipos
fn crear_jugador(nombre: &str, puntos: u32) -> Jugador {
    println!("\nCreando equipo para {nombre}:");

    println!(
        "Tienes {puntos} puntos para repartir entre tus escuadrones."
    );

    let cantidad = leer_entero(
        "¿Cuántos escuadrones quieres crear? ",
        1,
        puntos,
    );

    let mut restantes = puntos;
    let mut escuadrones = Vec::with_capacity(cantidad as usize);

    for i in 0..cantidad {
        let pendientes = cantidad - i - 1;

        // Reservar 1 integrante para cada escuadrón pendiente
        let maximo = restantes - pendientes;

        let miembros = if pendientes == 0 {
            println!(
                "Escuadrón {}: {} integrantes (restantes)",
                i + 1,
                restantes
            );

            restantes
        } else {
            leer_entero(
                &format!(
                    "Integrantes del escuadrón {} (1 a {}): ",
                    i + 1,
                    maximo
                ),
                1,
                maximo,
            )
        };

        escuadrones.push(miembros);
        restantes -= miembros;
    }

    Jugador {
        nombre: nombre.to_string(),
        escuadrones,
    }
}

// mostrar equipos
fn mostrar_jugador(jugador: &Jugador) {
    println!("\n{}:", jugador.nombre);

    for (i, miembros) in jugador.escuadrones.iter().enumerate() {
        println!(
            "  Escuadrón {}: {} integrantes",
            i + 1,
            miembros
        );
    }

    let total: u32 = jugador.escuadrones.iter().sum();

    println!("  TOTAL: {} integrantes", total);
}

fn main() {
    let funciones = crear_funciones();

    let indice = rand::random_range(0..funciones.len());
    let seleccionada = funciones[indice];

    let puntos = calcular_puntos(
        &seleccionada,
        ATAQUE_INICIAL,
    );

    println!("el juego si inicio sheeesh");

    println!("Funciones disponibles: {}", funciones.len());

    println!(
        "\nFUNCIÓN SELECCIONADA: {}",
        seleccionada.nombre()
    );

    println!(
        "Número: {} de {}",
        indice + 1,
        funciones.len()
    );

    println!("Variante: {}", seleccionada.variante);

    println!("{}", seleccionada.formula());

    println!(
        "A(t) = máx(1, {} + limitar(f(t) − f(0), {}, {}))",
        ATAQUE_INICIAL,
        CAMBIO_MINIMO,
        CAMBIO_MAXIMO
    );

    // Mostrar la evolución del ataque
    println!(
        "\nANÁLISIS MATEMÁTICO ({} turnos)",
        TURNOS_ANALISIS
    );

    let mut suma = 0_i64;

    for turno in 0..TURNOS_ANALISIS {
        let ataque = seleccionada.ataque(
            ATAQUE_INICIAL,
            turno,
        );

        suma += ataque;

        println!(
            "Turno {:2} │ Cambio: {:+3} │ Ataque: {:2}",
            turno + 1,
            seleccionada.cambio(turno),
            ataque
        );
    }

    println!(
        "Ataque promedio: {:.2}",
        suma as f64 / TURNOS_ANALISIS as f64
    );

    println!("Puntos por jugador: {puntos}");

 
    let jugador_1 = crear_jugador("Jugador 1", puntos);
    let jugador_2 = crear_jugador("Jugador 2", puntos);

   
    println!("\n equipos ");

    mostrar_jugador(&jugador_1);
    mostrar_jugador(&jugador_2);

    println!(
        "\nEquipos listos. El combate todavia no esta codeado ;(."
    );
}
