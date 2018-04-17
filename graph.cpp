#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<list>
#include<queue>
#include<stack>

using namespace std;


class Graph
{
	int v;
	list<int> *adj;

	public:
	Graph(int v);
	void addEdge(int v , int w);
	void BFS(int s);
	void DFS(int s);
	void DFSHelper(int s, vector<bool> &visited);
	void topologicalSort();
	void topologicalSortHelper(int s, vector<bool> &visited, stack<int> &st);
	bool isCyclic();
	bool isCyclicUtil(int s , vector<bool>& visited , vector<bool> &backEdge);
};


class unDirectedGraph
{
	int s;
	list<int> *edges;
	public:
	unDirectedGraph(int v);
	void addBiEdge(int v, int w);
	bool iscyclic();
	bool isCyclicUtil(int s , vector<int> &visited);
}; 

unDirectedGraph::unDirectedGraph(int v)
{
	this->s = v;
	edges = new list<int>[s];
}

unDirectedGraph::addBiEdge(int v , in w)
{
	edges[v].push_back(w);
	edges[w].push_back(v);
	cout << v << "<->" << w <<endl;
}	

bool unDirectedGraph::isCyclicUtil(int s, vector<bool> &visited, int parent)
{
	visited[s] = true;
	list<int>::iterator it;
	for(it = adj[s].begin(); it != adj[s].end(); it++)
	{
		if(!visited[*it])
		{
		    if(isCyclicUtil(*it, visited, s)
			return true;
		}
		else if(*it != parent)
			return true;
	}
	return false;
	
}

bool unDirectedGraph::isCyclic()
{
	vector<bool> visited(s, false);
	for(int i = 0; i < s ; i++)
	{	// No traversing if already visited
		if(!visited[i])
		   if(isCyclicUtil(i, visited, -1)
			return true;
	}
	return false;
}

Graph::Graph(int v)
{
	this->v = v;
	adj = new list<int>[v];
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w);
	cout << v << "->" << w <<endl;
}

void Graph::BFS(int s)
{
	vector<bool> visited (v, false);
	queue<int> q;
	visited[s] = true;
	q.push(s);
	
	list<int>::iterator it;
	while(!q.empty())
	{
		s = q.front();
		cout << s << endl;	
		q.pop();
		for(it = adj[s].begin(); it != adj[s].end(); it++)
		{
			if(!visited[*it])
			{
				visited[*it] = true;
				q.push(*it);		
			}
		}
	}
}


bool Graph::ifPathExists(int s, int d)
{
	if(s == d)
	return true;
	
	vector<bool> visited(v, false);
	visited[s] = true;
	
	queue<int> q;
	list<int>::iterator it;
	
	q.push(s);
	while(!q.empty())
	{
		s=q.front();
		q.pop();
		for(it = adj[s].begin(); it != adj[s].end() ; it++)
		{
			if(*it == d)
			return true;
			if(!visited[*it])
				q.push_back(*it);
		}	
	}
	return false;
}



void Graph::DFSHelper(int s, vector<bool> &visited)
{
	visited[s] = true;
	cout << s << endl;
	
	list<int>::iterator it;
	for(it = adj[s].begin(); it!= adj[s].end(); it++)
	{
		if(!visited[*it])
		DFSHelper(*it, visited);
	}
}

void Graph::DFS(int s)
{
	vector<bool> visited(v, false);
	DFSHelper(s, visited);
}

void Graph::topologicalSortHelper(int i, vector<bool>&visited , stack<int> &st)
{
	visited[i] = true;
	list<int> ::iterator it;
	for(it = adj[i].begin(); it!= adj[i].end(); it++)
	{
		if(!visited[*it])
			topologicalSortHelper(*it, visited, st);
	}
	st.push(i);
}

void Graph::topologicalSort()
{
	stack<int> st;
	vector<bool>visited (v, false);
	for(int i = 0 ; i < v; i++)
	{
		if(!visited[i])
			topologicalSortHelper(i, visited, st);
	}
	
	while(!st.empty())
	{
		cout << st.top() << endl;
		st.pop();
	}
}

bool Graph::isCyclicUtil(int i, vector<bool> &visited, vector<bool> &backEdge)
{
	if(visited[i] == false)
	{
		visited[i] = true;
		backEdge[i] = true;
		list<int>::iterator it;
		for(it = adj[i].begin(); it!= adj[i].end(); it++)
		{
			if(!visited[*it] && isCyclicUtil(*it, visited, backEdge))
				return true;
			else if(backEdge[*it])
				return true;
		}
	}
	// Remove vertex from backEdge vector
	backEdge[i] = false;
	return false;
}


bool Graph::isCyclic()
{
	vector<bool> visited(v , false);
	vector<bool> backEdge(v, false);
	
	for(int i = 0; i < v; i++)
	{
		if(isCyclicUtil(i, visited, backEdge))
			return true;
	}
	return false;
}

bool isSafe(vector<vector<int>> matrix, int i, int j, vector<vector<int> >&visited)
{
	int rowNum = matrix.size();
	int colNum = matrix[0].size();

	if((i>= 0) && (i < rowNum) && (0>= j) && (j <colNum) &&
		matrix[i][j] && !visited[i][j])
	return true;
	else
	return false; 	

}

//Every cell should be checked for 8 neighbours 
void matDFS(vector<vector<int> > matrix, int i, int j , vector<int> &visited)
{
	int rowNum[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int colNum[] = {-1, 0, 1, -1, 1, -1, 0, 1};

	visited[i][j] = true;

	for(int k = 0; k < 8; k++)
	{
		if(isSafe(matrix, i+rowNum[k] , j+colNum[k], visited))
			DFS(matrix, i+rowNum[k], j+colNum[k], visited);
	}
}

int countIslands(vector<vector<int> > &matrix)
{
	int r = matrix.size();
	int c = matrix[0].size();
	
	vector<vector<int> > visited (r, vector<int>(c));
	int count = 0;	
	for(int i = 0; i < r; i++)
	{
	  for(int j = 0; j < c; j++)
	  {     //if cell is one and not visited before
		if(matrix[i][j] && !visited[i][j])
		{
			matDFS(matrix, i, j, visited);
			++count;
		}
	  }	

	}
	return count;
}




bool isBipartite(vector<vector<int> > gMatrix , int s )
{
	vector<int>colorArr(v, -1);
	colorArr[s] = 1;
	queue<int> q;
	q.push(s);
	while(!q.empty())
	{
		int u = q.front();
		q.pop();
		//if self loop 
		if(gMatrix[u][u] == 1)
		return false;

		for(int d = 0 ; d < v; d++)
		{	//if the edge exists and desti is not colored yet
			if(gMatrix[u][d] && colorArr[d] == -1)
			{
				colorArr[d] = 1 - colorArr[u];
				q.push(d);
			}
			else if (gMatrix[u][d] && colorArr[u] == colorArr[d])
				return false;
		}
	}
	return true;
}




int main()
{
	Graph g(4);
	g.addEdge(0,1);
	g.addEdge(0,2);
	g.addEdge(1,2);
	g.addEdge(2,0);
	g.addEdge(2,3);	
	g.addEdge(3,3);
	cout << "The BFS Traversal from vertex 2: "<< endl;
	g.BFS(2);
	cout << "The DFS Traversal from vertex 2: " <<endl;
	g.DFS(2);
	Graph g1(6);
	g1.addEdge(5,2);
	g1.addEdge(5,0);
	g1.addEdge(4,0);
	g1.addEdge(4,1);
	g1.addEdge(2,3);	
	g1.addEdge(3,1);
	cout << "The topological sorting for the graph is :" <<endl;
	g1.topologicalSort();
	Graph g2(4);
	g2.addEdge(0,1);
	g2.addEdge(0,2);
	g2.addEdge(1,2);
	g2.addEdge(2,0);
	g2.addEdge(2,3);	
	g2.addEdge(3,3);
	if(g2.isCyclic())
	cout << "Graph has cycle " << endl;
	else
	cout << "Graph does not contain any cycle" << endl;
	
	//Group of connected 1s form island. Count the number of islands from matrix
	vector<vector<int> > matrix ({1,1,0,0,0},
				     {0,1,0,0,1},
				     {1,0,0,1,1},
				     {0,0,0,0,0},					  				   {1,0,1,0,1});
	return 0;
	cout << "The number of islands is " << countIslands(matrix) <<endl;
}



























