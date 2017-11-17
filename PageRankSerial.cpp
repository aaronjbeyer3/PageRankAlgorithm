//CSCI415; Aaron Beyer/Leighton Covington/Brian Engelbrecht, 11/28/2017
//To compile: g++ -O3 -w PageRankSerial.cpp -o PageRankSerial
//To run: ./PageRankSerial filename
//./PageRankSerial networkDatasets/toyGraph1.txt
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
}

final float S_VALUE = 0.8;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

int main(int argc, char** argv)
{
    if(argc<2){
      cout<<"To run: ./PageRankSerial filename"<<endl;
      cout<<"./PageRankSerial networkDatasets/toyGraph1.txt"<<endl;
      return 0;
    }
      
    //start timer
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    fstream myfile(argv[1],std::ios_base::in);
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

    int n = maxNode +1;  //Since nodes starts with 0
    cout<<endl;
    cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(n,vector<int>(n));
    //populate the matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       //adjMatrix[v][u] = 1; directed graph so this line removed
    }



    // ADD CODE HERE

    //HELLO MY NAME IS AARON
    
    //Hello my name is Brian

    //STEPS
    //DONE - get n for directed graph (adjmatrix but only [u][v])
    //DONE - calculate starting PR for each node (1/n)
    //
    //
    //
    //
    
    vector<nodeData> nodes;
    for(int i=0; i<n; i++)
    {
        struct nodeData temp;
        temp->pr = 1.0/n;
        temp->outD = i;
        nodes.push_back(temp);
    }
    
    for (int i=0; i<n; i++)
    {
        cout << "Node " << i << ": " << nodes[i].outD << endl;
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
