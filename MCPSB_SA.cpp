// Entrega 2 IAA - Matias Moreno - 201673508-9

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h>
#include <chrono>  // for high_resolution_clock
using namespace std;

// Random integers generator

#include <stdlib.h>
#define srand48(x) srand((int)(x))
#define drand48() ((double)rand()/RAND_MAX)

int Seed = 0;
int randLength = 5;

void Capture_Params(int argc, char **argv){
    Seed = atoi(argv[1]);
}

int int_rand(int a, int b){
    int retorno = 0;

    if (a < b){
        retorno = (int) ((b - a) * drand48());
        retorno = retorno + a;
    }

    else{
        retorno = (int) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

// Función miope
void miopeRand(int randLenght, int i, vector<int> iQualityFarms[], int T, int& randFarm, int capacity[], int loadQuality[], int position[], int production[], float profit[], int **cost, int minPrize[], int nTrucks, int nFarms, int nQualities){
  int j, k;
  // Busco en las granjas de la calidad actual
  int available = iQualityFarms[i].size(), realAvailable = 0;
  // No quedan mas granjas para visitar
  if (available == 0)
  {
    randFarm = 0;
    return;
  }
  // Obtener las granjas posibles y sus beneficios
  int farmValues[available], availableFarms[available];
  for (j = 0; j < available; j++)
  {
    if ((production[iQualityFarms[i][j]] > 0) && (capacity[T] >= production[iQualityFarms[i][j]]))
    {
      farmValues[realAvailable] = production[iQualityFarms[i][j]] * profit[i] - cost[position[T]][iQualityFarms[i][j]];
      availableFarms[realAvailable] = iQualityFarms[i][j];
      realAvailable++;
    }
  }

  /* cout << "Calidad " << i << ", camion" << T << ", hay " << realAvailable << " granjas disponibles, las cuales son: ";
  for (j = 0; j < realAvailable; j++)
  {
    cout << availableFarms[j] << " ";
  }
  cout << "\n"; */
  
  // Este camion no puede llevar mas carga
  if (realAvailable == 0)
  {
    randFarm = 0;
    return;
  }

  // Generar las mejores randLenght granjas
  int l = (realAvailable < randLenght) ? realAvailable : randLenght; 
  int maxValues[l], pickFarm[l];
  for (j = 0; j < l; j++)
  {
    maxValues[j] = -999999;
  }
  int minValue, minPosition;
  // j son todas las granjas disponibles
  for (j = 0; j < realAvailable; j++)
  {
    minValue = 999999;
    // k son los elementos de la lista de pick
    for (k = 0; k < l; k++)
    {
      // Busco el valor minimo dentro de maxValues
      if (maxValues[k] < minValue)
      {
        minValue = maxValues[k];
        minPosition = k;
      }
    }
    // Si mi farmValues[j] es mayor que el valor minimo, lo reemplazo y guardo la granja en pickFarm
    if (farmValues[j] > minValue)
    {
      maxValues[minPosition] = farmValues[j];
      pickFarm[minPosition] = availableFarms[j];
    }
  }
  // Tomo una granja aleatoria en pickFarm para retornarla
  randFarm = pickFarm[int_rand(0, l)];
}

// int argc, char** argv
// Capture_Params(argc,argv); */
int main()
{
  srand48(Seed);
  ofstream summary;
  summary.open("outputsGR/summary.txt");
  int in;
  for (in = 1; in < 7; in++){
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    ifstream inFile;
    // inFile.open("MCWSB/Real instances/5clusters.dat." + to_string(in);
    inFile.open("MCWSB/Instances/instancia" + to_string(in) + ".mcsb");
    string line;
    string word;
    
    int nFarms, nTrucks, i, j, origin;

    // Obtener nodo origin

    while (inFile >> word)
    {
      if (word == "-")
      {
        inFile >> word;
        origin = stoi(word);
        break;
      }
    }

    // Obtener tamaño de arreglos

    // LLegar hasta predios productores
    while (inFile >> word)
    {
      if (word == "I:=")
      {
        break;
      }
    }

    nFarms = 1;
    // Recorrer predios
    while (inFile >> word)
    {
      nFarms = nFarms + 1;
      size_t found = word.find(";");
      if (found != string::npos) 
      {
        break;
      }
    }

    // LLegar hasta camiones
    while (inFile >> word)
    {
      if (word == "K:=")
      {
        break;
      }
    }

    // Recorrer camiones
    nTrucks = 1;
    while (inFile >> word)
    {
      nTrucks = nTrucks + 1;
      size_t found = word.find(";");
      if (found != string::npos) 
      {
        break;
      }
    }

    int capacity [nTrucks], position [nTrucks], load [nTrucks], loadQuality [nTrucks], production [nFarms], farmQuality [nFarms];
    int **cost;
    cost = new int *[nFarms];
    for(int i = 0; i < nFarms; i++)
    {
      cost[i] = new int[nFarms];
    }

    // LLegar hasta #capacidades
    while (inFile >> word)
    {
      if (word == "param")
      {
        break;
      }
    }

    // Salta 'Q:='
    std::getline(inFile, line);

    // Obtener capacidades
    i = 1;
    inFile >> word;
    while (word != ";")
    {
      // Capacidad
      inFile >> word;
      capacity[i] = stoi(word);
      // N° Camion o ;
      inFile >> word;
      i = i + 1;
    }
    
    // Cantidades a recolectar
    int nQualities = 4;
    int minPrize [nQualities];
    while (inFile >> word)
    {
      if (word == "P:=")
      {
        for (int i = 1; i < nQualities; i++)
        {
          inFile >> word;
          minPrize[i] = stoi(word);
        }
        break;
      }
    }

    // Beneficio por calidad de leche

    float profit [nQualities] = {0, 1, 0.7, 0.3};

    // LLegar hasta #producción
    while (inFile >> word)
    {
      if (word == "param")
      {
        break;
      }
    }
    // Salto qu :=
    std::getline(inFile, line);
    // Salto producción de planta 0
    std::getline(inFile, line);

    // Obtener producciones
    i = 1;
    inFile >> word;
    while (word != ";")
    {
      // Produccion
      inFile >> word;
      production[i] = stoi(word);
      // Calidad
      inFile >> word;
      farmQuality[i] = stoi(word);
      // N° granja ;
      inFile >> word;
      i = i + 1;
    }

    // LLegar hasta #costos :
    while (inFile >> word)
    {
      if (word == ":")
      {
        break;
      }
    }
    
    // Salto primera fila
    std::getline(inFile, line);

    for (i = 0; i < nFarms; i++)
    {
      // Salto la primera palabra de cada fila (La que corresponde al nodo de salida)
      inFile >> word;
      for (j = 0; j < nFarms; j++)
      {
        inFile >> word;
        if (i == j)
        {
          cost[i][j] = 0;
        }
        else
        {
          cost[i][j] = stoi(word);
        }
      }
    }

    inFile.close();

    // Crear nTrucks vectores dinamicos para generar las rutas de cada camion

    vector <int> routes[nTrucks];

    // Crear nQualities vectores dinamicos para agrupar granjas por calidad

    vector <int> iQualityFarms[nQualities];

    for (i = 1; i < nFarms; i++)
    {
      iQualityFarms[farmQuality[i]].push_back(i);
    }

    // Inicializar posiciones, carga y calidad de leche de los camiones en el origen: nodo 0

    for (i = 1; i < nTrucks; i++)
    {
      position[i] = 0;
      load[i] = 0;
      loadQuality[i] = 1;
    }

    // Repetir Función añadir optimo local hasta alcanzar solución inicial factible
    int randFarm, actualPrize [nQualities], realPrize [nQualities], T = 1, excess;
    for (i = 0; i < nQualities; i++)
    {
      actualPrize[i] = 0;
      realPrize[i] = 0;
    }

    for (i = 1; i < nQualities; i++)
    {
      //cout << "Llega calidad " << i << "\n";
      for (T; T < nTrucks; T++)
      {
        //cout << "Llega al camion " << T << "\n";

        // Si con el excelente anterior ya llegue al mínimo, salto a la siguiente calidad
        if (minPrize[i] <= actualPrize[i])
        {
          break;
        }

        // Repetir ciclo hasta llenar el camión de producto
        while (true)
        {
          miopeRand(randLength, i, iQualityFarms, T, randFarm, capacity, loadQuality, position, production, profit, cost, minPrize, nTrucks, nFarms, nQualities);
          //cout << "Para el camion " << T << " se elige la farm " << randFarm << "\n";
          // Cada uno de estos ciclos termina cuando el camión se llena de producto
          if (randFarm == 0)
          {
            break;
          }
          if (routes[T].size() == 0)
          {
            routes[T].push_back(origin);
          }
          routes[T].push_back(randFarm);
          position[T] = randFarm;
          capacity[T] -= production[randFarm];
          load[T] += production[randFarm];
          loadQuality[T] = i;
          actualPrize[i] += production[randFarm];
          realPrize[i] += production[randFarm];
          production[randFarm] = 0;
        }
        //cout << "Calidad 1: minPrize: " << minPrize[i] << ", actualPrize: " << actualPrize[i] << "\n";
        
        // Si terminé de recolectar la cantidad de leche minima de esta calidad
        // lo que sobra se podrá mezclar en planta para la siguiente calidad
        // Luego continua con la siguiente calidad
        // Esto no aplica para la menor calidad de leche
        if (minPrize[i] <= actualPrize[i])
        {
          if (i != (nQualities - 1))
          {
            excess = actualPrize[i] - minPrize[i];
            actualPrize[i] = minPrize[i];
            actualPrize[i+1] += excess;
          }
          T++;
          break;
        }
      }
    }

    // Volver a la planta
    int notEmptyRoutes = 0;

    for (i = 1; i < nTrucks; i++)
    {
      if (routes[i].size() != 0)
      {
        routes[i].push_back(origin);
        notEmptyRoutes++;
      }
    }

    // Calcular beneficio solución
    float solQuality = 0;
    int distance;

    // Beneficio por valor de leche
    cout << "File " << in << "\n";
    cout << "antes (actual): ";
    for (i = 1; i < nQualities; i++)
    {
      cout << actualPrize[i] << " ";
    }
    cout << "\n";
    
    int finalPrize[4];
    int diff;

    finalPrize[3] = realPrize[3];
    finalPrize[2] = realPrize[2];
    finalPrize[1] = realPrize[1];
    if (finalPrize[3] < minPrize[3])
    {
      // Completar la diferencia con leche de calidad 2 y 1 si es necesario
      diff = minPrize[3] - finalPrize[3];
      if (finalPrize[2] >= diff)
      {
        finalPrize[2] -= diff;
      }
      else
      {
        finalPrize[1] -= diff - finalPrize[2];
        finalPrize[2] -= 0;
      }
      finalPrize[3] += diff;
    }

    // Verificar si se cumple la restriccion para la leche 2
    if (finalPrize[2] < minPrize[2])
    {
      // Completar la diferencia con leche de calidad 1
      diff = minPrize[2] - finalPrize[2];
      finalPrize[2] += diff;
      finalPrize[1] -= diff;
    }

    cout << "despues (final): ";
    for (i = 1; i < nQualities; i++)
    {
      cout << finalPrize[i] << " ";
    }
    cout << "\n";

    for (i = 1; i < nQualities; i++)
    {
      solQuality += finalPrize[i] * profit[i];
    }

    // Costo de transporte
    for (i = 1; i < nTrucks; i++)
    {
      distance = 0;
      if (routes[i].size() != 0)
      {
        for (j = 0; j < int(routes[i].size() - 1); j++)
        {
          if (routes[i][j] == origin)
          {
            solQuality -= cost[0][routes[i][j + 1]];
            distance -= cost[0][routes[i][j + 1]];
          }
          else if (routes[i][j - 1] == origin)
          {
            solQuality -= cost[routes[i][j]][0];
            distance += cost[routes[i][j]][0];
          }
          else
          {
            solQuality -= cost[routes[i][j]][routes[i][j + 1]];
            distance += cost[routes[i][j]][routes[i][j + 1]];
          } 
        }
      }
      if (routes[i].size() != 0)
      {
        routes[i].push_back(distance);
      }
    }

    // Printear solución y costo asociado

    ofstream outFile;
    outFile.open ("outputsGR/" + to_string(in) + ".txt");

    outFile << solQuality << " " << finalPrize[1] << " " << finalPrize[2] << " " << finalPrize[3] << " " << nTrucks <<" " << notEmptyRoutes << endl;

    for (i = 1; i < nTrucks; i++)
    {
      if (routes[i].size() != 0)
      { 
        outFile << "Truck " << i << " Q" << loadQuality[i] << " ";
        for (j = 0; j < int(routes[i].size() - 1); j++)
        {
          if (routes[i][j] == origin)
          {
            if (j == 0)
            {
              outFile << origin << "-";
            }
            else
            {
              outFile << origin << " " << routes[i][j + 1] << " " << load[i] << endl;
            } 
          }
          else
          {
            outFile << routes[i][j] << "-";
          } 
        }
      }
    }
    // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    outFile << "Elapsed time: " << elapsed.count() << " s\n";
    
    summary << to_string(in) << " " << nFarms << " " << nTrucks << " " << solQuality << " " << elapsed.count() << endl; 
    outFile.close();
  }
  summary.close();
 return 0;
}
// Trabajar siempre en factibilidad

// Quitar load del calculo en cada paso y calcularlo al final
// Inicializar todos los camiones saliendo y llegando al origen, limpiar las rutas que solo tengan origen al finalizar
// Solo es necesario manejar realPrize para factibilidad y calidad
// No se usa actualPrize en SA y finalPrize solo se usa para calcular la mejor combinación

// Simulated Annealing
// Tomar solución inicial
// Tirar moneda, sacar o añadir

// Sacar: Toma un camion al azar y le quita un nodo al azar
// Si el camion no tiene ruta se repite (while)
// Revisar factibilidad para efectuar cambio
// Se obtiene la diferencia en calidad de solucion
// Si el cambio es positivo se saca de inmediato y se modifica la solucion
// Si el cambio es negativo se evalua con la temperatura
// Al sacar un nodo se modifica la solución
// Se restaura la produccion del nodo, la carga del camion, (la capacidad)
// La ruta del camion se modifica quitando el nodo y conectando los nodos sueltos

// Añadir: Tomar una ruta y añadir un nodo factible (de igual o mayor calidad, respetando capacidad)
// Si el cambio es positivo se añade de inmediato y se modifica la solucion
// Si el cambio es negativo se evalua con la temperatura

// Para checkear factibilidad se revisa si realPrize puede satisfacer las demandas minimas
// Para calcular calidad se encuentra la mejor combinacion de mezcla en planta de realPrize
// Despues de cada cambio, comparar calidad actual con la mejor encontrada y actualizar
// solQuality, routes

// Para las instancias pequeñas
// Repetir 1000 veces y tomar tiempo
// Repetir 10000 veces y tomar tiempo
// Repetir 100000 veces y tomar tiempo
