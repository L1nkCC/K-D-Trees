/*
// file: kd.cpp
// Class: CS 315
// Author: Connor Leslie
// Date : 3/31/2021
*/
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int b = 10;//bucket size, should be the length of arr.size()


//base node to create the Tree
//	isLeaf determines wheter the node holds children or 
//	a two dimensional vector for the points
struct dNode{
	int dim;
	int med;
	dNode *left;
	dNode *right;
	bool isLeaf = false;
	vector<vector<int>> bucket;
};



//partition for quickSelect
int partition(vector<vector<int>> &arr, int left, int right, int dim){
	int pivot = arr[right][dim];
	int index = left;
	vector<int> temp;
	//iterate
	for(int i =left; i < right; i++){
		//if on wrong side of pivot
		if(arr[i][dim] < pivot){
			//swap
			temp = arr[index];
			arr[index] = arr[i];
			arr[i] = temp;
			
			index++;
		}
	}
	//swap
	temp = arr[right];
	arr[right] = arr[index];
	arr[index] = temp;
	return index;
}

//QuickSelect: determines the ordinal position of given P
int quickSelect(vector<vector<int>> &arr, int left, int right, int dim, int p){
	
	if((p >0)&&( p <= right-left+1)){

		int index = partition(arr, left, right, dim);

		if(index-left == p-1){
			//done
			return arr[index][dim];
		}
		if(index -left >= p-1){
			//call on lower half
			return quickSelect(arr,left,index-1,dim,p);
		}
		//call on upper half
		return quickSelect(arr, index+1, right, dim, p-index-1+left);
		

	}else{
		cout << "ERROR: kthSmallest(vector<vector<int>>, int,int,int,int)"<<endl; 
		return INT_MIN;
	}


}

//wrapper func that eases QuickSelect for medians
int median(vector<vector<int>> &arr, int dim){
	return quickSelect(arr, 0, arr.size()-1, dim, arr.size()/2);
}

//determine the range given a dimension and the array
int findRange(vector<vector<int>> arr, int dim){
	int smallest = quickSelect(arr, 0 ,arr.size()-1,dim,1);
	int largest = quickSelect(arr, 0, arr.size()-1,dim, arr.size());
	return (largest-smallest);
}

//largestRangeDim()
//returns the dimension with the largest range
//returns -1 if Error occurs
int largestRangeDim(vector<vector<int>> &arr){
	int currRange = 0;
	int largestRange = 0;
	int largestRangeDim = -1;
	//iterate through
	for(long unsigned int d = 0; d < arr[0].size(); d++){
		//find it's range and compare to largestRange
		currRange = findRange(arr, d);
		if(currRange > largestRange){
			//set largestRange and largestDim
			largestRange = currRange;
			largestRangeDim = d;
		}
	}
	return largestRangeDim;
}

//func to ease splitting two dimensional vectors in half
vector<vector<int>> subVector(vector<vector<int>> arr, int start, int end){
	vector<vector<int>> temp (arr);
	auto first = temp.cbegin()+start;
	auto last = temp.cbegin()+end+1;
	vector<vector<int>> returnVal (first, last);
	return returnVal;
}


//recursively build tree
dNode* buildTree(vector<vector<int>> &arr){
	dNode* curr = new dNode();
	if(arr.size() > b){//bucket size is not small enough yet
		
		//get the dim to discriminate on
		int ndim = largestRangeDim(arr);
		//get the median in that dimension
		int nmed = median(arr,ndim);
		
		//set these in curr
		curr->dim = ndim;
		curr->med = nmed;
	
		//split the array's in half on median (median should have partitioned the
		//					vector's properly)
		int halfSize = (arr.size()-2)/2;
		vector<vector<int>> left = subVector(arr, 0, halfSize);
		vector<vector<int>> right = subVector(arr, halfSize+1,arr.size()-1);
		
		//with the new vectors build children
		curr->left = buildTree(left);
		curr->right = buildTree(right);
		return curr;
	}
	//bucket size meets size requirements
	//make curr a leaf
	curr->isLeaf = true;
	curr->bucket = arr;
	return curr;


}

//print the node as a leaf if isLeaf is true
void printLeaf(dNode* posLeaf){
	//passed node is not a leaf;
	if(!posLeaf->isLeaf){
		cout <<"ERROR: printLeaf didn't recieve leaf\n";
		return;
	}
	//go through entire two dimensional vector printing it's contents
	for(long unsigned int i = 0; i < posLeaf->bucket.size();i++){
		for(long unsigned int j = 0; j < posLeaf->bucket[i].size(); j++){
			cout << posLeaf->bucket[i][j] << " ";
		}
		cout << endl;
	}
}

//recursively scale tree and print the leaf it was found in
void probeTree(dNode* branch, vector<int> probe){
	if(branch == NULL){
		cout << "ProbeTree was given Null branch\n";
		return;
	}
	//it is a leaf
	if(branch->isLeaf){
		cout << "probe ";
		for(long unsigned int i = 0; i <probe.size(); i++){
			cout << probe[i]<< " ";
		}
		cout << "reaches bucket: " << branch->bucket.size() << " elements" << endl;
		printLeaf(branch);
	//it is not a leaf
	}else{
		if(probe[branch->dim] > branch ->med){
			probeTree(branch->right, probe);
		}else{
			probeTree(branch->left, probe);
		}
	}
}

//allow for input and run algothrim
int main(int argc, char* argv[]){
	int k = atoi(argv[1]);
	int n = atoi(argv[2]);
	int p = atoi(argv[3]);
	vector<vector<int>> points;
	long unsigned int i = 0;
	long unsigned int j = 0;
	
	//accept point inputs
	for(i = 0; i < (long unsigned int)n; i++){
		vector<int> temp;
		for(j = 0; j < (long unsigned int)k; j++){
			int coord;
			cin >> coord;
			temp.push_back(coord);
		}
		points.push_back(temp);
	}
	
	//build tree
	dNode* tip = buildTree(points);
	
	//accept probe inputs
	for(i = 0; i < (long unsigned int)p; i++){
		vector<int> probe;
		for(j= 0; j < (long unsigned int)k; j++){
			int coord;
			cin >> coord;
			probe.push_back(coord);
		}
		probeTree(tip, probe);
	}
	return 0;
}



