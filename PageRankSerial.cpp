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

const float S_VALUE = 0.85;
int n;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

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

    //vector pair to remap i guess
    vector<int> killMe;
    bool existsU, existsV;
    while(myfile >> u >> v)
    {
      existsU = false;
      existsV = false;

      //check to see if it exists
      for(int i = 0; i < killMe.size(); i++)
      {
        if(killMe[i] == u)
        {
          u = i;
          existsU = true;
        }
        else if(killMe[i] == v)
        {
          v = i;
          existsV = true;
        }
      }

      //if u doesn't exist
      if(!existsU)
      {
        killMe.push_back(u);
        u = killMe.size() - 1;           
      }

      //if v doesn't exist
      if(!existsV)
      {
        killMe.push_back(v);   
        v = killMe.size() - 1;          
      }

      allEdges.push_back(make_pair(u,v));
      if(u > maxNode)
        maxNode = u;

      if(v > maxNode)
        maxNode = v;                 
    }

    // Map nodes that don't point to anything.
    //(or else it doesn't work with PageRank, duhhhh)
    int newNode=0;
    int tempSize = allEdges.size();
    bool dirExists;
    for(int i=0; i<tempSize; i++)
    {
        dirExists = false;
        for(int j=0; j<tempSize; j++)
        {
          if(allEdges[i].second == allEdges[j].first)
            dirExists = true;
        }
        
        //if it doesn't exist as a first, make it point to a node
        if(!dirExists)
        {
          allEdges.push_back(make_pair(allEdges[i].second,newNode));
          newNode++;
        }
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


    for(int i=0; i<20; i++)
    {
      cout << "[Node" << i << ":] " << killMe[i] << endl;
    }

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
        cout << "=====================" << endl;
        cout << "HELLO!!! I'M LOOP #" << i << "     :D" << endl;
        cout << "=====================" << endl;
        
        // calc new page ranks
        for(int i = 0; i < n; i++)
        {
            cout << endl;
            cout << "CURRENT NODE: " << i << endl;
            float sumOfInfluence = 0.0;
            
            for(int k = 0; k < nodes[i].pointers.size(); k++){
                int p = nodes[i].pointers[k];
                sumOfInfluence += (nodes[p].pr/nodes[p].outD);
            }
            /*
            cout << "sumOfInfluence: " << sumOfInfluence << endl;
            
            sumOfInfluence *= S_VALUE;
            cout << "sumOfInfluence: " << sumOfInfluence << endl;
            cout << "(1- S_VALUE): " << (1-S_VALUE) << endl;
            
            newPageRanks[i] = (1-S_VALUE)/n +  sumOfInfluence;
            cout << "newPageRank: " << newPageRanks[i] << endl;
              */
            
            newPageRanks[i] = (1-S_VALUE)/n + (sumOfInfluence * S_VALUE);
        }
        
        // sets new page rank
        for(int i = 0; i < n; i++){
            nodes[i].pr = newPageRanks[i];
        }   

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
