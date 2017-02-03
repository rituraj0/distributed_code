#include "sandwich.h"
#include <message.h>

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <map>
#include <set>
using namespace std;
#define pb push_back
#define mp make_pair
#define bg begin()
#define en end()
#define Y second
#define X first
typedef long long ll;

#define fi freopen("input.txt","r",stdin)
#define fo freopen("output.txt","w",stdout)

const double pi     =   acos(-1.0);
const double eps    =   1e-8;

#define print(a) cout<<(#a)<<" = "<<a<<"\n";
#define fill(a,val) memset(a ,val, sizeof(a) );
#define sz(s) ((int)(s.size()))

pair<long long, long long> getStartAndEndIndex(long long currNodeId) {
	long long nodeCount = NumberOfNodes();
	long long inputCount = GetN();
	long long start = (inputCount * currNodeId)/nodeCount;
	long long endIndex = min( inputCount, (inputCount * (currNodeId + 1))/nodeCount );
	return make_pair(start, endIndex);
}

void slaveProcess() {
	int currNodeId = MyNodeId();
	pair<int, int> currRange = getStartAndEndIndex(currNodeId);

	long long maxPrefix = -(1LL<<62);
	long long minPrefix = (1LL<<62);
	long long totalSum = 0;
	long long miniSubstring = (1LL<<62);

	// find left max 
	long long miniSubstringEndingHere = 0;

	for(int i=currRange.first; i < currRange.second; i++) {
		long long tasteHere = GetTaste(i);
		
		// update miniSubstring
		miniSubstringEndingHere += tasteHere;
		miniSubstring = min(miniSubstring, miniSubstringEndingHere);
		if(miniSubstringEndingHere > 0) {
			miniSubstringEndingHere = 0;
		}

		// update
		totalSum += tasteHere;

		maxPrefix = max(maxPrefix, totalSum);

		minPrefix = min(minPrefix, totalSum);
	}

	long long minSuffix = min(totalSum, totalSum - maxPrefix);
	minPrefix = min(minPrefix, 0LL);
	minSuffix = min(minSuffix, 0LL);
	miniSubstring = min(miniSubstring, 0LL);

	// send all data to master
	PutLL(0, totalSum);
	PutLL(0, minPrefix);
	PutLL(0, minSuffix);
	PutLL(0, miniSubstring);
	//cout<<currNodeId<<" "<<currRange.first<<" "<<currRange.second<<endl;
	//cout<<currNodeId<<" : "<<totalSum<<" "<<minPrefix<<" "<<minSuffix<<" "<<miniSubstring<<endl;
    Send(0);
}

void masterProcess() {
	int currNodeId = MyNodeId();

	if(currNodeId != 0) {
		// this node is not a master node
		return;
	}

	int nodeCount = NumberOfNodes();

	long long totalSum, minPrefix, minSuffix, miniSubstring;

	long long currMaxNeg = 0;
	long long allNodeSum = 0;

	long long ans = (1LL<<62);

	for(int nodeId=0; nodeId < nodeCount; nodeId++) {
		Receive(nodeId);
		totalSum = GetLL(nodeId);
		minPrefix = GetLL(nodeId);
		minSuffix = GetLL(nodeId);
		miniSubstring = GetLL(nodeId);

		//cout<<"Got "<<nodeId<<" "<<totalSum<<" "<<minPrefix<<" "<<minSuffix<<" "<<miniSubstring<<endl;
		// update allNodeSum
		allNodeSum += totalSum;
		// take minPrefix
		ans = min(ans, currMaxNeg + minPrefix);
		// take miniSubstring
		ans = min(ans, miniSubstring);
		ans = min(ans, minPrefix);
		ans = min(ans, minSuffix);
		// update for next node
		currMaxNeg = currMaxNeg + totalSum;
		currMaxNeg = min(currMaxNeg, minSuffix);
		currMaxNeg = min(currMaxNeg, totalSum);
	}

	cout<<allNodeSum - ans<<endl;

	//cout<<"Final answer is "<<allNodeSum - ans<<endl;

}
int main() {
	
	slaveProcess();
	
	masterProcess();
	
	return 0;
}