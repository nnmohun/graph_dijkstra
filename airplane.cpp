#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <string>
#include <iterator>


using namespace std;

//use of graph to find shortest path or cost to travel between cities
//graph is built using an adjacency list to store connecting vertices 
typedef pair<int, int> intPair;

class graph
{
    public: 
        
        graph(int v);
      

        void addEdge(int v1, int v2, int cost);//edge connecting v1 to v2 with cost

        void fillMap(ifstream &file);
        void printGraph();

        //dijkstra algorithm with priority queue
            //returns the path of vertices of shortest path
        vector<int> shortestPath(string cityStart, string cityEnd);//pass in the start end keys of the city map

        //takes in strings converts to ints key for shortest path call
            //returns the vector path of the trip
        void printTrip(vector<int> path);
      

    private:
        int num_verts; //number vertices in graph
        map<string,int> cities; //key,value
        list<pair<int,int> > *adjacentVerts;
        int totalMiles;




};


graph::graph(int v)
{
    this->num_verts=v+1;//pass in num verts to this
    adjacentVerts = new list<intPair>[num_verts];

    this->totalMiles=0;
}

void graph::addEdge(int v1, int v2, int cost)
{
    
    
    adjacentVerts[v1].push_back(make_pair(v2,cost));
    adjacentVerts[v2].push_back(make_pair(v1,cost));
}
void graph::fillMap(ifstream &file)
{
    string city="";
    int i=0;
    while(!file.eof())
    {
        file>>city;
        cities[city]=i; //tying city to map key i
        i++;
        
    }
   
    
    for(auto i:cities)
        cout<<i.first<<" "<<i.second<<endl;

    string city1,city2;
    int c1,c2,w;

    fstream file2;
    file2.open("map.txt");
    while(!file2.eof())
    {
        file2>>city1;
        file2>>w;
        file2>>city2;

        c1 = cities[city1];
        c2=cities[city2];
        cout<<c1<<"  "<<city1<<"  ";
        cout<<w<<"  ";
        cout<<c2<<"  "<<city2<<endl;

        addEdge(c1,c2,w);
    }


}

vector<int> graph::shortestPath(string cityStart, string cityEnd)
{
    typedef int vertex;
    vertex start = cities[cityStart];
    vertex end = cities[cityEnd];

    vector<int> dist(num_verts,INT_FAST32_MAX);
    vector<vertex> path(num_verts);
    vector<bool> visit(num_verts,false);
    
    typedef pair<int,vertex> pqElement;
    priority_queue<pqElement,vector<pqElement>,greater<pqElement> > pq;

    pq.push(pqElement(0,start));

    dist[start]=0;
    visit[start]=true;
    path.push_back(start);

    while(!pq.empty() && (visit[end]!=true))
    {
        pqElement top = pq.top();
        vertex v = top.second;
        cout<<"Vertex is: "<<v;
        pq.pop();

        int dV = dist[v];
        cout<<"  distance is "<<dV<<endl;

        path.push_back(v);
        
        //adjacent list is vertex, cost format
        for(auto i = adjacentVerts[v].begin(); i != adjacentVerts[v].end();i++)
        {
            vertex adjV = (*i).first;
            int dAdjV = (*i).second;
            cout<<"adjacent vertices: "<<adjV<<"  distance: "<<dAdjV<<endl;

            int dEdgeSum = dV + dAdjV;

            cout<<"Traveled so far: "<<dEdgeSum<<endl;

            if(dist[adjV] > dEdgeSum)
            {
                cout<<dist[adjV]<<" > "<<dEdgeSum<<endl;

                dist[adjV] = dEdgeSum;
                totalMiles=dist[adjV];

                cout<<"updated dist: "<<dist[adjV]<<endl;

                pq.push(pqElement(dist[adjV],adjV));
                path[adjV]=v;
                visit[adjV]=true;
            }


        }
        
       
    }
    
    //cout<<"\nTotal Miles: "<<totalMiles<<endl; works

    vector<vertex> finalPath;
    vertex f = end;
    while(f!=start)
    {
        finalPath.push_back(f);
        f=path[f];
    }
    cout<<endl;
    finalPath.push_back(start);
    reverse(finalPath.begin(), finalPath.end());
    for(int i=0; i<finalPath.size(); i++)
        cout<<finalPath[i]<<"  ";

    return finalPath;
}

void graph::printTrip(vector<int> trip)
{

    cout<<"\nThe shortest distance is to travel throught these cities: ";

   for(int i=0;i<trip.size();i++)
   {
       int v = trip[i];

       for(auto c=cities.begin();c!=cities.end();c++)
       {
           if(v==c->second)
                cout<<c->first<<"  ";
       }
   }
   cout<<"\nYou will travel: "<<totalMiles<<endl;


}




void graph::printGraph()
{
    for(int i=0;i<num_verts;i++)
    {
        cout<<"Vertex "<<i<<" makes an Edge with Vertices: \n";
        for(auto itr = adjacentVerts[i].begin(); itr!=adjacentVerts[i].end(); itr++)
        {
            int v = itr->first;
            int w = itr->second;

            cout<<"->"<<v<<" cost: "<<w;
            cout<<"\n";
            
        }
        cout<<endl;
    }
    

}

int main()
{
     // vertices in given graph are 9  
    ifstream file1;
    file1.open("map.txt");
    int v =std::count(std::istreambuf_iterator<char>(file1), 
             std::istreambuf_iterator<char>(), '\n');
    file1.close();
    
    graph g(v); //  call Constructor by creating an object of graph  
    
    file1.open("cities.txt");
    g.fillMap(file1);

    g.printGraph();

    vector<int> i= g.shortestPath("Cambridge","Toledo");
    g.printTrip(i);
   

    return 0;
}
