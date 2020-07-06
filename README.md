Departamento de informática - UTFSM
Matías Alexander Moreno Pando
201673508-9

# E2_IAA
Repositorio para avances de Inteligencia Artificial Avanzada

## Para compilar y ejecutar el programa
Abrir consola en el mismo directorio del archivo MCPSB_SA.cpp
usar comando "make" para complicar el programa
luego para ejecutar el programa escribir en la misma linea de comandos "./MCPSB_SA " + parametros
Se pueden manipular las semillas, las instancias y diferentes parametros para la ejecucion del programa.
Los parámetros en orden de entrada son:
  int realInstance: 1 para usar instancias reales, 0 para usar instancias de prueba
  int ini = 1 a 6, primera instancia del conjunto a ejecutar
  int fin = 2 a 7, ultima instancia del conjunto a ejecutar
  int iniS = primera semilla del conjunto a ejecutar
  int finS = última semilla del conjunto a ejecutar
  int randLength = largo de lista de las mejores granjas para algoritmo GRASP
  int nRes = Cantidad de reinicios de temperaruta
  int nIt = Cantidad de iteraciones por cada reinicio
  float T0 = Temperatura máxima del sistea
  float alpha = Tasa de decrecimiento de temperatura
  float addP = Probabilidad de seleccionar movimiento Añadir en cada iteración

Ejemplo de linea de ejecución para ejecutar solo la primera instancia de prueba:
./MCPSB_SA 0 1 2 0 5 5 1000 2000 10000 0.995 0.2

Ejemplo de linea de ejecución para ejecutar todas las instancias reales:
./MCPSB_SA 1 1 6 0 5 5 5000 10000 10000 0.9995 0.2