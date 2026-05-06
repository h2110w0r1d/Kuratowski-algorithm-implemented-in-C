# Kuratowski-algorithm-implemented-in-C
This project implemented Kuratowski algorithm in C language during a Sophomore undergraduate summer project in 2022

The following algorithms are used:
1. Tarjan SCC
2. [Johnson’s algorithm](https://www.cs.tufts.edu/comp/150GA/homeworks/hw1/Johnson%2075.PDF)
3. converting a directed graph to an undirected graph by BFS (made it by myself as an exercise)
4. [A planarity algorithm based on the Kuratowski theorem](https://dl.acm.org/doi/10.1145/1476936.1476957)

The project was developed using Code::Blocks.

Please input one graph each time. The vertex of the imported graph should start from 0. The total number of vertexes should be no more than 20. Otherwise, the total number of vertexes allowed can be modified by changing the value of variable - ‘MAX’ in AdjLisr.h, but the total number of cycles needs to be stored may increase correspondingly (adjust the variable – ‘MAX2’  in JohnsonCycleAlgo.h to adjust the number of cycles can be stored)


‘MAX’ = the total number of vertexes allowed 

‘MAX2’ = the total number of cycles needs to be stored

The program output the planarity of a graph, or a subgraph which is subdivision to K3,3 or K5 if the graph is non-planar, the vertex of the subgraph is renumbered when the program is running and starts from 1.

i.e. 

Original vertex	0	1	2	…

Renumbered vertex	1	2	3	…

