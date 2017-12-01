//CSCI415; Aaron Beyer/Leighton Covington/Brian Engelbrecht, 11/28/2017
//To compile: g++ -std=c++11 -O3 -w PageRankReMapGraph.cpp -o PageRankReMapGraph
//To run: ./PageRankReMapGraph filename minOutgoing
//./PageRankReMapGraph graph8722.txt 5
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

int n;
typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

int main(int argc, char** argv)
{
    if(argc < 3)
    {
      cout << "To run: ./PageRankReMapGraph filename minOutgoing" << endl;
      cout << "./PageRankReMapGraph networkDatasets/toyGraph1.txt 5" << endl;
      return 0;
    }

    fstream myfile(argv[1],std::ios_base::in);
    int minOutgoing = atoi(argv[2]);
    int u,v;
    int maxNode = 0;
    vector<pair<int,int> > allEdges;

    //vector pair to remap i guess
    vector<int> killMe;
    bool existsU, existsV;
    while(myfile >> u >> v)
    {
      
        //Code to re-map graph. Not needed anymore.
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

    n = maxNode +1;
    //Check if minOutgoing is valid
    if(minOutgoing >= n)
    {
        cout << "minOutgoing is too large for the # of nodes!" << endl;
        return 0;
    }

    //Create RANDOM variables  
    srand((unsigned)time(0)); 
    int randIndex;

    // Map nodes that don't point to anything.
    int origSize = allEdges.size();
    int addedSize = origSize;
    int tempTOTALCount = 0;
    vector<int> tempALLPointers;
    bool existingPointer;
    int howManyNew;

    for(int i = 0; i < origSize; i++)
    {
        // Count number of times it points
        for(int j = 0; j < addedSize; j++)
        {
            if(allEdges[i].second == allEdges[j].first)
            {
                tempTOTALCount++;
                tempALLPointers.push_back(allEdges[j].second);
            }          
        }
        
        // If it doesn't point enough times, create more'
        if(tempTOTALCount < minOutgoing)
        {
            howManyNew = minOutgoing - tempTOTALCount;

            for(int m = 0; m < howManyNew; m++)
            {
                existingPointer = true;

                // Keep checking till finds a number not pointed to yet
                while(existingPointer)
                {
                    existingPointer = false;
                    randIndex = (rand() % n);

                    // Check if node already points to it
                    for(int x = 0; x < tempALLPointers.size(); x++)
                    {
                        if(randIndex == tempALLPointers[x])
                        {
                            existingPointer = true;
                            //cout << "[Node: " << allEdges[i].second << "] ALREADY points to " << randIndex << endl;
                        }                      
                    }
                    // Check if it is same value as node
                    if(randIndex == allEdges[i].second)
                    {
                        existingPointer = true;
                        //cout << "[Node: " << allEdges[i].second << "] Cannot point to itself! " << randIndex << endl;
                    }
                }
                //cout << "[Node: " << allEdges[i].second << "] --> " << randIndex << endl;

                allEdges.push_back(make_pair(allEdges[i].second, randIndex));
                tempALLPointers.push_back(randIndex);
                addedSize++;
            }
        }

        tempALLPointers.clear();
        tempTOTALCount = 0;
    }
    

    //OUTPUT GRAPH TO FILE HERE!!!!!
    string file_name = "graph" + std::to_string(killMe.size()) + "Nodes.txt";
    
    ofstream out(file_name.c_str());
    for(int i = 0; i < allEdges.size(); i++)
    {
        out << allEdges[i].first << " " << allEdges[i].second << endl;
    }

    out.close();
    
    cout << "DONE!" << endl << file_name.c_str() << " Created!" << endl;

    return 0;
}
