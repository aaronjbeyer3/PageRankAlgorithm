//CSCI415; Aaron Beyer/Leighton Covington/Brian Engelbrecht, 11/28/2017
//To compile: g++ -std=c++11 -O3 -w PageRankCreateGraph.cpp -o PageRankCreateGraph
//To run: ./PageRankCreateGraph numNodes minOutgoing maxOutgoing
//./PageRankCreateGraph 5000 30 1000
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

int main(int argc, char** argv)
{
    if(argc < 4)
    {
      cout << "To run: ./PageRankCreateGraph numNodes minOutgoing maxOutgoing" << endl;
      cout << "./PageRankCreateGraph 5000 30 1000" << endl;
      return 0;
    }

    int n = atoi(argv[1]);
    int minOutgoing = atoi(argv[2]);
    int maxOutgoing = atoi(argv[3]);
    vector<pair<int,int> > allEdges;

    /*-----------+
    | VALIDATION |
    +-----------*/
    // Check if minOutgoing is valid
    if(minOutgoing >= n)
    {
        cout << "minOutgoing is too large for the # of nodes!" << endl;
        return 0;
    }
    if(minOutgoing <= 0)
    {
        cout << "minOutgoing must be greater than 0!" << endl;
        return 0;
    }
    // Check if maxOutgoing is valid
    if(maxOutgoing >= n)
    {
        cout << "maxOutgoing is too large for the # of nodes!" << endl;
        return 0;
    }
    if(maxOutgoing <= 0)
    {
        cout << "maxOutgoing must be greater than 0!" << endl;
        return 0;
    }
    // Check if maxOutgoing/minOutgoing relationship is valid
    if(maxOutgoing < minOutgoing)
    {
        cout << "maxOutgoing must be equal to or greater than minOutgoing!" << endl;
        return 0;
    }
    /*------------------+
    | END OF VALIDATION |
    +------------------*/

    //Create RANDOM variables  
    srand((unsigned)time(0)); 
    int randOutgoing;
    int randIndex;
    int rangeOutgoing = maxOutgoing - minOutgoing;

    // Map nodes that don't point to anything.
    vector<int> tempALLPointers;
    bool existingPointer;

    // Create pointers for each node
    for(int i = 0; i < n; i++)
    {
        // Randomize number of pointers between min and max
        randOutgoing = (rand() % rangeOutgoing) + minOutgoing;

        for(int j = 0; j < randOutgoing; j++)
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
                        //cout << "[Node: " << i << "] ALREADY points to " << randIndex << endl;
                    }           
                }
                // Check if it is same value as node
                if(randIndex == i)
                {
                    existingPointer = true;
                    //cout << "[Node: " << i << "] Cannot point to itself! " << randIndex << endl;
                }
            }
            //cout << "[Node: " << i << "] --> " << randIndex << endl;

            allEdges.push_back(make_pair(i, randIndex));
            tempALLPointers.push_back(randIndex);
        }

        tempALLPointers.clear();
    }
    

    //OUTPUT GRAPH TO FILE HERE!!!!!
    string file_name = "graph" + std::to_string(n) + "Nodes.txt";
    
    ofstream out(file_name.c_str());
    for(int i = 0; i < allEdges.size(); i++)
    {
        out << allEdges[i].first << " " << allEdges[i].second << endl;
    }

    out.close();
    
    cout << "DONE!" << endl << file_name.c_str() << " Created!" << endl;

    return 0;
}
