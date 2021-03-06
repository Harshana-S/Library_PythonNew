
#include "Snap.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


static float  min_dist=1000000000, max_dist=-100000000000;

float ThshldArray[50] = {
0.0,
2082.47,
8329.86,
18742.19,
33319.45,
52061.64,
74968.76,
102040.82,
133277.80,
168679.72,
208246.56,
251978.34,
299875.05,
351936.69,
408163.27,
468554.77,
533111.20,
601832.57,
674718.87,
751770.10,
832986.26,
918367.35,
1007913.37,
1101624.32,
1199500.21,
1301541.02,
1407746.77,
1518117.45,
1632653.06,
1751353.60,
1874219.08,
2001249.48,
2132444.81,
2267805.08,
2407330.28,
2551020.41,
2698875.47,
2850895.46,
3007080.38,
3167430.24,
3331945.02,
3500624.74,
3673469.39,
3850478.97,
4031653.48,
4216992.92,
4406497.29,
4600166.60,
4798000.83,
5000000.00
};


  
int createPatternGraphPerFile(char *fileName, PUNGraph Graph, double dist_min, double dist_max)
{
	FILE *fp2;
	long long int ind1, ind2;
	int ii=0;
	float dist;

	fp2 = fopen(fileName, "r");
        if (fp2==NULL)
        {
            printf("Error opening file %s\n", fileName);
        }
        else
        {
            while(fscanf(fp2, "%lld\t%lld\t%f\n",&ind1,&ind2, &dist)!=EOF)
            {
                if ((dist >= dist_max)||(dist <= dist_min))
                {
                    continue;    
                }
                if (!Graph->IsNode(ind1)) Graph->AddNode(ind1);
                if (!Graph->IsNode(ind2)) Graph->AddNode(ind2);
                if (!Graph->IsEdge(ind1, ind2)) Graph->AddEdge(ind1, ind2);

            }
            fclose(fp2);
        }
        
     return 1;
  
}

int createPatternGraphPerCollection(char *listFile, char *patternKNNExt, PUNGraph Graph, double dist_min, double dist_max)
{
    FILE *fp1;
    int ii=0;
    char  tempFilename[400]= {'\0'},  patternKNNFile[400]= {'\0'};

    fp1 = fopen(listFile, "r");
	if (fp1==NULL)
    {
        printf("Error opening file %s\n", listFile);
        return 0;
    }
        
    while(fgets(tempFilename, 400, fp1))
    {
        sscanf(tempFilename, "%[^\n]s\n", patternKNNFile);
        
        strcat(patternKNNFile,patternKNNExt);
        
        printf("processing file %d\t%s\n", ii+1,patternKNNFile);
	    createPatternGraphPerFile(patternKNNFile, Graph, dist_min, dist_max);
        memset(tempFilename, '\0', sizeof(char)*400);
        memset(patternKNNFile, '\0', sizeof(char)*400);
        ii++;
    }
    fclose(fp1);
    
    return 1;
 
}
/*
 * THis function randomizes a network. This was needed to show how a current network clusters better compared to a random network.
 * NOTE that during the process of randomization we preserve several network topological characteristics
 * we preserve, number of notes, number of edges, in and out degree of every node.
 */
int randomizeGraph(PUNGraph Graph, int NSwapsMultiple)
{
    long int nNodes = Graph->GetNodes();
    long int nSwaps = NSwapsMultiple*nNodes;
    long int cnt = nSwaps, nChanges=0;
    int nId1, nId2, nId1nn, nId2nn, nn1, nn2, rnn1, rnn2;
    srand (time(NULL));
    TUNGraph::TNodeI node1, node2, node1nn, node2nn;
    //printf("Hello1\n");
    while(cnt >0)
    {
        nChanges++;
        //printf("-----------------------CNT %d----------------------------\n",cnt);
        //get a random note
        nId1 = Graph->GetRndNId();
        nId2 = Graph->GetRndNId();
        //printf("nodes are %d and %d\n",nId1, nId2);
        //Check if there is a link between these two nodes, if yes, just skip the following steps
        if(Graph->IsEdge(nId1, nId2)){continue;}
        
        
        node1 = Graph->GetNI(nId1);
        nn1 = node1.GetOutDeg();
        //printf("Out degree1 %d\n",nn1);
        rnn1 = rand()%nn1;
        //printf("random number 1 = %d\n",rnn1);
        nId1nn = node1.GetNbrNId(rnn1);
        //printf("Hello3\n");
        if(Graph->IsEdge(nId1nn, nId2)){continue;}
        
        node2 = Graph->GetNI(nId2);
        nn2 = node2.GetOutDeg();
        //printf("Out degree2 %d\n",nn2);
        rnn2 = rand()%nn2;
        //printf("random number 2 = %d\n",rnn2);
        nId2nn = node2.GetNbrNId(rnn2);
        //printf("Hello4\n");
        if(Graph->IsEdge(nId1, nId2nn)){continue;}
        
        if(Graph->IsEdge(nId1nn, nId2nn)){continue;}
        
        //printf("Hello5\n");
        Graph->AddEdge(nId1, nId2);
        Graph->AddEdge(nId1nn, nId2nn);
        
        //printf("Hello6\n");
        
        Graph->DelEdge(nId1, nId1nn);
        Graph->DelEdge(nId2, nId2nn);
        //rintf("Hello7\n");
        
        cnt--;
    }
    //rintf("total iterations %d\n",nChanges);
    
    return 1;
}


/*int computeClusterCoffCurve(char *listFile, char *patternKNNExt, , char *ClusterCoffFile)
{

}*/

int main(int argc, char* argv[])
{
    FILE *fp1, *fp2;
    char *listFile, *patternKNNExt, *config_id tempFilename[400]= {'\0'}, *outputNetworkFile, patternKNNFile[400]= {'\0'}, outFileName[400]= {'\0'}, outInfoFile[400]= {'\0'};;
    int verbos;
    int ii=0, jj=0;
    long long int ind1, ind2;
    float dist;
    double clusterCoff;
    int thresholdBin;

    PUNGraph Graph = TUNGraph::New();


    if(argc < 6 || argc > 7)
    {
        printf("\nInvalid number of arguments!!!\n");
        exit(1);
    }

    patternKNNExt = argv[1];
    listFile = argv[2];
    outputNetworkFile = argv[3];
    thresholdBin = atoi(argv[4]);
    config_id = atoi(argv[5]);
    if( argc == 7 ){verbos = atoi(argv[6]);}

    //Generating graph based on the distances between the patterns
    createPatternGraphPerCollection(listFile, patternKNNExt, Graph, 0, ThshldArray[thresholdBin-1]);

    //computing the clustering coffiecient of the generated graph
    clusterCoff = TSnap::GetClustCf(Graph);
    
    //dumping the computed clustering coefficient into an file
    sprintf(outFileName, "%d_ClusteringCoff_%s.txt", thresholdBin, config_id);
    fp2 = fopen(outFileName, "w");
    fprintf(fp2, "%f\n", clusterCoff);
    fclose(fp2);
    
    //Dumping the topological informatoin about the graph
    //sprintf(outInfoFile, "%d_NetworkInfo.txt", thresholdBin);
    //TSnap::PrintInfo(Graph, "",outInfoFile, 0);


    //Option to save the graph
    //TSnap::SaveEdgeList(Graph, "GraphEdgeListFile", "Save as tab-separated list of edges");
    //TSnap::SavePajek(Graph, "GraphPajekFile"); 

    /*########################### Here starts the randomization step ################################*/
    
    printf("Number of nodes are: %d\n",Graph->GetNodes());
    
    //Function to randomize the graph (REF: Joan serra suggested this way of randomization, its also a quite common way used in many contexts.)
    randomizeGraph(Graph, (int)1);
    
    //Obtaining the clustering coefficient of the randomized graph
    clusterCoff = TSnap::GetClustCf(Graph);

    //Dumping the clustering coefficient of the graph in an file
    sprintf(outFileName, "%d_ClusteringCoff_RANDOM_%s.txt", thresholdBin, config_id);
    fp2 = fopen(outFileName, "w");
    fprintf(fp2, "%f\n", clusterCoff);
    fclose(fp2);

    //Dumping the topological informatoin about the graph
    //sprintf(outInfoFile, "%d_NetworkInfo_RANDOM.txt", thresholdBin);
    //TSnap::PrintInfo(Graph, "",outInfoFile, 0);

    // This is another way to save a graph file
    /*TFOut FOut(outputNetworkFile); 
    Graph->Save(FOut);*/

}
