//CSCI415; Aaron Beyer/Leighton Covington/Brian Engelbrecht, 11/28/2017
//To compile: g++ -std=c++11 -O3 -w PageRank.cpp -lpthread -o PageRank
//To run: ./PageRank filename numLoops numThreads debugMode(0 or 1)
//./PageRank graph8722.txt 5 2 0
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
#include <pthread.h>
#include <mutex>

using namespace std;

const float S_VALUE = 0.85;
const bool debugMode = false;

mutex mtx;
int thread_count;
int n;

// Used to hold data for each thread while multithreading
struct thread_data
{
	int start;
	int end;
};

// Used to hold data for each node in the graph
struct nodeData
{
    float pr;
    int outD;
    int inD;
    vector<int> pointers;
};

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

// Initialize node vectors (called in thread functions)
vector<nodeData> nodes;
vector<float> newPageRanks;

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
  else // Only print pr values
  {
    for (int i = 0; i < n; i++)
    {
        cout << "[Node " << i << ":]     PR:  " << nD[i].pr << endl;
        sum += nD[i].pr;
    }
  }

  // Test if pr values still add up to 1 (WHY NOT)
  cout << "Sum of all PRs: " << sum << endl;
}

// Calculates new Page Rank value for a node
void calculatePageRank(int i)
{
    float sumOfInfluence = 0.0;
    
    for(int k = 0; k < nodes[i].pointers.size(); k++)
    {
        int p = nodes[i].pointers[k];
        sumOfInfluence += (nodes[p].pr/nodes[p].outD);
    }
    
    mtx.lock(); //writing to global variable - must lock
    newPageRanks[i] = (1-S_VALUE)/n + (sumOfInfluence * S_VALUE); 
    mtx.unlock();
}

// Determines what the chunk will do for each row.
void *ProcessChunk(void *threadarg)
{
	struct thread_data *my_data; //struct to hold arguments
	my_data = (struct thread_data *) threadarg;

	//set variables
	int start = my_data->start; // row to begin processing at
	int end = my_data->end; // end processing at this row

	// process rows
    for (int i = start; i < end; i++)
        calculatePageRank(i);
}

int main(int argc, char** argv)
{
    if(argc < 5){
      cout << "To run: ./PageRank filename numLoops numThreads debugMode(0 or 1)" << endl;
      cout << "./PageRank graph8722.txt 5 8 0" << endl;
      return 0;
    }    

    fstream myfile(argv[1],std::ios_base::in);
    int numLoops = atoi(argv[2]);
    thread_count = atoi(argv[3]);
    int debugMode = atoi(argv[4]);
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
    nodes = vector<nodeData>(n);
    newPageRanks = vector<float>(n);
    cout << endl;
    cout << "Graph has " << n << " nodes" << endl << endl;

    adjMatrix = AdjacencyMatrix(n, vector<int>(n));
    // Populate the matrix
    for(int i = 0; i < allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
    }

    // Set nodes vector values
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

    // Initialize multithreading variables
	pthread_t threads[thread_count];
	struct thread_data td[thread_count];
	long thread;

    // Set Chunk size (same for each loop)
    int chunkSize = (n + thread_count - 1) / thread_count; // divide by threads rounded up (works for even AND uneven)

    // Start timer
    //  (only need to time the difference the numThreads has on algorithm)
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    //==============================//
    //      START ITERATIONS        //
    //==============================//
    cout << "Going through iterations..." << endl;
    for(int i = 0; i < numLoops; i++)
    {
        if(debugMode == 1) 
        {
            cout << "=====================" << endl;
            cout << "HELLO!!! I'M LOOP #" << i << "     :D" << endl;
            cout << "=====================" << endl << endl;
        }
        
        
        // (Step 1) Calc new page ranks
        for(thread = 0; thread < thread_count; thread++)
        {
            //set nodes for the thread to process
            int start = thread * chunkSize;
            int end = min(start + chunkSize, n);
            //set arguments
            td[thread].start = start;
            td[thread].end = end;

            pthread_create(&threads[thread], NULL, ProcessChunk, (void *)&td[thread]); 
        }

        /*===================================================================+
        | BLOCK - Bring threads together before setting new Page Rank values |
        |                                                                   */
          for(thread = 0; thread < thread_count; thread++)
              pthread_join(threads[thread], NULL);
        /*                                                                   |
        |                                                                    |
        +===================================================================*/

        // (Step 2) Set new page ranks
        //      (has to be separate from part 1 since nodes[] is used in each loop)      
        //      **We chose not to multithread this part since it only calls one line of code**
        //      **It is much faster with one thread**
        for(int j = 0; j < n; j++)
        {
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

    // Stop clock
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    // Print out result
    cout << endl;
    cout<< "Time: " << elapsed << " s" << endl; 
    cout << endl;

    return 0;
}
