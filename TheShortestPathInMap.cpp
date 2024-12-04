#include <bits/stdc++.h>

using namespace std;

#define ll long long

struct Point {
     double x, y;
};

struct Map {
     Point points[1000];
     int numberOfPoints;
     int numberOfPaths;
     vector<pair<int, double>> adj[1000];
     int parent[1000];
     int start, end;
     double dist[1000];

     double distance(Point a, Point b) {
          return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
     }

     double heuristic(Point a, Point b) {
          return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
     }

     void input() { 
          ifstream inFile("input.txt");
          inFile >> numberOfPoints >> numberOfPaths;
          for (int i = 0; i < numberOfPoints; i++) {
               Point p;
               int point;
               inFile >> point >> p.x >> p.y;
               points[point] = p;
          }
          for (int i = 0; i < numberOfPaths; i++) {
               int a, b;
               inFile >> a >> b;
               double dist = distance(points[a], points[b]);
               adj[a].push_back({b, dist});
               adj[b].push_back({a, dist});
          }
          cin >> start >> end;
     }

     void init() {
          for (int i = 0; i < 1000; i++) {
               dist[i] = DBL_MAX;
          }
          dist[start] = 0;
          for (int i = 0; i < 1000; i++) {
               parent[i] = -1;
          }
     }

     void AStar() {
          priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
          pq.push({0, start});
          init();

          while (!pq.empty()) {
               auto p = pq.top();
               int currentPoint = p.second;
               pq.pop();

               cout << currentPoint << " " ;
               
               if (currentPoint == end) {
                    break;
               }
               for (auto next : adj[currentPoint]) {

                    int nextPoint = next.first;
                    double nextDist = next.second;

                    if (dist[currentPoint] + nextDist < dist[nextPoint] ) {
                         dist[nextPoint] = dist[currentPoint] + nextDist;
                         double f = dist[nextPoint] + heuristic(points[nextPoint], points[end]);
                         pq.push({f, nextPoint});
                         parent[nextPoint] = currentPoint;
                    }
               }
          }
          cout << "\nThe shortes path in map with A*: " << fixed << setprecision(3) << dist[end] << '\n';
     }

     void dijkstra() {
          priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
          pq.push({0, start});

          init();
          while (!pq.empty()) {
               auto p = pq.top();
               int currentPoint = p.second;
               pq.pop();

               cout << currentPoint << " " ;

               if (currentPoint == end) {
                    break;
               }
               for (auto next : adj[currentPoint]) {

                    int nextPoint = next.first;
                    double nextDist = next.second;
                    if (dist[currentPoint] + nextDist < dist[nextPoint] ) {
                         dist[nextPoint] = dist[currentPoint] + nextDist;
                         pq.push({dist[nextPoint], nextPoint});
                         parent[nextPoint] = currentPoint;
                    }
               }
          }
          cout << "\nThe shortes path in map in dijkstra: " << fixed << setprecision(3) << dist[end] << '\n';
     }

     void showPath() {
          vector<int> path;
          for (int i = end; i != -1; i = parent[i]) {
               path.push_back(i);
          }
          reverse(path.begin(), path.end());
          for (int i : path) {
               cout << i << " ";
          }
     }
};

int main(){
     ios_base::sync_with_stdio(0);cin.tie(0);
     Map map;
     map.input();
     cout << " The number of vertexs A* visit " << '\n'; 
     map.AStar();

     cout << " \nThe number of vertexs dijkstra visit " << '\n';
     map.dijkstra();   

     cout << " \nThe path from start to end " << '\n';
     map.showPath();
     return 0;
}