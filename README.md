# E2_IAA
Departamento de informática - UTFSM
Matías Alexander Moreno Pando
201673508-9

## Para compilar y ejecutar el programa
Abrir consola en el mismo directorio del archivo **MCPSB_SA.cpp**. Luego usar comando **make** para compilar el programa.
Para ejecutar el programa escribir en la misma linea de comandos **./MCPSB_SA  + parámetros**
Se pueden manipular las semillas, las instancias y diferentes parametros para la ejecucion del programa en la linea de comando. Los parámetros posibles en orden son:

| Parámetro | Efecto |
| ------ | ------ |
| int **realInstance** | 0 o 1, 1: instancias reales, 0: instancias de prueba |
| int **firstInstance** | 1 a 6, primera instancia del conjunto a ejecutar |
| int **lastInstance** | 2 a 7, ultima instancia del conjunto a ejecutar |
| int **firstSeed** | primera semilla del conjunto a ejecutar |
| int **lastSeed** | última semilla del conjunto a ejecuta |
| int **randLength** | largo de lista de las mejores granjas para algoritmo GRASP |
| int **nResets** | Cantidad de reinicios de temperaruta |
| int **nIterations** | Cantidad de iteraciones por cada reinicio |
| float **T0** | Temperatura máxima del sistema |
| float **alpha** | Tasa de enfriamiento |
| float **addP** | Probabilidad de seleccionar movimiento Añadir en cada iteración |
| float **swapP** | Probabilidad de seleccionar movimiento Swap en cada iteración |

Ejemplo de linea de ejecución para ejecutar solo la primera instancia de prueba:
```sh
$ ./MCPSB_SA 0 1 2 0 5 5 1000 2000 10000 0.995 0.2 0.6
```

Ejemplo de linea de ejecución para ejecutar todas las instancias reales:
```sh
$ ./MCPSB_SA 1 1 6 0 5 5 5000 10000 10000 0.9995 0.2 0.6
```
