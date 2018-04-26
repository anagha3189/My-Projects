#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<set>
#include<unordered_set>
#include<algorithm>

using namespace std;

// Add without plus
int add (int x, int y)
{
	while(y != 0)
	{
		int carry = x ^ y;
		x = x ^ y;
		y = carry << 1;
	}
	return x;
}

/*****************************************************************************************************************/

//Shuffle numbers uniformly

void shuffle (vector<int> &nums)
{
	srand(time(NULL));
	int n = nums.size();
	for(int i = n-1; i > 0 ; i--)
	{
		j = rand() % (i+1)
		swap(nums[i] , nums[j]);
	}
}

/*****************************************************************************************************************/

//Random set









/*****************************************************************************************************************/

//Missing Numbers














/**********************************************************************************************************************/

//Letters and Numbers

int findMaxLength(vector<char> &alNums)
{
	int result = 0;
	int count = 0;
	unordered_map<int, int> lookup;
	lookup[0] = -1;
	for(int i = 0 ; i < alNums.size(); i++)
	{
		count += alNums[i] == 1 ? 1 : -1;
		if(lookup.count(count))
		{
			result = max(result, i - lookup[count]);	
		}
		else
		{
			lookup[count] = i;
		}
	}
	return result;
}

/******************************************************************************************************************/

//Word Ladder

bool isAdjacent(string word1 , string word2)
{
	int count  = 0;
	int n = word1.length();
	for(int i = 0; i < n ; i++)
	{
		if(word1[i] != word2[i])
			count++;
		if(count > 1)
		return false;
	}
	return count == 1 ? true : false;
}


struct QItem
{
	string word;
	int len;
};

int shortestChainLen(string src, string target, unordered_set<string> words)
{
	Queue<QItem> q;
	QItem item = {src, 1};
	q.push(item);

	while(!q.empty())
	{
		QItem curr = q.front();
		q.pop();
		for(unordered_set<string>::iterator it = words.begin() ; it != words.end() ; it++)
		{
			string temp = *it;
			if(isAdjacent(curr.word , temp))
			{
				item.word = temp;
				item.len = curr.len+1;
				
				q.push(item);
				
				words.erase(temp);
				if(temp == target)
					return item.len;
			}

		}
	}
	return 0;

}

/***************************************************************************************************************/

//Kth number with just 2, 3, 7 as prime factors
















/******************************************************************************************************************/

//Dutch-National Flag Problem














/**************************************************************************************************************/

//Shortest Word Distance Problem 1

int shortestDistance(vector<string> words, string word1 , string word2 )
{
	int index1 = -1;
	int index2 = -1;
	int dist = numeric_limits<int>::max();
	
	for(int i = 0; i < words.size(); i++)
	{
		if(words[i].compare(word1) == 0)
			index1 = i;
		else if(words[i].compare(word2) == 0)
			index2 = i;
		if(index1 != -1 && index2 != -1)
		dist = min(dist, abs(index1- index2));
	}
	return dist;
}


/************************************************************************************************************************/

//Shortest Word Distance Problem 2 (If the shortest distance is to be found repeatedly)
















/************************************************************************************************************************/

//Shortest Word Distance Problem 3

int shortestDistanceSame(vector<string> words, string word1 , string word2)
{
	int index1 = -1, index2 = -1;
	int dist = numeric_limits<int>::max();
	bool is_same = word1 == word2;

	for(int o = 0; i < words.size(); i++)
	{
		if(words[i].compare(word1) == 0)
		{
			index1 = i;
			if(is_same && index1 != -1)
				dist = min(dist, abs(i - index1);		
			index1 = i;
		}
		else if(words[i].compare(word2) == 0)
		{
			index2 = i;
		}

		if(index1 != -1 && index2 != -1)
		dist = min(dist, abs(index2 - index1));

	}
	return dist;
}

/************************************************************************************************************************/

// Shortest distance between numbers / Minimum distance between numbers

int shortestDistanceNumbers(vector<int> nums, int x, int y)
{
	int dist = numeric_limits<int>::max();
	int prev = -1;

	for(int i = 0; i < nums.size(); i++)
	{	
		if(nums[i] == x)
		index1 = i;
		else if(nums[i] == y)
		index2 = i;
		if(index1 != -1 && index2 != -1)
		dist = min(dist, abs(index2 - index1));
	}
	return dist;
}

/**********************************************************************************************************************/

//Shortest Distance from particular character


















/*******************************************************************************************************************/

//Smallest K numbers in an array

vector<int> smallestK(vector<int> nums , int k)
{
	priority_queue<int, vector<int> , greater<int>> pq;
	vector<int> result;
	for(int i = 0 ; i < nums.size(); i++)
		pq.push(nums[i]);

	for(int i = 0; i < k ; i++)
	{
		result.push_back(pq.top());
		pq.pop();
	}
	return result;
}


//Largest K numbers in an array 

vector<int> largestK(vector<int> nums , int k)
{
	priority_queue<int> pq;
	vector<int> result;
	
	for(int i = 0; i < nums.size(); i++)
		pq.push(nums[i]);
	
	for(int i = 0; i < k; i++)
	{
		result.push_back(pq.top());
		pq.pop();
	}
	return result;
}

/*****************************************************************************************************************************/

//Count number of 2's



















/********************************************************************************************************************/

//Volume of Histogram






















/********************************************************************************************************************/

// Largest Rectangle in Histogram

int largestRectangleArea(vector<int> histogram)
{
	stack<int> increasingHeights;
	int max_area = 0;

	for(int i = 0; i < histogram.size(); )
	{
		if(increasingHeights.empty() || i < histogram.size() && histogram[i] > increasingHeights.top())
		{
			increasingHeights.push(i);
			i++;
		}
		else
		{
			int h = histogram[increasingHeights.top()];
			increasingHeights.pop();
			int left = increasingHeights.empty() ? -1 : increasingHeights.top();
			max_area = max(max_area , h * (i - left - 1));
		}
	}
	return max_area;
}


/*************************************************************************************************************************/

//Continuous Median 
int continuousMedian(istringstream *sequence)
{
	priority_queue<int, vector<int>, greater<int>> min_heap;
	priority_queue<int, vector<int>, lesser<int>> max_heap;
	
	int x;
	//Add numbers to heaps
	while(*sequence >> x)
	{
		if(max_heap.empty() || x > max_heap.top())
			min_heap.push(x);
		else
			max_heap.push(x);
	}
	
	// Balance the heaps 
	if(min_heap.size() > max_heap.size() + 1)
	{
		max_heap.push(min_heap.top());
		min_heap.pop();	
	}
	else if(max_heap.size() > min_heap.size())
	{
		min_heap.push(max_heap.top());
		max_heap.pop();
	}

	cout << "The median is " <<  ( min_heap.size() == max_heap.size() ? 0.5 * (min_heap.top() + max_heap.top()) : min_heap.top()) << endl;
}

/*****************************************************************************************************************/

//Find 2 missing numbers



/****************************************************************************************************************/

//Find 2 repeating numbers





/***************************************************************************************************************/

//Find missing and repeating number







/***************************************************************************************************************/

// 
















































































