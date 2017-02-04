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
	long maximumCount = 0;

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

	vector<long long> allLocalMajority;

	long long temp;

	for(int i=0; i < nodeCount; i++) {
		temp = GetLL(0);
		allLocalMajority.push_back(temp);
		contOfLocalMaximums[temp] = 0;
	}

	for(int i = currRange.first; i < currRange.second; i++) {
		currVote = GetVote(i);
		if(contOfLocalMaximums.find(currVote) != contOfLocalMaximums.end()) {
			contOfLocalMaximums[currVote]++;
		}
	}

	//cout<<"Sent by"<<currNodeId<<" "<<allLocalMajority.size()<<endl;

	for(auto local : allLocalMajority) {
		PutLL(0, local);
		PutLL(0, contOfLocalMaximums[local]);
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

	//cout<<"masterProcessToTakeFinalDecision\n"<<endl;

	for(int nodeId=0; nodeId < nodeCount; nodeId++) {
		Receive(nodeId);
		for(int message=0; message < nodeCount; message++) {
			number = GetLL(nodeId);
			numberCount = GetLL(nodeId);
			//cout<<"Received from "<<nodeId<<" "<<number<<" "<<numberCount<<endl;
			allCount[number] += numberCount;
		}
	}

	long long inputCount = GetN();

	cout<<"MY answer ";
	for(auto keyValue : allCount) {
		if(keyValue.second >= (inputCount + 1)/2LL) {

			cout<<keyValue.first<<endl;
			return;
		}
	}
	cout<<"NO WINNER"<<endl;
}

int main() {
	
	slaveProcessToFindLocalMajority();
	
	masterProcessToAggretaeAllLocalMajority();

	slaveProcessToCountAllPotentialMajority();

	masterProcessToTakeFinalDecision();
	
	return 0;
}


/*
My solution:
Could have done better if instead of counting all local maximum, get local maximum of local maximum at node 0
and send it to slaves to count again,
in same way correct_solution does

*/

/*

 // Merge, candidate generation, and resubmit
  if (MyNodeId() == 0) {
    ll k = -1, n = 0;
    for (int i = 0; i < NumberOfNodes(); ++i) {
      Receive(i);
      ll tk = GetLL(i);
      ll tn = GetLL(i);

      if (k == tk) {
        n += tn;
      } else {
        n -= tn;
        if (n < 0) {
          k = tk;
          n = -n;
        }
      }
    }

    for (int i = 0; i < NumberOfNodes(); ++i) {
      PutLL(i, k);
      Send(i);
    }
  }

  // Count and confirm
  {
    Receive(0);
    ll k = GetLL(0), n = 0;
    // printf("k = %lld\n", k);
    for (ll i = MyNodeId(); i < GetN(); i += NumberOfNodes()) {
      ll x = GetVote(i);
      if (x == k) ++n;
    }
    PutLL(0, n);
    Send(0);

    if (MyNodeId() == 0) {
      ll n = 0;
      for (int i = 0; i < NumberOfNodes(); ++i) {
        Receive(i);
        n += GetLL(i);
      }

      if (n * 2 > GetN()) printf("%lld\n", k);
      else puts("NO WINNER");
    }
  }


  */