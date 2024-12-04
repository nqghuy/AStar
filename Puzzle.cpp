#include <bits/stdc++.h>

using namespace std;

#define ll long long

const int MAX_SIZE = 3;

int di[4] = {-1, 0, 0, 1};
int dj[4] = {0, -1, 1, 0};

struct Board
{
     int board[MAX_SIZE][MAX_SIZE];
     int x, y;

     void input()
     {
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    cin >> board[i][j];
                    if (board[i][j] == 0)
                    {
                         x = i;
                         y = j;
                    }
               }
          }
     }

     bool operator==(const Board &b) const
     {
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    if (board[i][j] != b.board[i][j])
                    {
                         return false;
                    }
               }
          }
          return true;
     }

     bool operator!=(const Board &b) const
     {
          return !(*this == b);
     }

     bool operator<(const Board &b) const
     {
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    if (board[i][j] < b.board[i][j])
                    {
                         return true;
                    }
                    else if (board[i][j] > b.board[i][j])
                    {
                         return false;
                    }
               }
          }
          return false;
     }

     bool operator>(const Board &b) const
     {
          return b < *this;
     }

     friend ostream &operator<<(ostream &os, const Board &b)
     {
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    if (b.board[i][j] == 0)
                    {
                         os << "  ";
                    }
                    else
                    {
                         os << b.board[i][j] << " ";
                    }
               }
               os << '\n';
          }
          return os;
     }

     vector<Board> createNeighbors()
     {
          vector<Board> neighbors;
          for (int k = 0; k < 4; k++)
          {
               int newx = x + di[k];
               int newy = y + dj[k];

               // di chuyển ô trống về 4 hướng nếu được
               if (newx >= 0 && newx < MAX_SIZE && newy >= 0 && newy < MAX_SIZE)
               {
                    Board newBoard = *this;
                    swap(newBoard.board[x][y], newBoard.board[newx][newy]);
                    newBoard.x = newx;
                    newBoard.y = newy;
                    neighbors.push_back(newBoard);
               }
          }
          return neighbors;
     }
};

// Hàm băm cho Board
struct BoardHash
{
     size_t operator()(const Board &b) const
     {
          size_t hash_value = 0;
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    hash_value = hash_value * 31 + std::hash<int>()(b.board[i][j]);
               }
          }
          return hash_value;
     }
};

// Biến toàn cục để chống tràn
// nếu MAX_SIZE = 3 thì có thể để local
// nếu MAX_SIZE = 4 thì cần để global
// sử dụng unordered_map để truy xuất nhanh
vector<Board> steps;
unordered_map<Board, Board, BoardHash> parent;
unordered_map<Board, int, BoardHash> dist;
priority_queue<pair<int, Board>,
               vector<pair<int, Board>>,
               greater<pair<int, Board>>>
     pq;

struct Solution
{
     Board board;
     Board goal;
     
     void input()
     {
          board.input();
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    goal.board[i][j] = i * MAX_SIZE + j + 1;
                    goal.board[MAX_SIZE - 1][MAX_SIZE - 1] = 0;
               }
          }
     }

     int heuristic(Board b)
     {
          int h = 0;
          for (int i = 0; i < MAX_SIZE; i++)
          {
               for (int j = 0; j < MAX_SIZE; j++)
               {
                    if (b.board[i][j] != 0)
                    {    
                         // Tính khoảng cách Manhattan
                         h += abs(i - (b.board[i][j] - 1) / MAX_SIZE) + abs(j - (b.board[i][j] - 1) % MAX_SIZE);
                    }
               }
          }
          return h;
     }

     void solve()
     {    
          cout << "the number of nodes explored by BFS : ";
          bfs();        
          cout << "the number of nodes explored by A* : ";
          AStar();
     }

     void AStar () {
          pq.push({heuristic(board), board});
          dist.clear();
          dist[board] = 0;
          int cnt = 0;
          while (!pq.empty())
          {    
               cnt++;
               Board currentBoard = pq.top().second;
               int currentLength = pq.top().first;
               pq.pop();

               if (currentBoard == goal)
               {    
                    cout << cnt << '\n';
                    return;
               }

               // các bước tiếp theo của bảng hiện tại
               vector<Board> neighbors = currentBoard.createNeighbors();

               for (Board next : neighbors)
               {
                    int newDist = dist[currentBoard] + 1;
                    if (dist.find(next) == dist.end() || dist[next] > newDist)
                    {
                         dist[next] = newDist;
                         pq.push({newDist + heuristic(next), next});
                         parent[next] = currentBoard;
                    }
               }
          }
     }

     void bfs() {
          queue<Board> q;
          q.push(board);

          dist.clear();
          dist[board] = 0;

          int cnt = 0;
          while (!q.empty()) {
               cnt++;

               Board currentBoard = q.front();
               q.pop();

               if (currentBoard == goal) {
                    cout << cnt << '\n';
                    return;
               }

               vector<Board> neighbors = currentBoard.createNeighbors();

               for (Board next : neighbors) {
                    if (dist.find(next) == dist.end()) {
                         q.push(next);
                         dist[next] = dist[currentBoard] + 1;
                         parent[next] = currentBoard;
                    } 
               }
          }
          
     }
     void printPath()
     {    
          cout << "---------------\n";
          findSteps();
          for (Board b : steps)
          {
               cout << b << '\n';
          }
     }

     void findSteps()
     {
          Board current = goal;
          while (current != board)
          {
               steps.push_back(current);
               current = parent[current];
          }
          steps.push_back(board);
          reverse(steps.begin(), steps.end());
     }
};

int main()
{
     ios_base::sync_with_stdio(0);
     cin.tie(0);
     Solution solution;
     solution.input();
     solution.solve();
     solution.printPath();
     return 0;
}