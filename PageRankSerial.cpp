//CSCI415; Aaron Beyer/Leighton Covington/Brian Engelbrecht, 11/28/2017
//To compile: g++ -std=c++11 -O3 -w PageRankSerial.cpp -o PageRankSerial
//To run: ./PageRankSerial filename numLoops debugMode(0 or 1)
//./PageRankSerial graph5000Nodes.txt 20 0
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

// Used to hold data for each node in the graph
struct nodeData
{
    float pr;
    int outD;
    int inD;
    vector<int> pointers;
};

const float S_VALUE = 0.85;
int n;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

// Prints out value of nodes vector for each node
void printVector(vector<nodeData> nD, bool printAll)
{
  float sum = 0.0;
  if(printAll)
  {
    for (int i = 0; i < n; i++)
    {
        cout << "[Node " << i << ":]     PR:  " << nD[i].pr << "     outD:  " << nD[i].outD;
        cout << "     pointers: [";
        for(int j = 0; j < nD[i].pointers.size(); j++)
            cout << nD[i].pointers[j] << ", ";
        cout << "]" << endl;
        sum += nD[i].pr;
    }
  }
  else //only print pr values
  {
    for (int i = 0; i < n; i++)
    {
        cout << "[Node " << i << ":]     PR:  " << nD[i].pr << endl;
        sum += nD[i].pr;
    }
  }

  //test if pr values still add up to 1 (WHY NOT)
  cout << "Sum of all PRs: " << sum << endl;

}

int main(int argc, char** argv)
{
    if(argc < 4){
      cout << "To run: ./PageRankSerial filename numLoops debugMode(0 or 1)" << endl;
      cout << "./PageRankSerial graph5000Nodes.txt 20 0" << endl;
      return 0;
    }
      
    //start timer
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    fstream myfile(argv[1],std::ios_base::in);
    int numLoops = atoi(argv[2]);
    int debugMode = atoi(argv[3]);
    int u,v;
    int maxNode = 0;
    vector<pair<int,int> > allEdges;

    while(myfile >> u >> v)
    {     
      allEdges.push_back(make_pair(u,v));
      if(u > maxNode)
        maxNode = u;

      if(v > maxNode)
        maxNode = v;                 
    }

    n = maxNode +1;  //Since nodes starts with 0
    cout << endl;
    cout << "Graph has " << n << " nodes" << endl << endl;

    adjMatrix = AdjacencyMatrix(n, vector<int>(n));
    //populate the matrix
    for(int i = 0; i < allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       //adjMatrix[v][u] = 1; directed graph so this line removed
    }

    // Set nodes vector values
    vector<nodeData> nodes = vector<nodeData>(n);
    for(int i = 0; i < n; i++)
    {
        int d = 0;
        for(int j = 0; j < n; j++)
            if(adjMatrix[i][j] == 1)
                d++;
        
        struct nodeData temp;
        temp.pr = 1.0/n;
        temp.outD = d;
        nodes[i] = temp;
    }
    
    // Populate pointer vectors for each node
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(adjMatrix[i][j] == 1)
                nodes[j].pointers.push_back(i);
        }
    }
    
    // Print out starting values
    if(debugMode == 1)
    {
        cout << "STARTING VALUES" << endl;
        printVector(nodes, true);
        cout << endl;
    }

    //==============================//
    //      START ITERATIONS        //
    //==============================//
    vector<float> newPageRanks = vector<float>(n);
    cout << "Going through iterations..." << endl;
    for(int i = 0; i < numLoops; i++)
    {
        if(debugMode == 1)
        {
            cout << "=====================" << endl;
            cout << "HELLO!!! I'M LOOP #" << i << "     :D" << endl;
            cout << "=====================" << endl << endl;
        }
        
        // calc new page ranks
        for(int j = 0; j < n; j++)
        {
            float sumOfInfluence = 0.0;
            
            for(int k = 0; k < nodes[j].pointers.size(); k++){
                int p = nodes[j].pointers[k];
                sumOfInfluence += (nodes[p].pr/nodes[p].outD);
            }
            
            newPageRanks[j] = (1-S_VALUE)/n + (sumOfInfluence * S_VALUE);
        }
        
        // sets new page rank
        for(int j = 0; j < n; j++){
            nodes[j].pr = newPageRanks[j];
        }   

        //print out results of loop
        if(debugMode == 1)
        {
            printVector(nodes, false);
            cout << endl;
        }      
    }
    cout << "Done!" << endl;

    //stop clock
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    //Print out result
    cout << endl;
    cout<< "Time: " << elapsed << " s" << endl; 
    cout << endl;

    return 0;
}
