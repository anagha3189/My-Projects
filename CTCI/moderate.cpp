#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

 // Number swapping without using temporary variable
void numswap (unsigned int &a , unsigned int &b)
{
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}

int main()
{
	unsigned int a = 5;
	unsigned int b = 10;
	cout << "The numbers before swapped are " << a << "  " << b << endl;
	numswap(a, b);
	cout << "The numbers after swapped are " << a << "  " << b << endl;
	return 0;
}
/**********************************************************************************/


//Word Frequencies




