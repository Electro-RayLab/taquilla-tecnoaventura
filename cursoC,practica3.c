#include <stdio.h>
/* Taquilla del Parque Tecnoaventura PLus*/

/* Constantes simbólicas (no son estrictamente necesarias pero dan legibilidad) */
#define Lunes 1
#define Martes 2
#define Miercoles 3
#define Jueves 4
#define Viernes 5
#define Sabado 6
#define Domingo 7

#define Efectivo 8
#define Tarjeta  9
#define App      10

#define General 11
#define Plus    12
#define Ultra   13

int main(void){
    int edad,esEstudiante,esDocente,esVip,discapacidad,diaSemana,hora,temporadaAlta,metodoPago,boletos,zona,Zona1,Zona2,Zona3,
        acumuladorDescuentos = 0;

    /* Banderas */
    int banderaInputInvalido = 0,
        banderaRamaVip = 0,
        banderaRamaPreferente = 0,
        banderaRamaPublicogeneral = 0,
        banderaVipMiercoles = 0,
        banderaVipHora = 0,
        banderaVipEstandar = 0,
        banderaDiscapacidad = 0,
        banderaMetodoPago = 0,
        banderaRamaPreferenteMenorEdad = 0,
        banderaRamaPrefenteJueves = 0,
        banderaRamaPreferenteNormal = 0,
        /* ⚠️ En C no se debe usar 'ñ' en identificadores; renombrada a ...Nino */
        banderaRamaPublicogeneralNino = 0,
        banderaRamaPublicoGeneralAnciano = 0,
        banderaCap = 0,
        banderaRamaPublicoGeneralFamilia = 0;

    float total_global = 0.0f, total_boleto = 0.0f, base = 0.0f;

    /* ---------------- ENTRADAS ---------------- */
    printf("Ingrese su edad: ");
    scanf("%i", &edad);

    printf("Es usted un estudiante activo?: (0/1) ");
    scanf("%i", &esEstudiante);

    if (esEstudiante == 0){
        printf("Es usted un Docente activo?: (0/1) ");
        scanf("%i", &esDocente);
    }

    printf("Es usted miembro vip?: (0/1) ");
    scanf("%i", &esVip);

    printf("Tiene usted alguna discapacidad?: (0/1) ");
    scanf("%i", &discapacidad);

    printf("Que dia de la semana es hoy?: 1=Lunes,2=martes,3=miercoles,4=jueves,5=viernes,6=sabado,7=domingo ");
    scanf("%i", &diaSemana);
    if (diaSemana < 1 || diaSemana > 7){
        banderaInputInvalido = 1;
        printf("Por favor, ingrese un numero valido (1-7)\n");
    }

    printf("Que hora del dia es?: (Formato 24 horas) ");
    scanf("%i", &hora);
    if (hora < 0 || hora > 23){
        banderaInputInvalido = 1;
        printf("Por favor, ingrese una hora valida (0-23)\n");
    }

    printf("Actualmente es la temporada alta?: (0/1) ");
    scanf("%i", &temporadaAlta);

    printf("Ingrese su metodo de pago: (8=Efectivo, 9=Tarjeta, 10=App) ");
    scanf("%i", &metodoPago);
    if (metodoPago < 8 || metodoPago > 10){
        banderaInputInvalido = 1;
        printf("Por favor, ingrese un metodo de pago valido (8-10)\n");
    }

    printf("Cuantos boletos desea comprar?: (1-10) ");
    scanf("%i", &boletos);
    if (boletos < 1 || boletos > 10){
        banderaInputInvalido = 1;
        printf("Por favor, ingrese una cantidad de boletos valida (1-10)\n");
    }

    printf("En que zona desea permanecer?: (11=General, 12=Plus, 13=Ultra) ");
    scanf("%i", &zona);
    if (zona < 11 || zona > 13){
        banderaInputInvalido = 1;
        printf("Por favor, ingrese una zona valida (11-13)\n");
    }

    /* Si hubo errores de entrada, cortar el programa */
    if(banderaInputInvalido){
        printf("\n-------------- ERROR DE REGISTRO -------------------\n");
        return 1;
    }

    /* ⚙️ Normalizacion: hiciste #define App=10 pero luego comparas (metodoPago==3).
       Para no tocar tus IF posteriores, transformo 10 -> 3 aqui a modo de compatibilidad. */
    if (metodoPago == App) {
        metodoPago = 3;  /* ahora (metodoPago==3) será verdadero para App */
    }

    /* ---------------- PRECIOS BASE POR TEMPORADA ---------------- */
    if (temporadaAlta == 0){
        Zona1 = 90;
        Zona2 = 120;
        Zona3 = 160;
    } else {
        Zona1 = 110;
        Zona2 = 150;
        Zona3 = 200;
    }

    /* ---------------- LÓGICA PRINCIPAL ---------------- */
    if (esVip == 1){
        banderaRamaVip = 1;

        /* Base por zona */
        if (zona == 11){
            base = Zona1;
        } else if (zona == 12){
            base = Zona2;
        } else {
            base = Zona3;
        }

        /* VIP: miércoles / matinée / estándar -> mutuamente excluyentes */
        if (diaSemana == 3){
            banderaVipMiercoles = 1;
            acumuladorDescuentos = 50;
        } else if (hora < 12){
            banderaVipHora = 1;
            acumuladorDescuentos = 30;
        } else {
            banderaVipEstandar = 1;
            acumuladorDescuentos = 20;
        }

        /* Extras acumulables en VIP */
        if (discapacidad == 1){
            banderaDiscapacidad = 1;
            acumuladorDescuentos += 10;
        }
        if (metodoPago == 3){ /* App normalizada anteriormente */
            banderaMetodoPago = 1;
            acumuladorDescuentos += 5;
        }

        /* Cálculo preliminar (se recalcula abajo si aplica CAP o Gratis) */
        total_boleto  = base - (base * (acumuladorDescuentos/100.0f));
        total_global  = total_boleto * boletos;

    } else if (esEstudiante == 1 || esDocente == 1){
        banderaRamaPreferente = 1;

        /* Base por zona */
        if (zona == 11){
            base = Zona1;
        } else if (zona == 12){
            base = Zona2;
        } else {
            base = Zona3;
        }

        /* Pref: menor de edad vs adulto (y dentro de adulto, jueves vs normal) */
        if (edad < 18){
            banderaRamaPreferenteMenorEdad = 1;
            acumuladorDescuentos = 15;
        } else {
            if (diaSemana == 4){
                banderaRamaPrefenteJueves = 1;
                acumuladorDescuentos = 20;
            } else {
                banderaRamaPreferenteNormal = 1;
                acumuladorDescuentos = 10;
            }
        }

        /* Extras acumulables en Preferente (dejé 10% y 5% como tú pusiste) */
        if (discapacidad == 1){
            banderaDiscapacidad = 1;
            acumuladorDescuentos += 10;
        }
        if (metodoPago == 3){
            banderaMetodoPago = 1;
            acumuladorDescuentos += 5;
        }

        total_boleto  = base - (base * (acumuladorDescuentos/100.0f));
        total_global  = total_boleto * boletos;

    } else {
        banderaRamaPublicogeneral = 1;

        /* Base por zona */
        if (zona == 11){
            base = Zona1;
        } else if (zona == 12){
            base = Zona2;
        } else {
            base = Zona3;
        }

        /* General: niños <4 gratis; 5-12 sin descuento; adultos con 3a edad */
        if (edad < 4){
            banderaRamaPublicogeneralNino = 1;
            base = 0.0f;             /* Gratis */
            acumuladorDescuentos = 0;/* Ignoramos descuentos cuando es gratis */
        } else if (edad <= 12){
            /* Sin descuento por edad */
            /* base = base;  <- redundante, se deja para claridad */
        } else {
            if (edad >= 60){
                banderaRamaPublicoGeneralAnciano = 1;
                acumuladorDescuentos = 40;
            } else {
                /* 0% por edad adulta sin 3a edad */
                /* acumuladorDescuentos = acumuladorDescuentos;  <- redundante */
            }
        }

        /* Extras acumulables SOLO si no es gratis */
        if (!banderaRamaPublicogeneralNino){
            if (discapacidad == 1){
                banderaDiscapacidad = 1;
                acumuladorDescuentos += 10;
            }
            if (boletos >= 4){
                banderaRamaPublicoGeneralFamilia = 1; /* marca combo familia */
                acumuladorDescuentos += 12;
            }
            if (metodoPago == 3){
                banderaMetodoPago = 1;
                acumuladorDescuentos += 5;
            }
        }

        total_boleto  = base - (base * (acumuladorDescuentos/100.0f));
        total_global  = total_boleto * boletos;
    }

    /* ---------------- POST-PROCESO GLOBAL ---------------- */

    /* A) NIÑOS GRATIS: no combinan con descuentos (ya forzamos base=0 y pct=0) */
    if (banderaRamaPublicogeneralNino){
        total_boleto = 0.0f;
        total_global = 0.0f;
        acumuladorDescuentos = 0; /* por coherencia con el resumen */
    }
    else {
        /* B) CAP del 65%: si la suma supera 65, recalcule totales con 65% */
        if (acumuladorDescuentos > 65){
            banderaCap = 1;
            acumuladorDescuentos = 65;  /* aplica tope */
            total_boleto  = base - (base * (acumuladorDescuentos/100.0f));
            total_global  = total_boleto * boletos;
        }
    }

    /* ---------------- SALIDA / RESUMEN ---------------- */
    printf("\n================= RESUMEN =================\n");

    /* Rama usada */
    if (banderaRamaVip)               printf("Rama: VIP\n");
    else if (banderaRamaPreferente)   printf("Rama: Preferente\n");
    else                              printf("Rama: Publico General\n");

    /* Datos base */
    printf("Boletos: %d\n", boletos);
    printf("Precio base por boleto: $%.2f\n", base);

    /* Desglose de descuentos aplicados */
    if (banderaRamaPublicogeneralNino){
        printf("\nEntrada GRATIS (edad < 4). No se aplican otros descuentos.\n");
    } else {
        printf("\nDescuentos aplicados:\n");
        if (banderaVipMiercoles)             printf("- VIP Miercoles: 50%%\n");
        if (banderaVipHora)                  printf("- VIP Matinee (hora<12): 30%%\n");
        if (banderaVipEstandar)              printf("- VIP Estandar: 20%%\n");

        if (banderaRamaPreferenteMenorEdad)  printf("- Preferente menor de edad: 15%%\n");
        if (banderaRamaPrefenteJueves)       printf("- Preferente Jueves: 20%%\n");
        if (banderaRamaPreferenteNormal)     printf("- Preferente adulto (no Jueves): 10%%\n");

        if (banderaRamaPublicoGeneralAnciano)printf("- General tercera edad: 40%%\n");
        if (banderaRamaPublicoGeneralFamilia)printf("- General Combo Familia (>=4 boletos): 12%%\n");

        if (banderaDiscapacidad) {
            /* Nota: tú usaste +10%% en todas las ramas; se reporta así para coincidir con tu logica */
            printf("- Discapacidad: 10%%\n");
        }
        if (banderaMetodoPago) {
            /* Nota: tú usaste +5%% para App en todas las ramas; se reporta así para coincidir */
            printf("- Pago via App: 5%%\n");
        }

        if (banderaCap) printf("** Se aplico tope maximo de 65%% **\n");
        printf("Porcentaje total aplicado: %d%%\n", acumuladorDescuentos);
    }

    /* Totales finales */
    printf("\nTotal por boleto: $%.2f\n", total_boleto);
    printf("Total global (%d boletos): $%.2f\n", boletos, total_global);
    printf("==========================================\n");

    return 0;
}

