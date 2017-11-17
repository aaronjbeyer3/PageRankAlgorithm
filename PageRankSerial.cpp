//CSCI415; Aaron Beyer/Leighton Covington/Brian Engelbrecht, 11/28/2017
//To compile: g++ -O3 -w PageRankSerial.cpp -o PageRankSerial
//To run: ./PageRankSerial filename numLoops
//./PageRankSerial networkDatasets/toyGraph1.txt 5
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

struct nodeData
{
    float pr;
    int outD;
    int inD;
    vector<int> pointers;
};

const float S_VALUE = 0.8;
int n;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

void printVector(vector<nodeData> nD, boolean printAll)
{
  if(printAll)
  {
    for (int i = 0; i < n; i++)
    {
        cout << "[Node " << i << ":]     PR:  " << nD[i].pr << "     outD:  " << nD[i].outD;
        cout << "     pointers: [";
        for(int j = 0; j < nD[i].pointers.size(); j++)
            cout << nD[i].pointers[j] << ", ";
        cout << "]" << endl;
    }
  }
  else //only print pr values
  {
    for (int i = 0; i < n; i++)
    {
        cout << "[Node " << i << ":]     PR:  " << nD[i].pr << endl;
    }
  }

}

int main(int argc, char** argv)
{
    if(argc<3){
      cout<<"To run: ./PageRankSerial filename numLoops"<<endl;
      cout<<"./PageRankSerial networkDatasets/toyGraph1.txt 5"<<endl;
      return 0;
    }
      
    //start timer
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    fstream myfile(argv[1],std::ios_base::in);
    int numLoops = atoi(argv[2]);
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

    //STEPS
    //DONE - get n for directed graph (adjmatrix but only [u][v])
    //DONE - calculate starting PR for each node (1/n)
    //
    //
    //
    //
    
    // initialize nodes
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
    
    // populate pointer vectors for each node
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(adjMatrix[i][j] == 1)
                nodes[j].pointers.push_back(i);
        }
    }
    
    //test print
    cout << "STARTING VALUES" << endl;
    printVector(nodes, true);
    cout << endl;
    
    //===============================================================================
    //START LOOPING HERE
    //===============================================================================
    vector<float> newPageRanks = vector<float>(n);
    for(int i = 0; i < numLoops; i++)
    {
        // calc new page ranks
        for(int i = 0; i < n; i++)
        {
            for(int k = 0; k < nodes[i].pointers.size(); k++){
                int p = nodes[i].pointers[k];
                newPageRanks[i] += (nodes[p].pr/nodes[p].outD);
            }
            
        }
        
        // sets new page rank
        for(int i = 0; i < n; i++){
            nodes[i].pr = newPageRanks[i];
        }
        
        //APPLY S FACTOR
        for(int i = 0; i < n; i++){
            nodes[i].pr = nodes[i].pr*S_VALUE;
        }
        
        //Add residual
        float residual = (1-S_VALUE)/n;
        for(int i = 0; i < n; i++){
            nodes[i].pr += residual;
        }
        
        cout << "=====================" << endl;
        cout << "HELLO!!! I'M LOOP #" << i << "     :D" << endl;
        cout << "=====================" << endl;
        
        //ADD ALGORITHM HERE!!!
        
        
        //print out results of loop
        printVector(nodes, false);
        cout << endl;
    }

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
