#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<unordered_map>
#include<set> 

using namespace std;

// (Climbing stairs problem) -> Number of ways to get to TOP, 
// if you are allowed to climb 1 to N steps at a time.
int numberOfWaysTopHelper(int top , int max_step, vector<int>& numberOfWays)
{
	if(top <= 1)
	return 1;

	if(numberOfWays[top] == 0)
	{
		for (int i = 1; i <= max_step && top - i >= 0 ; i++)
			numberOfWays[top] += numberOfWaysTopHelper(top-i, max_step, numberOfWays); 
	}
	return numberOfWays[top]
}


int numberOfWaysTop(int top , int max_step)
{
	vector<int> numberOfWays(top + 1 , 0);
	return numberOfWaysTopHelper(top, max_step, numberOfWays);
}

/*******************************************************************************************/

//(Coin Problem 1) -> Given quarters(25 cents) , dimes(10 cents) and nickels(5 cents) and pennies (1 cent),
// number of ways to form N cents

int numberOfWaysAmount(int amount, vector<int> &coins)
{
	vector<int> numberOfWays(amount+1 , 0):
	numberOfWays[0] = 1;
	for(auto &coin : coins)
	{
		for(int i = coin; i <= amount ; i++)
		numberOfWays[i] += numberOfWays[i-coin] ;
	}
	return numberOfWays[amount] ;
}

/************************************************************************************************/

//(Coin Problem 2) -> Given quarters(25 cents) , dimes(10 cents) and nickels(5 cents) and pennies (1 cent),
// compute fewest number of coins to make N cents

int fewestNumberOfCoins(int amount, vector<int> &coins)
{
	int max = amount + 1;
	vector<int> numberOfWays(amount + 1, 0);
	numberOfWays[0] = 1;
	for(int i = 1; i <= amount; i++)
	{
		for(int j = 0; j < coins.size(); j++)
		{	
			if(coins[j] <= amount)
			{
				numberOfWays[i] = min(numberOfWays[i] , numberOfWays[i - coins[j]] );		
			}
		}
		return numberOfWays[amount] > amount ? -1 : numberOfways[amount];
	}
	
}

/*************************************************************************************************/

//(Soccer Game) -> 
int numberOfWaysFinalScore(int final_score , vector<int>& individual_scores)
{
	vector<vector<int>> numberOfWays(individual_scores.size() , vectr<int>(final_score+1 , 0));
	for(int i = 0; i < individual_scores.size(); i++)
	{	numberOfWays[i][0] = 1;
		for(int j = 1; j <= final_score; j++)
		{
			without_this_play = i>=1 ? numberOfWays[i-1][j] : 0;
			with_this_play = j>=individual_scores[i] = numberOfWays[i][j-individual_scores[i]] : 0;
			numberOfWays[i][j] = with_this_play + without_this_play;
		}
	}
	return numberOfWays.back().back();
}


/************************************************************************************************/

//(Number of ways to Tarverse 2D array Problem 1) 
// Count the number of ways to go from top left corner to bottom right corner, 
// if bottom move and right move is only allowed


int numberOfWaysTraverse(int y , int x , vector<vector<int>> &numberOfWays)
{
	if(x == 0 && y == 0)
	return 1;

	if(numberOfWays[x][y] == 0)
	{
		int ways_right = y == 0 ? 0:numberOfWaysTraverse(y-1, x , numberOfWays); 
		int ways_bottom = x == 0? 0:numberOfWaysTraverse(y, x-1, numberOfWays); 
		numberOfWays[x][y] = ways_right + ways_bottom;
	}
	return numberOfWays[x][y];
}


int numberOfWaysTraverse(int n , int m)
{
	vector<vector<int>> numberOfWays( n , vector<int> (m , 0));
	return numberOfWaysTraverseHelper(n-1, m-1, numberOfWays);
}

/*******************************************************************************************************/

//(Number of ways to Traverse 2D array Problem 2: ObstacleGrid)
// Count the number of ways to go from top left corner to bottom right corner, 
// if bottom move and right move is only allowed 

int numberOfWaysTraverse(int x , int y, vector<vector<int>> &numberOfWays, vector<vector<int>> &ObstacleGrid)
{
	if(x == 0 && y == 0)
	return 1;

	if(numberOfWays[x][y] == 0)
	{
		if(ObstacleGrid[x][y]==0) 
		{
			int ways_right = y == 0 ? 0:numberOfWaysTraverse(x, y-1, numberOfWays, ObstacleGrid); 
			int ways_bottom = x == 0? 0:numberOfWaysTraverse(x-1, y, numberOfWays, ObstacleGrid); 
			numberOfWays[x][y] = ways_right + ways_bottom;
		}
	}
	return numberOfWays[x][y];
}


int numberOfWaysTraverse(vector<vector<int>> &ObstacleGrid)
{	
	int m = ObstacleGrid.size();
	int n = ObstacleGrid[0].size();
	vector<vector<int>> numberOfWays(m, vector<int> (n, 0));
	return numberOfWaysTraverseHelper(m-1, n-1, numberOfWays, ObstacleGrid);
}


/***********************************************************************************************************/





















/*********************************************************************************************************/

int main()
{
	



	return 0;
}
