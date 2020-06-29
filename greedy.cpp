// Entrega 1 IAA - Matias Moreno - 201673508-9

#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <chrono>  // for high_resolution_clock

using namespace std;
void miope(int& bestTruck, int& bestFarm, int capacity[], int position[], int production[], int **cost, float minPrize, int nTrucks, int nFarms, float avg, float p, int actualPrize){
 float maxValue = -10000;
 float value = 0;
 for (int i = 1; i < nTrucks; i++)
 {
  for (int j = 1; j < nFarms; j++)
  {
   if (cost[position[i]][j] != 0){
                if (production[j] > 0)
                {
                    if ((capacity[i] - production[j] - avg*p) > 0)
                    {
                        if ((float(actualPrize) + float(production[j])) > minPrize)
                        {
                            value = (float(minPrize) - float(actualPrize)) / ((float(cost[position[i]][j]) + float(cost[j][0])));
                        }
                        else
                        {
                            value = float(production[j]) / ((float(cost[position[i]][j]) + float(cost[j][0])));
                        }
                        if (maxValue < value)
                        {
                            maxValue = value;
                            bestTruck = i;
                            bestFarm = j;
                        }
                    }
                    else
                    {
                        if (float(actualPrize + production[j]) > minPrize)
                        {
                            value = (float(minPrize) - float(actualPrize)) / float(cost[position[i]][j]);
                        }
                        else
                        {
                            value = float(production[j]) / float(cost[position[i]][j]);
                        }
                        if (maxValue < value)
                        {
                            maxValue = value;
                            bestTruck = i;
                            bestFarm = j;
                        }
                    }
                }
   }
  }
 }
}

int main()
{
        ofstream summary;
        summary.open("outputs/summary.txt");
        int in;
        for (in = 1; in < 7; in++){
            // Record start time
            auto start = std::chrono::high_resolution_clock::now();

            ifstream inFile;
            string instance;
            instance = "5clusters.dat." + to_string(in);
            inFile.open("MCWSB/Instances/" + instance + ".dat");
            string line;
            string word;
            
            int nFarms, nTrucks, i, j, origin;
            float minPrize;

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

            int capacity [nTrucks], position [nTrucks], load [nTrucks], production [nFarms];
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
            getline(inFile, line);

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
            
            // Cantidad a recolectar

            while (inFile >> word)
            {
                if (word == "P:=")
                {
                    inFile >> word;
                    minPrize = stof(word);
                    break;
                }
            }
            // LLegar hasta #producción
            while (inFile >> word)
            {
                if (word == "param")
                {
                    break;
                }
            }
            // Salto qu :=
            getline(inFile, line);
            // Salto producción de planta 0
            getline(inFile, line);
            
            int m = 0;
            int sum = 0;
            // Obtener producciones
            i = 1;
            inFile >> word;
            while (word != ";")
            {
                // Produccion
                inFile >> word;
                production[i] = stoi(word);
                sum = sum + stoi(word);
                // N° granja ;
                inFile >> word;
                m++;
                i = i + 1;
            }
            float avg = float(sum) / float (m);
            
            // LLegar hasta #costos :
            while (inFile >> word)
            {
                if (word == ":")
                {
                    break;
                }
            }
            
            // Salto primera fila
            getline(inFile, line);

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

            // Crear 3 vectores dinamicos para generar las rutas

            vector <int> routes[nTrucks];

            // Inicializar posiciones y carga de los camiones en el origen: nodo 0

            for (i = 1; i < nTrucks; i++)
            {
                position[i] = 0;
                load[i] = 0;
            }

            // Repetir Función añadir optimo local hasta alcanzar minimo de producción
            int bestTruck = 0, bestFarm = 0, actualPrize = 0;
            while (actualPrize < minPrize)
            {
                miope(bestTruck, bestFarm, capacity, position, production, cost, minPrize, nTrucks, nFarms, avg, p, actualPrize);
                if (routes[bestTruck].size() == 0)
                {
                    routes[bestTruck].push_back(origin);
                }
                routes[bestTruck].push_back(bestFarm);
                position[bestTruck] = bestFarm;
                capacity[bestTruck] = capacity[bestTruck] - production[bestFarm];
                load[bestTruck] = load[bestTruck] + production[bestFarm];
                actualPrize = actualPrize + production[bestFarm];
                production[bestFarm] = 0;
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

            // Calcular costo solución

            int costRoutes = 0;
            int distance;

            for (i = 1; i < nTrucks; i++)
            {
                distance = 0;
                if (routes[i].size() != 0)
                {
                    for (j = 0; j < int(routes[i].size() - 1); j++)
                    {
                        if (routes[i][j] == origin)
                        {
                            costRoutes = costRoutes + cost[0][routes[i][j + 1]];
                            distance = distance + cost[0][routes[i][j + 1]];
                        }
                        else if (routes[i][j + 1] == origin)
                        {
                            costRoutes = costRoutes + cost[routes[i][j]][0];
                            distance = distance + cost[routes[i][j]][0];
                        }
                        else
                        {
                            costRoutes = costRoutes + cost[routes[i][j]][routes[i][j + 1]];
                            distance = distance + cost[routes[i][j]][routes[i][j + 1]];
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
            outFile.open ("outputsImproved/" + instance + ".txt");

            outFile << costRoutes << " " << actualPrize << " " << notEmptyRoutes << endl;
            // cout << costRoutes << " " << actualPrize << " " << notEmptyRoutes << endl;

            for (i = 1; i < nTrucks; i++)
            {
                if (routes[i].size() != 0)
                { 
                    // cout << "Truck " << i << " ";
                    outFile << "Truck " << i << " ";
                    for (j = 0; j < int(routes[i].size() - 1); j++)
                    {
                        if (routes[i][j] == origin)
                        {
                            if (j == 0)
                            {
                                // cout << origin << "-";
                                outFile << origin << "-";
                            }
                            else
                            {
                                // cout << origin << " " << routes[i][j + 1] << " " << load[i] << endl;
                                outFile << origin << " " << routes[i][j + 1] << " " << load[i] << endl;
                            } 
                        }
                        else
                        {
                            // cout << routes[i][j] << "-";
                            outFile << routes[i][j] << "-";
                        } 
                    }
                }
            }
            // Record end time
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            // std::cout << "Elapsed time: " << elapsed.count() << " s\n";
            outFile << "Elapsed time: " << elapsed.count() << " s\n";
            
            summary << instance << " " << nFarms << " " << nTrucks << " " << costRoutes << " " << elapsed.count() << endl; 
            outFile.close();
        }
        summary.close();
    }
 return 0;
}
