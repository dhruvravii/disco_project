/**
 * Group ID -
 * Member 1 Name -
 * Member 1 BITS ID -
 * Member 2 Name -
 * Member 2 BITS ID -
 * Member 3 Name -
 * Member 3 BITS ID -
 */

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Graph
{
    int n;                // Size of the graph
    int **adj;            // Adjacency matrix
    char **station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase
 * and creates the Graph structure from it.
 */
Graph *create_graph(char input_file_path[])
{
    FILE *f = fopen(input_file_path, "r");
    Graph *g = (Graph *)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char **)malloc(g->n * sizeof(char *));
    for (int i = 0; i < g->n; i++)
    {
        g->station_names[i] = (char *)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

// /**
//  * Q.1
//  * Return the number of junctions.
// */
// int find_junctions(Graph* g) {

// }

// /**
//  * Q.2
//  * Return true if the tour specified in the question is possible for this
//  * graph, else return false.
//  * Parameter SAME_STATION - If it is true, then question 2(a) must be solved.
//  * If false, then question 2(b) must be solved.
// */
// bool sheldons_tour(Graph* g, bool SAME_STATION) {

// }

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required.
 */
int **warshall(Graph *g)
{
    // Do not modify or delete pre-filled code!
    int **closure = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        closure[i] = calloc(g->n, sizeof(int));
    }

    // Code goes here
    int m = g->n; // number of vertices in the graph
    // initial adjacency matrix initialised as W0
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            closure[i][j] = g->adj[i][j];
        }
    }
    // Warshall's Algorithm written in C
    for (int k = 0; k < m; k++)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
            }
        }
    }

    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
 */
int find_impossible_pairs(Graph *g)
{
    int **closure = warshall(g); // Do not modify
    int ct = 0;                  // count variable that counts number of
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            // only evaluates lower triangle of matrix to prevent double counting
            // 0/1 present at the ith & jth position of closure matrix
            int t = closure[i][j];
            printf("%d ", t);
            if (t == 0)
                ct++; // count +1 when an impossible pair found
        }
        printf("\n");
    }
    free(closure);
    return ct / 2;
}

/**
 * Q.4
 * Return the number of vital train tracks.
 */

int find_vital_train_tracks(Graph *g)
{
    int **adjm = g->adj;
    int num = g->n;

    // Call of DFS while vertex are not all visited
    int num_cut_edge = 0;
    for (int i = 0; i < num; i++)
    {
        // evaluating only upper traingle to reduce double counting edges
        for (int j = i; j < num; j++)
        {
            // evaluating only for those cases where an edge exists
            if (adjm[i][j] == 1)
            {
                // temporarily removing this to edge to check if any impossible paths formed
                adjm[i][j] = 0;
                adjm[j][i] = 0;
                // Applying BFS at node i to see if any vertex remians unvisited
                int BFS_List[num];
                int visited[num]; // flag matrix to determine which cities have been visited
                for (int k = 0; k < num; k++)
                {
                    // array initialisation, 0 = not visited
                    visited[k] = 0;
                }
                int start = 0;
                int end = 0;
                BFS_List[end++] = i;
                visited[i] = 1; // since node i is visited thus marked = 1
                while (start < end)
                {
                    int current_city = BFS_List[start++];
                    for (int k = 0; k < g->n; k++)
                    {
                        if (adjm[current_city][k] == 1 && visited[k] != 1)
                        {
                            visited[k] = 1;
                            BFS_List[end++] = k;
                        }
                    }
                }
                // restoring values of adjacency matrix
                adjm[i][j] = 1;
                adjm[j][i] = 1;
                for (int k = 0; k < num; k++)
                {
                    // checking if any vertex is unvisited, and if true current edge is a cut edge
                    if (visited[k] == 0)
                    {
                        num_cut_edge++;
                        break;
                    }
                }
                printf("\n");
            }
        }
    }
    // return the number of vital edges (cut-edges)
    return num_cut_edge;
}
// /**
//  * Q.5
//  * Return upgrades, an array of size n.
//  * upgrades[i] should be 0 if railway station i gets a restaurant
//  * upgrades[i] should be 1 if railway station i gets a maintenance depot
//  * If it is not possible to upgrade, then return "-1" in the entire array
// */
int *upgrade_railway_stations(Graph *g)
{
    int *upgrades = calloc(g->n, sizeof(int)); // Do not modify

    // Code goes here
    int **adjm = g->adj;
    int num = g->n;
    int BFS_list[num];
    int visited[num];
    int start = 0;
    int end = 0;
    int current_city;
    for (int k = 0; k < num ; k++)
    {
        for (int i = 0; i < num; i++)
        {
            upgrades[i] = -1;
            visited [i] = 0;
        }
        
        visited[k] = 1;   // take k as starting vertex, thus it's already visited, hence 1
        upgrades[k] = 0; // assume at this vertex a restraunt is placed
        BFS_list[end++] = k; // inserting k as the 1st node in the list

        while (start < end)
        {
            current_city = BFS_list[start++];
            for (int i = 0; i < num; i++)
            {
                if (adjm[current_city][i] == 1 && visited[i] == 0)
                {
                    if (upgrades[current_city] == 1) 
                        upgrades[i] = 0;
                    else if (upgrades[current_city] == 0) 
                        upgrades[i] = 1;
                }
                else if (adjm[current_city][i] == 1 && visited[i] == 1){
                    if ((upgrades[current_city] == 1 && upgrades[i]==0) || (upgrades[current_city] == 0 && upgrades[i]==1)) 
                        continue;
                    else if ((upgrades[current_city] == 1 && upgrades[i]==1) || (upgrades[current_city] == 1 && upgrades[i]==1)){
                        for (int j=0; j<num; j++)
                            upgrades[j] = -1;
                        goto return_stmt; 
                    }
                }
            }
        }
    }
    return_stmt:
    return upgrades; // Do not modify
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
 */
int distance(Graph *g, int city_x, int city_y)
{
    int BFS_List[g->n];
    int visited[g->n];  // flag matrix to detrmine which cities have been visited
    int distance[g->n]; // calculated distance matrix
    for (int i = 0; i < g->n; i++)
    {
        // array initialisation
        visited[i] = 0;
        // initialistaion of all distance with -1,
        // which later implies unreachable
        distance[i] = -1;
    }
    int start = 0;
    int end = 0;
    BFS_List[end++] = city_x;
    visited[city_x] = 1;  // since x is visited thus flag = 1
    distance[city_x] = 0; // distance of x from x is 0
    while (start < end)
    {
        int current_city = BFS_List[start++];
        for (int i = 0; i < g->n; i++)
        {
            if (g->adj[current_city][i] == 1 && visited[i] != 1)
            {
                visited[i] = 1;
                BFS_List[end++] = i;
                distance[i] = distance[current_city] + 1;
                if (i == city_y)
                {
                    return distance[city_y];
                }
            }
        }
    }
    return distance[city_y];
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
 */
int railway_capital(Graph *g)
{
    int n = g->n;
    int min_sum[n];
    int answer = -1;
    for (int i = 0; i < n; i++)
    {
        int sum = 0;
        for (int j = 0; j < n; j++)
        {
            sum += distance(g, i, j);
        }
        min_sum[i] = sum;
    }
    int min_min = 25576;

    for (int k = 0; k < n; k++)
    {
        if (min_sum[k] < min_min)
        {
            min_min = min_sum[k];
            answer = k;
        }
    }
    return answer;
}

// /**
//  * Helper function for Q.8
// */
// bool maharaja_express_tour(Graph* g, int source, int current_city, int previous_city, int* visited) {

// }

// /**
//  * Q.8
//  * Return true if it is possible to have a Maharaja Express tour
//  * starting from source city, else return false.
// */
// bool maharaja_express(Graph* g, int source) {
//     int* visited = (int*)malloc(g->n * sizeof(int));
//     for(int i = 0; i < g->n; i++) {
//         visited[i] = 0;
//     }
//     // Hint: Call the helper function and pass the visited array created here.
// }

int main()
{
    char input_file_path[100] = "testcase_1.txt"; // Can be modified
    Graph *g = create_graph(input_file_path);     // Do not modify
    printf("Q.3 \n");
    printf("Impossible to travel city pairs = %d \n", find_impossible_pairs(g));
    printf("Q.4 \nNumber of vital train tracks = %d \n", find_vital_train_tracks(g));
    printf("Q.6 \nDistance between Delhi and Faridabad = %d \n", distance(g, 3, 4));
    printf("Q.7 \nCapital City = \n %s", g->station_names[railway_capital(g)]);
    return 0;
}