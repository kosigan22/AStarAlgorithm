#include <vector>
#include <iostream>
#include <jni.h>
#include <string.h>
#include <string>
#include <stack>
#include <set>
#include <cmath>
#include <android/log.h>


using namespace std;

#define Width 7
#define Length 10

struct Node
{
    int row,col;

    Node(int i,int j)
    {
        row=i;
        col=j;
    }

    Node()
    {
        row = 0;
        col = 0;
    }
};

struct Nodes
{
    double index;
    Node node;

    Nodes(double i, int n1, int n2)
    {
        index=i;
        node= Node(n1,n2);
    }

    bool operator <(const Nodes& pt) const
    {
        return (index < pt.index) || (pt.index >= index && (node.col < pt.node.col && node.row < pt.node.row));
    }
};

struct NodeParams
{
    int x, y;
    double f, g, h;

    NodeParams()
    {
        x = -1;
        y = -1;
        f=999;
        g=999;
        h=999;
    }
};

int direction[8][2] = {
        {-1,0},  //North
        {1,0},   //South
        {0,1},   //East
        {0,-1},  //West
        {-1,1},  //North-East
        {-1,-1}, //North-West
        {1,1}, //South-East
        {1,-1} //South-West
};

bool validCheck(int grid[Width][Length], int row, int col)
{
    if (row >= 0 && row < Width && col >= 0 && col < Length && grid[row][col] == 0)
        return true;
    else
        return false;
}

double heuristicFunction(int row, int col, Node dest)
{
    return ((double)sqrt ((row-dest.row)*(row-dest.row)
                          + (col-dest.col)*(col-dest.col)));
}

double distanceFunction(int l, int k)
{
    return ((double)sqrt(pow(l,2) + pow(k,2)));
}

bool destinationCheck(int row, int col, Node dest)
{
    if (col == dest.col && row == dest.row)
        return true;
    else
        return false;
}

stack<Node> printPath(NodeParams nodeParams[][Length], Node dest)
{
    printf ("\nThe Path to the destination is ");
    stack<Node> map;

    int row, column, str_row, str_col;
    row = dest.row;
    column = dest.col;

    while (!(nodeParams[row][column].y == column
             && nodeParams[row][column].x == row ))
    {
        map.push(Node(row, column));
        str_row = nodeParams[row][column].x;
        str_col = nodeParams[row][column].y;
        row = str_row;
        column = str_col;
    }

    // Print path
    map.push(Node(row, column));
    return map;
}

stack<Node> aStar(int parkingLot[][Length], Node src, Node dest) {

    NodeParams nodeParams[Width][Length];

    int x_corr, y_corr, new_x, new_y;
    double temp_g, temp_h, temp_f;

    if (!validCheck(parkingLot, src.row, src.col))
    {
        printf("Source is blocked\n");
    }

    if (!validCheck(parkingLot, dest.row, dest.col)) {
        printf("Destination is blocked\n");
    }

    if (destinationCheck(src.row, src.col, dest)) {
        printf("We are already at the destination. Try again\n");
    }

    // Initializing parameters for the origin node
    x_corr = src.row;
    y_corr = src.col;
    nodeParams[x_corr][y_corr].x = x_corr;
    nodeParams[x_corr][y_corr].y = y_corr;
    nodeParams[x_corr][y_corr].f = 0.0;
    nodeParams[x_corr][y_corr].g = 0.0;
    nodeParams[x_corr][y_corr].h = 0.0;

    set<Nodes> visited;

    visited.insert(Nodes(0.0, x_corr, y_corr));

    while (!visited.empty()) {

        Nodes p = *visited.begin();

        // Removing first vertex from the visited Node list
        visited.erase(visited.begin());

        x_corr = p.node.row;
        y_corr = p.node.col;

        for(int l = 0; l < 8; ++l) {
                new_x = x_corr + direction[l][0];
                new_y = y_corr + direction[l][1];

                    if (validCheck(parkingLot, new_x, new_y)) {
                        if (destinationCheck(new_x, new_y, dest)) {
                            printf("The destination has been found\n");
                            nodeParams[new_x][new_y].x = x_corr;
                            nodeParams[new_x][new_y].y = y_corr;
                            return printPath(nodeParams, dest);
                        }
                        else if (validCheck(parkingLot, new_x, new_y)) {
                            temp_g = nodeParams[x_corr][y_corr].g + distanceFunction(direction[l][0], direction[l][1]);
                            temp_h = heuristicFunction(new_x, new_y, dest);
                            temp_f = temp_g + temp_h;

                            if (nodeParams[new_x][new_y].f > temp_f) {
                                visited.insert(Nodes(temp_f, new_x, new_y));
                                nodeParams[new_x][new_y].f = temp_f;
                                nodeParams[new_x][new_y].g = temp_g;
                                nodeParams[new_x][new_y].h = temp_h;
                                nodeParams[new_x][new_y].x = x_corr;
                                nodeParams[new_x][new_y].y = y_corr;
                            }
                        }
                    }
        }
    }
        }


extern "C" JNIEXPORT jobject JNICALL
Java_com_example_myapplication_activity_useractivity_parking_ParkingSpaceDirectionsActivity_astar(
        JNIEnv* env,
        jobject, jstring parkingSpotString /* this */) {

    const char *parkingSpot = env->GetStringUTFChars(parkingSpotString, NULL);

    int conversion_1 = NAN;
    if(parkingSpot[0]=='A')
    {
        conversion_1 = (int) parkingSpot[0] - (int) 'A' + 1;
    }
    else if(parkingSpot[0]=='D')
    {
        conversion_1 = (int) parkingSpot[0] - (int) 'A' -1 ;
    }
    int conversion_2 = (int) parkingSpot[1] - (int) '0';

    int parkingLot[Width][Length] =
                {
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                };

        Node origin = Node(4,9);
        Node destination = Node(  conversion_1,conversion_2 );


    stack<Node> directions = aStar(parkingLot, origin, destination);

    jclass clsVec = env->FindClass("java/util/Vector");
        jclass clsInt = env->FindClass("java/lang/Integer");

        jmethodID midInit = env->GetMethodID(clsInt, "<init>", "(I)V");
        jmethodID constructor = env->GetMethodID(clsVec, "<init>", "()V");

        jobject results = env->NewObject(clsVec, constructor, "");

        jmethodID vecAdd = env->GetMethodID(clsVec, "addElement", "(Ljava/lang/Object;)V");

        while (!directions.empty())
        {
  //        jobject retStr = env->NewObject(to_string(directions.top().row).c_str());
            jobject newObj_x = env->NewObject(clsInt, midInit, directions.top().row);
            jobject newObj_y = env->NewObject(clsInt, midInit, directions.top().col);

//          jobject retStr1 = env->NewStringUTF(to_string(directions.top().col).c_str());
            env->CallVoidMethod(results, vecAdd, newObj_x);
            env->CallVoidMethod(results, vecAdd, newObj_y);
            directions.pop();
        }
        return results;
}