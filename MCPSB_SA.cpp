// Entrega 2 IAA - Matias Moreno - 201673508-9

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h>
#include <chrono>  // for high_resolution_clock
#include <math.h>       /* exp */
using namespace std;

// Random integers generator

#define srand48(x) srand((int)(x))
#define drand48() ((double)rand()/RAND_MAX)

int Seed = 0;
int randLength = 5;
float addP = 0.5;

float float_rand(float a, float b) {
    float retorno = 0;

    if (a < b) {
        retorno = (float) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (float) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

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
    if (retorno == b)
    {
      retorno -= 1;
    }
    return retorno;
}

// Mejor mezcla

void bestBlend(int finalPrize[], int realPrize[], int minPrize[])
{
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
}

void getRealPrize(int realPrize[], vector <int> routes[], int nTrucks, int nQualities, int oProd[], int farmQuality[])
{
  int i, j, load, minQ;
  for (i = 1; i < nTrucks; i++)
  {
    load = 0;
    minQ = 1;
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      load += oProd[routes[i][j]];
      if (farmQuality[ routes[i][j] ] > minQ)
      {
        minQ = farmQuality[ routes[i][j] ];
      }
    }
    realPrize[minQ] += load;
  }
}

void getCapacity(int capacity[], int oCap[], int oProd[], vector <int> routes[], int nTrucks){
  int i, j;
  for (i = 1; i < nTrucks; i++)
  {
    capacity[i] = oCap[i];
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      capacity[i] -= oProd[routes[i][j]];
    }
  }
}
void getProduction(int production[], int oProd[], vector <int> routes[], int nTrucks, int nFarms){
  int i, j;
  for (i = 0; i < nFarms; i++)
  {
    production[i] = oProd[i];
  }
  
  for (i = 1; i < nTrucks; i++)
  {
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      production[routes[i][j]] = 0;
    }
  }
}
void getLoadQuality(int loadQuality[], int farmQuality[], vector <int> routes[], int nTrucks){
  int i, j, minQ;
  for (i = 1; i < nTrucks; i++)
  {
    minQ = 1;
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      if (farmQuality[routes[i][j]] > minQ)
      {
        minQ = farmQuality[routes[i][j]];
      }
    }
    loadQuality[i] = minQ;
  }
}

// Calculo de distancias

void measureDist(int distance[], int nTrucks, vector <int> routes[], int **cost)
{
  int dist, i , j;
  for (i = 1; i < nTrucks; i++)
    {
      dist = 0;
      if (routes[i].size() != 0)
      {
        for (j = 1; j < int(routes[i].size() - 1); j++)
        {
          if (j == 1)
          {
            dist += cost[0][routes[i][1]];
          }
          dist += cost[routes[i][j]][routes[i][j + 1]];
        }
      }
      distance[i] = dist;
    }
}

// Funcion de factibilidad
bool feasible(int realPrize[], int minPrize[])
{
  int av2, av3;
  // Checkear factibilidad en calidad 1
  if (realPrize[1] < minPrize[1])
  {
    return false;
  }
  else
  {
    // Factibilidad en calidad 2
    av2 = realPrize[2] + realPrize[1] - minPrize[1];
    if (av2 < minPrize[2])
    {
      return false;
    }
    else
    {
      // Factibilidad en calidad 3
      av3 = realPrize[3] + av2 - minPrize[2];
      if (av3 < minPrize[3])
      {
        return false;
      }
    }
  }
  return true;
}

// Funcion de evaluación
float eval(int realPrize[], int minPrize[], float profit[], int ** cost, int nTrucks, vector <int> routes[])
{
  float quality = 0;
  int finalPrize[4], diff, i;
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
  for (i = 1; i < 4; i++)
  {
    quality += finalPrize[i] * profit[i];
  }

  for (i = 1; i < nTrucks; i++)
  {
    if (routes[i].size() != 0)
    {
      for (int j = 1; j < int(routes[i].size() - 1); j++)
      {
        if (j == 1)
        {
          quality -= cost[0][routes[i][1]];
        }
        quality -= cost[routes[i][j]][routes[i][j + 1]];
      }
    }
  }
  return quality;
}

// Función miope
void miopeRand(int randLenght, int i, vector<int> iQualityFarms[], int T, int& randFarm, int capacity[], int position[], int production[], float profit[], int **cost){
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
  summary.open("outputs/summary.txt");
  int in;
  for (in = 1; in < 7; in++){
    cout << "Instancia: " << in << endl;
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

    int capacity [nTrucks], oCap [nTrucks], position [nTrucks], loadQuality [nTrucks], production [nFarms], oProd [nFarms], farmQuality [nFarms];
    int **cost;
    cost = new int *[nFarms];
    for(i = 0; i < nFarms; i++)
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
      oCap[i] = stoi(word);
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
      oProd[i] = stoi(word);
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

    vector <int> actualRoutes[nTrucks];

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
      loadQuality[i] = 1;
      actualRoutes[i].push_back(origin);
    }

    // Repetir Función añadir optimo local hasta alcanzar solución inicial factible
    int randFarm, actualPrize [nQualities], actualRealPrize [nQualities], T = 1, excess;
    for (i = 0; i < nQualities; i++)
    {
      actualPrize[i] = 0;
      actualRealPrize[i] = 0;
    }

    for (i = 1; i < nQualities; i++)
    {
      for (; T < nTrucks; T++)
      {

        // Si con el excelente anterior ya llegue al mínimo, salto a la siguiente calidad
        if (minPrize[i] <= actualPrize[i])
        {
          break;
        }

        // Repetir ciclo hasta llenar el camión de producto
        while (true)
        {
          miopeRand(randLength, i, iQualityFarms, T, randFarm, capacity, position, production, profit, cost);
          // Cada uno de estos ciclos termina cuando el camión se llena de producto
          if (randFarm == 0)
          {
            break;
          }
          actualRoutes[T].push_back(randFarm);
          position[T] = randFarm;
          capacity[T] -= production[randFarm];
          loadQuality[T] = i;
          actualPrize[i] += production[randFarm];
          actualRealPrize[i] += production[randFarm];
          production[randFarm] = 0;
        }
        
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

    for (i = 1; i < nTrucks; i++)
    {
      actualRoutes[i].push_back(origin);
    }

    // Beneficio por valor de leche
    int finalPrize[4];
    float actualQuality;

    bestBlend(finalPrize, actualRealPrize, minPrize);
    actualQuality = eval(actualRealPrize, minPrize, profit, cost, nTrucks, actualRoutes);

    // Distancia de transporte
    int distance [nTrucks];
    measureDist(distance, nTrucks, actualRoutes, cost);

    // Escribir solución
        
    ofstream outFile;
    outFile.open ("outputs/" + to_string(in) + ".txt");
    outFile << "** GRASP **\n";
    outFile << actualQuality << " " << finalPrize[1] << " " << finalPrize[2] << " " << finalPrize[3] << " " << nTrucks << endl;
    for (i = 1; i < nTrucks; i++)
    {
      if (actualRoutes[i].size() != 0)
      { 
        outFile << "Truck " << i << " Q" << loadQuality[i] << " ";
        for (j = 0; j < int(actualRoutes[i].size()); j++)
        {
          if (actualRoutes[i][j] == origin)
          {
            if (j == 0)
            {
              outFile << origin << "-";
            }
            else
            {
              outFile << origin << " " << distance[i] << " " << oCap[i] - capacity[i] << endl;
            } 
          }
          else
          {
            outFile << actualRoutes[i][j] << "-";
          } 
        }
      }
    }
    // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    outFile << "Elapsed time: " << elapsed.count() << " s\n";
    
    summary << to_string(in) << " " << nFarms << " " << nTrucks << " " << actualQuality << " " << elapsed.count() << endl; 
     
    // Simulated annealing
    start = std::chrono::high_resolution_clock::now();


    float bestQuality = actualQuality, newQuality;

    // realPrize
    int bestRealPrize [nQualities];
    for (i = 0; i < nQualities; i++)
    {
      bestRealPrize[i] = actualRealPrize[i];
    }
    int newRealPrize [nQualities];
    for (i = 0; i < nQualities; i++)
    {
      newRealPrize[i] = actualRealPrize[i];
    }

    // Rutas
    
    vector <int> bestRoutes[nTrucks];
    for (i = 1; i < nTrucks; i++)
    {
      bestRoutes[i] = actualRoutes[i];
    }
    vector <int> newRoutes[nTrucks];
    for (i = 1; i < nTrucks; i++)
    {
      newRoutes[i] = bestRoutes[i];
    }

    // Iterador
    
    int it0, it, r, rFarm, rTruck, nAvailableF, availableF[nFarms], minDist, dist, minDistPos, minQuality;
    float T0 = 10000, Temp = 10000;
    float p, alpha = 0.995, add;
    bool updt;
    for (it0 = 0; it0 < 50; it0++)
    {
      Temp = T0;
      for (it = 0; it < 1000; it++)
      {
        nAvailableF = 0;
        updt = false;

        // Añadir o Quitar granja
        add = float_rand(0,1);
        if (add > addP)
        {
          //cout << "add\n";
          // Añadir nodo factible
          rTruck = int_rand(1, nTrucks);
          // Generar una lista de granjas factibles para la capacidad de rTruck
          for (i = 1; i < nFarms; i++)
            if (production[i] > 0 && production[i] <= capacity[rTruck])
          {
            {
              availableF[nAvailableF] = i;
              nAvailableF +=1;
            }
          }
          // cout << " add for 1 \n";
          // Si el tamaño de la lista es mayor a 0 escojer una granja al azar
          if (nAvailableF > 0)
          {
            r = int_rand(0, nAvailableF);
            rFarm = availableF[r];
            // Actualizar newRoutes, añadiendo rFarm a la posicion con menor costo
            if (int(actualRoutes[rTruck].size() == 2))
            {
              newRoutes[rTruck].insert(newRoutes[rTruck].begin() + 1, rFarm);
            }
            else
            {
              minDist = cost[ 0 ][ rFarm ] + cost[ rFarm ][ actualRoutes[rTruck][i] ];
              minDistPos = 1;
              // cout << " antes add for 2 \n";
              for (i = 1; i < int(actualRoutes[rTruck].size() - 1); i++)
              {
                if (i == int(actualRoutes[rTruck].size() - 2))
                {
                  dist = cost[ actualRoutes[i][rTruck] ][ rFarm ] + cost[ rFarm ][ 0 ];
                }
                else
                {
                  dist = cost[ actualRoutes[i][rTruck] ][ rFarm ] + cost[ rFarm ][ actualRoutes[rTruck][i+1] ];
                }
                if (dist < minDist)
                {
                  minDist = dist;
                  minDistPos = i+1;
                }
              }
              // cout << " add for 2 \n";
              newRoutes[rTruck].insert(newRoutes[rTruck].begin() + minDistPos, rFarm);
            }
            // Nueva cantidad de recolección por calidad
            getRealPrize(newRealPrize, newRoutes, nTrucks, nQualities, oProd, farmQuality);

            if (feasible(newRealPrize, minPrize))
            {
              updt = true;
            }
            else
            {
              // Restaurar newRealPrize
              for (i = 0; i < nQualities; i++)
              {
                newRealPrize[i] = actualRealPrize[i];
              }
              // Restaurar newRoutes
              newRoutes[rTruck] = actualRoutes[rTruck];
            }
          }
        }
        else
        {
          //cout << "remove\n";
          // Quitar nodo
          rTruck = int_rand(1, nTrucks);
          // La ruta siempre tiene el nodo de origen y destino
          if (actualRoutes[rTruck].size() > 2)
          {
            r = int_rand(1, actualRoutes[rTruck].size() - 1);
            rFarm = actualRoutes[rTruck][r];
            newRoutes[rTruck].erase(newRoutes[rTruck].begin() + r);
            getRealPrize(newRealPrize, newRoutes, nTrucks, nQualities, oProd, farmQuality);
            
            // Factibilidad del cambio
            if (feasible(newRealPrize, minPrize))
            {
              updt = true;
            }
            else
            {
              // Restaurar newRealPrize
              for (i = 0; i < nQualities; i++)
              {
                newRealPrize[i] = actualRealPrize[i];
              }
              // Restaurar newRoutes
              newRoutes[rTruck] = actualRoutes[rTruck];
            }
          }
          //cout << "despues remove\n";
        }
        if (updt)
        {
          //cout << "update\n";
          newQuality = eval(newRealPrize, minPrize, profit, cost, nTrucks, newRoutes);
          p = exp((newQuality - actualQuality)/Temp);
          Temp *= alpha;
          if (p > float_rand(0,1))
          {
            // Actualizar actualQuality, actualRoutes, actualRealPrize
            actualQuality = newQuality;
            actualRoutes[rTruck] = newRoutes[rTruck];
            for (i = 0; i < nQualities; i++)
            {
              actualRealPrize[i] = newRealPrize[i];
            }

            // actualizar production, capacity
            getCapacity(capacity, oCap, oProd, actualRoutes, nTrucks);
            getProduction(production, oProd, actualRoutes, nTrucks, nFarms);
          }
          else
          {
            // Restaurar actualRealPrize
            for (i = 0; i < nQualities; i++)
            {
              newRealPrize[i] = actualRealPrize[i];
            }
            // Restaurar newRoutes
            newRoutes[rTruck] = actualRoutes[rTruck];
          } 

          // Checkear actualquality con bestQuality
          if (bestQuality < actualQuality)
          {
            cout << "Instancia: " << in << ", it0: " << it0 << " bQuality Upd: " << bestQuality << " -> "<< actualQuality << endl;
            bestQuality = actualQuality;
            bestRoutes[rTruck] = actualRoutes[rTruck];
            for (i = 0; i < nQualities; i++)
            {
              bestRealPrize[i] = actualRealPrize[i];
            }
          }
          //cout << "despues update\n";
        }
      }
    }
    
    // Guardar solución en archivo

    //Calcular loadQuality final
    getLoadQuality(loadQuality, farmQuality, actualRoutes, nTrucks);

    //Calcular distancia final
    measureDist(distance, nTrucks, bestRoutes, cost);

    // Calcular la mejor mezcla en planta final
    bestBlend(finalPrize, bestRealPrize, minPrize);

    // Escribir solución
    outFile << "\n** Simulated annealing ** \n";
    outFile << bestQuality << " " << finalPrize[1] << " " << finalPrize[2] << " " << finalPrize[3] << " " << nTrucks << endl;
    for (i = 1; i < nTrucks; i++)
    {
      if (bestRoutes[i].size() != 0)
      { 
        outFile << "Truck " << i << " Q" << loadQuality[i] << " ";
        for (j = 0; j < int(bestRoutes[i].size()); j++)
        {
          if (bestRoutes[i][j] == origin)
          {
            if (j == 0)
            {
              outFile << origin << "-";
            }
            else
            {
              outFile << origin << " " << distance[i] << " " << oCap[i] - capacity[i] << endl;
            } 
          }
          else
          {
            outFile << bestRoutes[i][j] << "-";
          } 
        }
      }
    }

    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    outFile << "Elapsed time: " << elapsed.count() << " s\n";
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

// Para checkear factibilidad se revisa si realPrize puede satisfacer las demandas minimas (feasible)
// Para calcular calidad se encuentra la mejor combinacion de mezcla en planta de realPrize (BestBlend)
// Despues de cada cambio, comparar calidad actual con la mejor encontrada y actualizar
// BestSolQuality, BestRoutes

// Para las instancias pequeñas
// Repetir 1000 veces y tomar tiempo
// Repetir 10000 veces y tomar tiempo
// Repetir 100000 veces y tomar tiempo
