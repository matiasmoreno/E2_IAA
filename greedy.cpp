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

int Seed;
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
    if (capacity[T] >= production[iQualityFarms[i][j]])
    {
      farmValues[realAvailable] = production[iQualityFarms[i][j]] * profit[i] - cost[position[T]][iQualityFarms[i][j]];
      availableFarms[realAvailable] = iQualityFarms[i][j];
      realAvailable++;
    }
  }

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
      // Si mi farmValues[j] es mayor que el valor minimo, lo reemplazo y guardo la granja en pickFarm
      if (farmValues[j] > minValue)
      {
        maxValues[minPosition] = farmValues[j];
        pickFarm[minPosition] = availableFarms[j];
      }
    }
  }
  // Tomo una granja aleatoria en pickFarm para retornarla
  randFarm = pickFarm[int_rand(0, realAvailable)];
}

int main(int argc, char** argv)
{
  Capture_Params(argc,argv);
  srand48(Seed);

  ofstream summary;
  summary.open("outputs/summary.txt");
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

    for (i = 0; i < nFarms; i++)
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

    cout << loadQuality[1];

    // Repetir Función añadir optimo local hasta alcanzar solución inicial factible
    int randFarm, actualPrize [nQualities] = {0, 0, 0, 0};
    int T = 1;
    for (i = 1; i < nQualities; i++)
    {
      for (T; T < nTrucks; T++)
      {
        miopeRand(randLength, i, iQualityFarms, T, randFarm, capacity, loadQuality, position, production, profit, cost, minPrize, nTrucks, nFarms, nQualities);
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
        actualPrize[i] += production[randFarm];
        production[randFarm] = 0;
        loadQuality[T] = i;
        if (minPrize <= actualPrize)
        {
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

    for (i = 1; i < nQualities; i++)
    {
      solQuality += actualPrize[i] * profit[i];
    }

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
    outFile.open ("outputs/" + to_string(in) + ".txt");

    outFile << solQuality << " " << actualPrize[1] << " " << actualPrize[2] << " " << actualPrize[3] << " " << notEmptyRoutes << endl;

    for (i = 1; i < nTrucks; i++)
    {
      if (routes[i].size() != 0)
      { 
        outFile << "Truck " << i << " ";
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
