# PageRankAlgorithm
Adding multi threading to the Page Rank Algorithm.

How to Run Program:

Serial Version:
- To compile: g++ -std=c++11 -O3 -w PageRankSerial.cpp -o PageRankSerial
- To run: ./PageRankSerial filename numLoops debugMode(0 or 1)
- EX: ./PageRankSerial graph5000Nodes.txt 20 0

Multithreading Version:
- To compile: g++ -std=c++11 -O3 -w PageRank.cpp -lpthread -o PageRank
- To run: ./PageRank filename numLoops numThreads debugMode(0 or 1)
- EX: ./PageRank graph5000Nodes.txt 20 4 0

Proposal Slide Show: https://docs.google.com/presentation/d/1uYDWkE8xOc7U-1w7kA58CF0u0ih58Z-3avtpin58rOg/edit?usp=sharing

Proposal Document: https://docs.google.com/document/d/14MHrv3gs6qIUclV2ZhM2vhJwf-rcphMs9lsKk-HqQLU/edit?usp=sharing

Final Project Report: https://docs.google.com/a/ndsu.edu/document/d/1NIRYaMTqbLcBGAiwltpKe4hZk-od-igYBcOH_vnlY0w/edit?usp=sharing
