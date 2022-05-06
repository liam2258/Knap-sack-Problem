#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
using namespace std;

//Genome is (Weight, Value)

//Function to find weight of Genome
int weight(vector<int> Pop, vector<vector<int>> Items) {
	int weight = 0;
	for (int i = 0; i < 30; i++) {
		if (Pop[i] == 1) {
			weight += Items[i][0];
		}
	}
	return weight;
}

//Function to find value of Genome
int value(vector<int> Pop, vector<vector<int>> Items) {
	int value = 0;
	for (int i = 0; i < 30; i++) {
		if (Pop[i] == 1) {
			value += Items[i][0];
		}
	}
	return value;
}

//Function to find if Genome goes over weight limit
bool Limit(vector<int> Pop, int limit, vector<vector<int>> Items) {
	if (weight(Pop, Items) > limit) {
		return false;
	}
	return true;
}

//Randomly generates a population of 20 genomes
vector<vector<int>> PopGen(int limit, vector<vector<int>> Items) {
	vector<vector<int>> Population;
	for (int i = 0; i < 20; i++) {
		vector<int> Pop;
		for (int i = 0; i < 30; i++) {
			Pop.push_back(rand() % 2);
		}
		if (Limit(Pop, limit, Items) == true) {
			Population.push_back(Pop);
		}
		else {
			i--;
		}
	}
	return Population;
}

//Displays a list of items as (Weight, Value)
void ItemsViewer(vector<vector<int>> Items) {
	for (int i = 0; i < 30; i++) {
		cout << "(" << Items[i][0] << ", " << Items[i][1] << ")" << endl;
	}
}

//Create 30 potential items to go in knap-sack with a value of 1-20 and weight of 1-20
vector<vector<int>> ItemGen() {
	vector<vector<int>> Items;
	for (int i = 0; i < 30; i++) {
		vector<int> Item;
		Item.push_back(rand() % 70 + 1);
		Item.push_back(rand() % 70 + 1);
		Items.push_back(Item);
	}
	return Items;
}

//Invert mutation function
void invert(vector<int>& Pop) {
	vector<int> A;
	vector<int> B;
	for (int i = 0; i < 30; i++) {
		A.push_back(Pop[i]);
	}
	int a = rand() % 30;
	int b = rand() % 30;
	int c = a;
	while (b == a || (b == a + 1) || (b == a - 1) || (b == a - 29) || (b == a + 29)) {
		b = rand() % 30;
	}
	int k = -1;
	while (a != b) {
		B.push_back(Pop[a]);
		k++;
		if (a == 29) {
			a = 0;
		}
		else {
			a++;
		}
	}
	c++;
	while (k > 0) {
		if (c > 29) {
			c = 0;
		}
		Pop[c] = B[k];
		k--;
		c++;
	}
}

//Order Crossover Function (Fixed!)
vector<int> Cross(vector<int> A, vector<int> B) {
	vector<int> A2;
	vector<int> B2;
	for (int i = 0; i < 30; i++) {
		A2.push_back(A[i]);
		B2.push_back(B[i]);
	}
	int a = rand() % 30;
	int b = rand() % 30;
	while (b == a || (b == a + 1) || (b == a - 1) || (b == a - 29) || (b == a + 29)) {
		b = rand() % 30;
	}

	while (a != b) {
		A2[a] = B2[a];
		if (a == 29) {
			a = 0;
		}
		else {
			a++;
		}
	}
	return A2;
}

//Greater then and smaller then may need to be reversed.
void fitnessOrganizer(vector<vector<int>>& Population, vector<vector<int>> Items) {
	deque<vector<int>> Population2;

	for (int i = 0; i < 20; i++) {
		if (i == 0) {
			Population2.push_back(Population[i]);
		}
		else if (value(Population2[i - 1], Items) < value(Population[i], Items)) {
			for (int k = 0; k < i; k++) {
				if (value(Population2[k], Items) < value(Population[i], Items)) {
					deque<vector<int>>::iterator it = Population2.begin();
					for (int h = 0; h < k; h++) {
						++it;
					}
					Population2.insert(it, Population[i]);
					break;
				}
			}
			
		}
		else {
			Population2.push_back(Population[i]);
		}
	}

	for (int i = 0; i < 20; i++) {
		Population[i] = Population2[i];
	}

}

//Evolve function to Cross two parents and mutate 50% of the offspring
void Evolve(vector<vector<int>>& Population, vector<vector<int>> Items, int limit) {
	for (int i = 10; i < 20; i = i + 2) {
		bool a = false;
		int r = 0;
		while (a == false) {
			vector<int> g = Population[i];
			Population[i] = Cross(Population[i - 10], Population[i - 9]);
			if ((rand() % 2) == 1) {
				invert(Population[i]);
			}
			if (weight(Population[i], Items) > limit) {
				if (r != 10) {
					a = false;
					r++;
				}
				else {
					Population[i] = g;
					r = 0;
					a = true;
				}
			}
			else {
				a = true;
			}
		}
		a = false;
		while (a == false) {
			vector<int> g = Population[i];
			Population[i + 1] = Cross(Population[i - 9], Population[i - 10]);
			if ((rand() % 2) == 1) {
				invert(Population[i]);
			}
			if (weight(Population[i], Items) > limit) {
				if (r != 10) {
					a = false;
					r++;
				}
				else {
					Population[i] = g;
					a = true;
				}
			}
			else {
				a = true;
			}
		}
	}
	fitnessOrganizer(Population, Items);
}

//Functions to initiate Evolve funtion and keep track of the fittest individual and what generation its from
int breed(vector<vector<int>>& Population, vector<vector<int>> Items, int limit) {
	int k = 1;
	vector<int> best = Population[0];
	for (int i = 1; i < 21; i++) {
		Evolve(Population, Items, limit);
		if (value(best, Items) < value(Population[0], Items)) {
			best = Population[0];
			k = i;
		}
	}
	return k;
}

int main() {
	for (int k = 0; k < 1; k++) {
		int limit = rand() % 100 + 200;
		vector<vector<int>> Items = ItemGen();
		for (int i = 1; i < 4; i++) {
			vector<vector<int>> Population = PopGen(limit, Items);
			cout << "Best of set 1, run " << i << ": ";
			fitnessOrganizer(Population, Items);
			int gen = breed(Population, Items, limit);
			for (int k = 0; k < 30; k++) {
				cout << Population[0][k];
			}
			cout << " from generation " << gen;
			cout << " with a fitness of " << value(Population[0], Items) << endl << endl;
		}
	}

	for (int k = 0; k < 1; k++) {
		int limit = rand() % 100 + 200;
		vector<vector<int>> Items = ItemGen();
		for (int i = 1; i < 4; i++) {
			vector<vector<int>> Population = PopGen(limit, Items);
			cout << "Best of set 2, run " << i << ": ";
			fitnessOrganizer(Population, Items);
			int gen = breed(Population, Items, limit);
			for (int k = 0; k < 30; k++) {
				cout << Population[0][k];
			}
			cout << " from generation " << gen;
			cout << " with a fitness of " << value(Population[0], Items) << endl << endl;
		}
	}

	for (int k = 0; k < 1; k++) {
		int limit = rand() % 100 + 200;
		vector<vector<int>> Items = ItemGen();
		for (int i = 1; i < 4; i++) {
			vector<vector<int>> Population = PopGen(limit, Items);
			cout << "Best of set 3, run " << i << ": ";
			fitnessOrganizer(Population, Items);
			int gen = breed(Population, Items, limit);
			for (int k = 0; k < 30; k++) {
				cout << Population[0][k];
			}
			cout << " from generation " << gen;
			cout << " with a fitness of " << value(Population[0], Items) << endl << endl;
		}
	}
}