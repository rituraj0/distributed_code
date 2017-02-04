#include "majority.h"
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

void slaveProcessToFindLocalMajority() {
	int currNodeId = MyNodeId();
	pair<int, int> currRange = getStartAndEndIndex(currNodeId);

	long long localMaximum = -1;// 0 ≤ GetVote(i) ≤ 109 
	long maximumCount = 1;

	long long currVote;

	for(int i = currRange.first; i < currRange.second; i++) {
		currVote = GetVote(i);

		if(maximumCount == 0) {
			localMaximum = currVote;
		}

		if(currVote != localMaximum) {
			maximumCount--;
		} else {
			maximumCount++;
		}
	}

	// send all data to master
	PutLL(0, localMaximum);
	Send(0);
	//cout<<currNodeId<<" "<<currRange.first<<" "<<currRange.second<<endl;
	//cout<<currNodeId<<" : "<<totalSum<<" "<<minPrefix<<" "<<minSuffix<<" "<<miniSubstring<<endl;
    
}


void masterProcessToAggretaeAllLocalMajority() {
	int currNodeId = MyNodeId();

	if(currNodeId != 0) {
		// this node is not a master node
		return;
	}

	int nodeCount = NumberOfNodes();

	vector<long long> allLocalMajority;

	long long number, numberCount;

	for(int nodeId=0; nodeId < nodeCount; nodeId++) {
		Receive(nodeId);
		allLocalMajority.push_back(GetLL(nodeId));
	}

	for(int nodeId=0; nodeId < nodeCount; nodeId++) {
		for(auto local : allLocalMajority) {
			PutLL(nodeId, local);
		}
		Send(nodeId);
	}

}

void slaveProcessToCountAllPotentialMajority() {
	int currNodeId = MyNodeId();
	pair<int, int> currRange = getStartAndEndIndex(currNodeId);

	long long nodeCount = NumberOfNodes();

	long long currVote;

	map< long long, long long > contOfLocalMaximums;

	// Receive nodeCount message from mater
	Receive(0);

	for(int i=0; i < nodeCount; i++) {
		contOfLocalMaximums[ GetLL(0) ] = 0;
	}

	for(int i = currRange.first; i < currRange.second; i++) {
		currVote = GetVote(i);
		if(contOfLocalMaximums.find(currVote) != contOfLocalMaximums.end()) {
			contOfLocalMaximums[currVote]++;
		}
	}

	for(auto it : contOfLocalMaximums) {
		PutLL(0, it.first);
		PutLL(0, it.second);
	}
	Send(0);
}

void masterProcessToTakeFinalDecision() {
	int currNodeId = MyNodeId();

	if(currNodeId != 0) {
		// this node is not a master node
		return;
	}

	int nodeCount = NumberOfNodes();

	map<long long, long long> allCount;

	long long number, numberCount;

	cout<<"masterProcessToTakeFinalDecision\n"<<endl;

	for(int nodeId=0; nodeId < nodeCount; nodeId++) {
		Receive(nodeId);

		cout<<"Received from "<<nodeId<<endl;

		for(int message=0; message < nodeCount; message++) {
			number = GetLL(nodeId);
			numberCount = GetLL(nodeId);
			cout<<number<<" "<<numberCount<<endl;
			allCount[number] += numberCount;
		}
	}

	long long inputCount = GetN();

	for(auto keyValue : allCount) {
		if(keyValue.second >= (inputCount + 1)/2LL) {
			cout<<keyValue.first<<endl;
			return;
		}
	}
	cout<<"NO ANSWER"<<endl;
}

int main() {
	
	slaveProcessToFindLocalMajority();
	
	masterProcessToAggretaeAllLocalMajority();

	slaveProcessToCountAllPotentialMajority();

	masterProcessToTakeFinalDecision();
	
	return 0;
}