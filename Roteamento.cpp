// 
//

#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>
#include <time.h>
#include <algorithm>
#include <iostream>

using namespace std;

typedef struct{
	int orig;
	int dest;
	int trafego;
}Trafego;

vector<vector <int>> topologia;

vector<Trafego> vecTrafego;

void leituraTopologia();
void imprimeTopologia();
void imprimeTrafego();

int main()
{
	leituraTopologia();
	imprimeTopologia();
	imprimeTrafego();
	

	
	int para;
	cin >> para;

	return 0;
}


void leituraTopologia(){

	ifstream infile("teste.txt");

	string line;
	int trafego = 0;
	//Leitura do arquivo
	while (getline(infile, line)){
		istringstream iss(line);
		int n;
		vector<int> v;

		while (iss >> n){
			v.push_back(n);
		}
		if (v[0] > 100){
			trafego = 1;
		}
		if (trafego == 0){
			topologia.push_back(v);
		}
		else if (trafego == 1 && v[0] < 100){
			Trafego ax;
			ax.orig = v[0];
			ax.dest = v[1];
			ax.trafego = v[2];
			vecTrafego.push_back(ax);
		}
		
	}
	infile.close();
}


void imprimeTopologia(){
	
	for (int i = 0; i < topologia.size(); i++){

		cout << topologia[i][0] << " = " << topologia[i][1];
		cout << endl;

	}
}

void imprimeTrafego(){
	cout << endl << "O --- D --> Trafego\n\n";
	for (int i = 0; i < vecTrafego.size(); i++){
		
		cout << vecTrafego[i].orig <<" --- "<< vecTrafego[1].dest <<" --> "<< vecTrafego[i].trafego<<endl;

	}
}