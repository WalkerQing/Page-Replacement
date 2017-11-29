#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
using namespace std;

//Sorry for my spaghetti code this was done when I should have been asleep.
//Also '<': signed/unsigned mismatch is a warning and not an error for a reason so who cares

void fifo(vector<int> data, int numPages) {
	//I honestly could have used a real queue, but I was copy pasting from my other algorithms.
	cout << "Algorith: FIFO " << endl;
	vector<int> pageData;
	vector<int> order;
	//pf = page faults
	int pf = 0;
	bool found = false;
	int first = 0;

	for (int i = 0; i < data.size(); i++) {
		found = false;
		cout << "Number in sequence: " << data[i] << ". Numbers in pages: { ";
		//Just check to see if number is in page already
		for (int j = 0; j < pageData.size(); j++) {
			cout << pageData[j] << " ";
			if (data[i] == pageData[j]) {
				found = true;
			}
		}
		//Failed attempt at fixing spacing, i've since given up.
		if (i == 0 || i == 1) {
			cout << "} \t\tResult: ";
		}
		else {
			cout << "} \tResult: ";
		}
		if (!found) {
			cout << "Page fault. ";
			//If pages aren't full, fill empty pages
			if (pageData.size() < numPages) {
				pageData.push_back(data[i]);
				order.push_back(i);
				pf++;
				cout << endl;
			}
			else {
				first = numPages - 1;
				pf++;
				//Check for oldest page
				for (int x = 0; x < order.size(); x++) {
					if (order[x] < order[first])
						first = x;
				}
				//Replace oldest page
				cout << "Replacing: " << pageData[first] << endl;
				pageData[first] = data[i];
				order[first] = numPages - 1;
				//Reduce age on pages
				for (int x = 0; x < order.size(); x++) {
					--order[x];
				}
			}
		}
		else {
			cout << "Found" << endl;
		}
	}

	cout << "Number of faults: " << pf << endl;
	cout << "Page fault rate: " << pf << "/" << data.size() << endl;
}

void lru(vector<int> data, int numPages) {
	//Pretty much everything is the same.
	cout << "Algorith: LRU " << endl;
	vector<int> pageData;
	vector<int> order;
	int pf = 0;
	bool found = false;
	int age = 0;
	//Up the age on each page
	for (int i = 0; i < data.size(); i++) {
		for (int x = 0; x < order.size(); x++) {
			++order[x];
		}
		found = false;
		cout << "Number in sequence: " << data[i] << ". Numbers in pages: { ";
		for (int j = 0; j < pageData.size(); j++) {
			cout << pageData[j] << " ";
			if (data[i] == pageData[j]) {
				//Refresh page age if found
				order[j] = 0;
				found = true;
			}
		}
		if (i == 0 || i == 1) {
			cout << "} \t\tResult: ";
		}
		else {
			cout << "} \tResult: ";
		}
		if (!found) {
			cout << "Page fault. ";
			if (pageData.size() < numPages) {
				pageData.push_back(data[i]);
				order.push_back(0);
				pf++;
				cout << endl;
			}
			else {
				//Not sure why I do this assignment tbqh
				age = numPages - 1;
				pf++;
				//Look for the oldest page.
				for (int x = 0; x < order.size(); x++) {
					if (order[x] > order[age])
						age = x;
				}
				cout << "Replacing: " << pageData[age] << endl;
				pageData[age] = data[i];
				order[age] = 0;
			}
		}
		else {
			cout << "Found" << endl;
		}
	}
	cout << "Number of faults: " << pf << endl;
	cout << "Page fault rate: " << pf << "/" << data.size() << endl;
}

//Honestly everything about OPT is ugly because I tried troubleshooting tons of stuff, but the problem was I had == and not an = at one point giving a bad logic error.
int findNext(vector<int> data, int target, int curr) {
	int distance = 1;
	for (int i = curr + 1; i < data.size(); i++) {
		if (target == data[i])
			return distance;
		distance++;
	}
	return INT_MAX;
}

void opt(vector<int> data, int numPages) {
	cout << "Algorith: OPT " << endl;
	vector<int> pageData;
	vector<int> order;
	int pf = 0;
	bool found = false;
	int closest = 0;
	for (int i = 0; i < data.size(); i++) {
		//Counts down how close next repeat is for each number in the page table.
		for (int j = 0; j < order.size(); j++) {
			--order[j];
		}
		found = false;
		cout << "Number in sequence: " << data[i] << ". Numbers in pages: { ";
		for (int j = 0; j < pageData.size(); j++) {
			cout << pageData[j] << " ";
			if (data[i] == pageData[j]) {
				//If its found refresh when the next time it shows up is.
				order[j] = findNext(data, pageData[j], i);
				found = true;
			}
		}
		if (i == 0 || i == 1) {
			cout << "} \t\tResult: ";
		}
		else {
			cout << "} \tResult: ";
		}
		if (!found) {
			cout << "Page fault. ";
			pf++;
			if (pageData.size() < numPages) {
				pageData.push_back(data[i]);
				order.push_back(findNext(data,data[i],i));
				cout << endl;
			}
			else {
				closest = 0;
				for (int x = 0; x < order.size(); x++) {
					if (order[x] > order[closest]) {
						closest = x;
					}
				}
				cout << "Replacing: " << pageData[closest] << endl;
				pageData[closest] = data[i];
				order[closest] = findNext(data,pageData[closest],i);
			}
		}
		else {
			cout << "Found" << endl;
		}
	}
	cout << "Number of faults: " << pf << endl;
	cout << "Page fault rate: " << pf << "/" << data.size() << endl;
}

int main() {
	srand(time(NULL));
	int numPages, numData;
	cout << "What is the length of inputer number sequence: ";
	cin >> numData;
	cout << "How many pages are there: ";
	cin >> numPages;


	vector<int> data;
	cout << "Number sequence: ";
	//Randomly generating numbers between 1-numData
	for (int i = 0; i < numData; i++) {
		int temp = (rand() % numData) + 1;
		cout << temp << " ";
		data.push_back(temp);
	}

	cout << endl << endl;
	fifo(data, numPages);
	cout << endl;
	lru(data, numPages);
	cout << endl;
	opt(data, numPages);

	//I dont like system("pause")
	cin.get();
	cin.get();
	return 0;
}