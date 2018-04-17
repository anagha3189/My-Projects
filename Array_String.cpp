#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stack>
using namespace std;

/* Removes recurring digits in num[] */
string removeRecurringDigits(string num)
{
	int len = num.length();
	int j = 0; // Index in modified string
	string result;
	for (int i=0; i<len; i++)
	{
		result  += num[i];
		while (i + 1 < len && num[i] == num[i+1])
			i++;
	}
	return result;
}

//Find the max depth of nested parenthesis
int maximumDepthNestedParenthesis(string str)
{
    int currentMax = 0;
    int max = 0;
    int n = str.length();
    
    for(int i = 0; i < n ; i++)
    {
        if(str[i] == ')')
        {
            currentMax++;
            if(max < currentMax)
            {
                max = currentMax;
            }
        }
        else if(str[i] == '(')
        {
            if(currentMax > 0)
                currentMax--;
            else
                return -1;
        }
    }
    
    if(currentMax != 0)
        return -1;
    else
        return max;
}

//Sort array of strings

//Group anagrams together
vector<vector<string>> groupAnagrams(vector<string> input)
{
    vector<vector<string> > result;
    map<string, vector<string> > hmap;
    map<string, vector<string> >::iterator it;
    for(string str : input)
    {
        string t = str;
        sort(t.begin(), t.end());
        hmap[t].push_back(str);
    }
    
    for(it= hmap.begin(); it!=hmap.end(); it++)
    {
        result.push_back(it->second);
    }
    return result;
}

//All combination of strings that can be used t dial a number
vector<string> keypad {"  ", " ", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
void phoneHelper(vector<string> &result, string out, int curr, string phone)
{
    if(curr == phone.size())
    {
        result.push_back(out);
        return;
    }
    
    for(auto &ch : keypad[phone[curr] - '0'])
    {
        out.push_back(ch);
        phoneHelper(result, out, curr+1 , phone);
        out.pop_back();
    }
}


vector<string> phoneCombStrings(string phoneNumber)
{
    if(phoneNumber.empty())
    return {};
    string out;
    vector<string> result;
    phoneHelper(result, out, 0, phoneNumber);
    return result;
}


//Run Length Encoding
string runLengthEncoding(const string s) 
{       
    string result;
    int count  = 1;
    for(int i = 1; i <= s.length(); i++)
    {
        if(i == s.length() || s[i] != s[i-1])
        {
            result+= to_string(count) + s[i-1];
            count = 1;
        }           
        else
        {
            count++;
        }
    }
    return result;
}

// Add Binary strings
string addBinary(string a, string b) 
{
    string res;
    int res_len = max(a.length() , b.length());
        
    size_t carry = 0;
    for(int i = 0; i < res_len ; i++)
    {
        size_t a_bit = i < a.length() ? a[a.length() - i - 1]-'0' : 0;
        size_t b_bit = i < b.length() ? b[b.length() - i - 1]-'0' : 0;
           
       int sum = carry + a_bit + b_bit;
       carry = sum / 2;
       sum = sum % 2;
       res.push_back(sum + '0');
   }
        
   if(carry)
       res.push_back(carry + '0');
   reverse(res.begin() , res.end());
   return res;
}


//Zigzag order of string in n rows
string convert(string s, int numRows) 
{    
    if (numRows == 1) 
	{
        return s;
    }
    // The number of steps neeeded to reach a charcter on the same row. // Only for first and last  
    const int step = 2 * numRows - 2; 
    string zigzag;		
    for (int i = 0; i < numRows; ++i) 
	{
        for (int j = i; j < s.length(); j += step) //Important
		{
            zigzag.push_back(s[j]);   
            // the number of steps needed for rows between first and last
            if (0 < i && i < numRows - 1 && j + step - 2 * i < s.length()) 
			{
				zigzag.push_back(s[j + step - 2 * i]);
			}
        }
    }
    return zigzag;    
}

// Excel column string into number 
int columnstr_num(string columnstr)
{
    int result = 0;
    for (char &c : columnstr)
    {
        result = result * 26 + c - 'A' + 1;
    }
    return result;
}

//Excel column number to  string
string columnnum_str(int number)
{
    string result = " " ;
    while(number > 0)
    {
        result += (number-1)%26 + 'A';
        number = (number-1)/26;
    }
    reverse(result.begin(), result.end());
    return result;
}

//Word Abbreviation



// Implement strstr
int my_strStr(string haystack , string needle)
{
    if(needle.empty())
        return 0;
    int m = haystack.size();
    int n = needle.size();
    
    for(int i = 0; i < m-n; i++)
    {
        int  j = 0;
        for(; j < n ; j++)
        {
            if(haystack[i+j] != needle[j])
                break;
        }
        
        if(j == n)
            return i;
    }
    
    return -1;
}

// First Non repeating charcater
char firstNonRepeating(string str)
{
    int n = str.length();
    map<char, int> hmap;
    int min_index = numeric_limits<int>::max();
    
    for(int i = n-1; i>= 0; i--)
    {
        if(hmap.find(str[i]) == hmap.end())
        {
            hmap[str[i]] = i;
            min_index = min(min_index , i);
        }
    }
    return str[min_index];
}


//Balance parenthesis
bool isValid(string s) 
{      
    stack<char> st;
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == '(' || s[i] == '[' || s[i] == '{')
            st.push(s[i]);
        else
        {
            if(st.empty())
                return false;
            if( (s[i] == ')' && st.top() != '(') || (s[i] == ']' && st.top() != '[') || (s[i] == '}' && st.top() != '{') )
            {
                return false;
            }
            st.pop();
        }
    }
        
    if(!st.empty())
        return false;
    else
        return true;
}



//Reverse Vowels
bool is_vowel(char ch)
{
    string vowel = "aeiou";
    return vowel.find(ch)!= string::npos;
}

// Reverse vowels
string reverseVowels(string str)
{
    int i = 0; 
    int j = str.length()-1;
    
    while(i < j)
    {
        if(!is_vowel(tolower(str[i])))
            i++;
        else if(!is_vowel(tolower(str[j])))
            j--;
        else
        {
            swap(str[i], str[j]);
            i++;
            j--;
        }
        
    }
}
    
//Check if anagrams    
bool areAnagrams(string s1 , string s2)
{
    if(s1.length() != s2.length())
    return false;
    
    map<char, int> hmap ;
    map<char, int>::iterator it;
    
    for(char &c : s1)
    {
        hmap[c]++;
    }
    
    for(char &c : s2)
    {
        hmap[c]--;
        if(hmap[c] < 0)
        return false;
    }
    
    for(it = hmap.begin() ; it != hmap.end(); it++)
    {
        if(it->second != 0)
        return false;
    }
    return true;
}


bool areKAnagrams(string s1, string s2, int k)
{
    if(s1.length() != s2.length())
    return false;
    
    map<char, int> hmap ;
    map<char, int>::iterator it;
    
    for(char &c : s1)
    {
        hmap[c]++;
    }
    
    int count = 0;
    for(char &c : s2)
    {
        if(hmap[c] > 0)
            hmap[c]--;
        else
            count++;
        
        if(count > k)
            return false;
    }
    
    return true;
    
}


bool isPalindrome(string str)
{
    int l = 0;
    int h = str.length()-1;
    
    while(h > l)
    {
        if(str[h--] != str[l++])
        {
            cout << "Not a Palindrome" << endl;
            return false;
        }
    }
    cout << "is a palindrome" << endl;
    return true;
}


//print permutation of a given string 
vector<string> permutation(string str)
{
    sort(str.begin() , str.end());
    vector<string> result;
    do
    {
        result.push_back(str);
    }while(next_permutation(str.begin() , str.end()));
    return result;
}

bool DupFunction(int i , int j)
{
    return (i == j);
}

//Remove adjacent duplicates in array
void removeAdjacentDuplicates(vector<int> &arr)
{
    vector<int>::iterator it = unique(arr.begin() , arr.end() , DupFunction);
    arr.resize(distance(arr.begin() , it));
}


//Remove duplicates in an array
void removeDuplicates(vector<int> &arr)
{
    sort(arr.begin() , arr.end());
    vector<int>::iterator it =  unique(arr.begin() , arr.end() , DupFunction);
    arr.resize(distance(arr.begin() , it));
}

// Generate Subsets
void subsetHelper(vector<string> &res, string &out, int start, string ip)
{
	res.push_back(out);
	for(int i = start; i < ip.length(); i++)
	{
		out += ip[i];
		subsetHelper(res, out, i+1, ip);
		out.pop_back();
	}
}

vector<string> generateSubsets(string ip)
{
	sort(ip.begin(), ip.end());
	string out;
	vector<string> result; 
	subsetHelper(result, out, 0, ip);
	return result;
}

//Generate subsets with duplicates
void subsetDupHelper(vector<string> &result, string &out, int start, string input)
{
	result.push_back(out);
	for(int i = start; i < input.length(); i++)
	{
		if(i == start || input[i] != input[i-1])
		{
			out.push_back(input[i]);
			subsetDupHelper(result, out, i+1 , input);
			out.pop_back();
		}
	}
}

vector<string> generateSubsetsDup(string input)
{
	sort(input.begin(), input.end());
	string out; 
	vector<string> result;
	subsetDupHelper(result, out, 0, input);
	return result;
}

//Coin change -> Given different coins provided min no of coins are needed to form the required amount
int coinChange(vector<int> coins , int amount)
{
	vector<int> (amount+1 , amount+1);
	dp[0] = 0;
	for(int i = 1; i <= amount ; i++)
	{
		for(int j = 0; j < coins.size(); j++)
		{
			if(coin[j] <= i)
			   dp[i] = min(dp[i], dp[i - coins[j]] + 1);
		}
	}
	return dp[amount] > amount ? -1 : dp[amount];
}



//Coin change -> Given the different coins provide the number of combinations to to form the amount  
int coinCombinations(int amount , vector<int> coins)
{
	vector<int> (amount+1);
	dp[0] = 1;
	for(const auto& coin : coins)
	{
		for(int i = coin; i <= amount; i++)
			dp[i] += dp[i-coin];
	
	} 
	return dp[amount];
}


//Number of combinations for final 
int numCombinationsForFinalScore(int final_score , vector<int>scores)
{
	//Combinations Matrix
	vector<vector<int> > numCombinations (scores.size, vector<int>(final_score+1 , 0));
	for(int i = 0; i < scores.size(); i++)
	{
		numCombinations[i][0] = 1;
		for(int j = 1; j <= final_score; j++)
		{
			without_this_play = i>=1 ? numCombinations[i-1][j] : 0;
			with_this_play = j>=scores[i] ? numCombinations[i][j-scores[i]] : 0; 
			numCombinations[i][j] = without_this_play + with_this_play;
		}
	}
	return numCombinations.back().back();
}



/* Driver program to test above function */
int main()
{
	string num = "1299888833";
	string result = removeRecurringDigits(num);
	cout << "Modified number is " << result << endl;
	
	vector<string> arr {"GeeksforGeeks", "GeeksQuiz", "CLanguage"};
	sort(arr.begin(), arr.end());
	vector<string> input {"eat", "tea", "tan", "ate", "nat", "bat"};
	vector<vector<string>> res = groupAnagrams(input);
	
	for(int i = 0; i < res.size(); i++)
	{
	    for(int j = 0 ; j < res[i].size(); j++)
	    cout << res[i][j] <<"\t";
	    cout << "\n";
	}
	
	vector<string> phoneCombi = phoneCombStrings("62");
	for(int i = 0; i < phoneCombi.size(); i++)
	cout << phoneCombi[i] << "\t";
	cout << "\n";
	char nonRepeatChar = firstNonRepeating("geeksforgeeks");
	cout << "The first non repeating character " << nonRepeatChar <<endl;
	
	cout << "Column number to string" << endl;
	cout << columnnum_str(26);
	cout << columnnum_str(51);
	cout << columnnum_str(52);
	cout << columnnum_str(80);
	cout << columnnum_str(676);
	cout << columnnum_str(705);
	
	cout << "Column string to number" <<endl;
	cout << columnstr_num("Z") << "\t";
	cout << columnstr_num("AY") <<"\t";
	cout << columnstr_num("AZ") << "\t";
	cout << columnstr_num("YZ") << "\t";
	cout << columnstr_num("CB") << "\t";
	cout << columnstr_num("AAC") << endl;
	
	if(areAnagrams("geeksforgeeks", "forgeeksgeeks"))
	cout << "Are anagarams" << endl;
	else
	cout << "Are not anagrams" << endl;
	
	
	if(areAnagrams("listens", "silent"))
	cout << "Are anagarams" << endl;
	else 
	cout << "Are not anagrams" << endl;
	
	string str1 = "fodr";
    	string str2 = "gork";
    	int k = 2;
    	if (areKAnagrams(str1, str2, k) == true)
        cout << "Yes. are K anagrams" << endl;
    	else
        cout << "No. are not k anagrams" <<endl;
    
    	cout << "Permuation of string " <<endl;    
        vector<string> permResult = permutation("CBA");
    	for (vector<string>::iterator it=permResult.begin(); it!=permResult.end(); ++it)
    	cout << ' ' << *it;
    	cout << '\n';
	
	cout << "Unique Permutation of a string with duplicate characters" <<endl;
	vector<string> permDupResult = permutation("AAB"); 
    	for (vector<string>::iterator it=permDupResult.begin(); it!=permDupResult.end(); ++it)
    	cout << ' ' << *it;
    	cout << '\n';
	
		
	vector<int> arr1 {10, 20, 20, 20, 30, 30, 20, 20 , 10};
	removeAdjacentDuplicates(arr1);
	cout << "myvector contains:";
    	for (vector<int>::iterator it=arr1.begin(); it!=arr1.end(); ++it)
    	cout << ' ' << *it;
    	cout << '\n';

	vector<int> arr2 {10, 20, 20, 20, 30, 30, 20, 20 , 10};
	removeDuplicates(arr2);
	cout << "myvector contains:";
    	for (vector<int>::iterator it=arr2.begin(); it!=arr2.end(); ++it)
    	cout << ' ' << *it;
    	cout << '\n';

	vector<string> subsets = generateSubsets("ABC");
	cout << "Subsets are:" << endl;
    	for (vector<string>::iterator it=subsets.begin(); it!=subsets.end(); ++it)
    	cout << ' ' << *it;
    	cout << '\n';
	
	vector<string>subsetsDup = generateSubsetsDup("ABB");
	cout << "The subsets with duplicate are:" << endl;
	for (vector<string>::iterator it=subsetsDup.begin(); it!=subsetsDup.end(); ++it)
    	cout << ' ' << *it;
    	cout << '\n';
	

	return 0;
}

