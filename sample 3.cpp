#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;


int main() 
{
	for(int i = 0; i < 100000; i++){
		cout << i;
	}
	return 0;
}
