#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
#include <iostream>

using namespace std;

#define INFINITY 32768
#define MEMBER 1
#define NONMEMBER 0
#define MAXNODES 50

typedef struct{
	int orig;
	int dest;
	int trafego;
}Trafego;

typedef struct graph GRAPH;

struct arc {
	int adj;
	int weight;
};

struct graph {
	struct arc arcs[MAXNODES][MAXNODES];
};

vector<vector <int>> topologia;
vector<Trafego> vecTrafego;
vector<vector<int>> matAdjacencia;
vector <vector<int>> vecCaminhos;
int nNos = 0;

void leituraTopologia();
void imprimeTopologia();
void imprimeTrafego();
void imprimeAdjacencia(GRAPH *ADJ);
void joinwt(GRAPH *ADJ, int origem, int destino, int wt);
void dijkstra(GRAPH *ADJ, int s, int t);
void calculaTrafego(GRAPH *ADJ);
void imprimeTopologiaTrafego(GRAPH *ADJ);
void imprimeCaminhos();
GRAPH* init_graph();

int main()
{
	cout << " \t|-------------------------------------------------|\n" ;
	cout << " \t| Simulador para calculo de Capacidade Por Enlace |\n";
	cout << " \t|                Alunos: Rodrigo Levinski         |\n";
	cout << " \t|                                E                |\n";
	cout << " \t|                           Guilherme Bizzani     |\n";
	cout << " \t|-------------------------------------------------|\n";
	
	leituraTopologia();
	imprimeTopologia();
	imprimeTrafego();

	GRAPH *ADJ = init_graph();

	for (int i = 0; i < topologia.size(); i++)
	{
		joinwt(ADJ, topologia[i][0] - 1, topologia[i][1] - 1, 1);

	}
	//imprimeAdjacencia(ADJ);

	for (int i = 0; i < vecTrafego.size(); i++)
	{
		dijkstra(ADJ, vecTrafego[i].orig-1, vecTrafego[i].dest-1);
	}
	cout << endl;

	calculaTrafego(ADJ);

	//imprimeCaminhos();
	imprimeTopologiaTrafego(ADJ);
	//imprimeAdjacencia(ADJ);
	cout << "\n\n";
	system("pause");

	vecCaminhos.clear();
	matAdjacencia.clear();
	vecTrafego.clear();
	topologia.clear();
	free(ADJ);

	return 0;
}

void imprimeCaminhos(){
	for (int i = 0; i < vecCaminhos.size(); i++)
	{
		cout << "Caminho " << i + 1 << " - > ";
		for (int j = 0; j < vecCaminhos[i].size(); j++)
		{
			if (j != vecCaminhos[i].size()-1)
			cout << vecCaminhos[i][j] << " - ";
			else
			cout << vecCaminhos[i][j];
		}
		cout << endl << endl;
	}
}

void imprimeAdjacencia(GRAPH *gTemp){
	cout << endl;
	for (int i = 0; i < nNos; i++) {

		for (int j = 0; j < nNos; j++) {

			if (gTemp->arcs[i][j].weight < 32000) cout << " - " << gTemp->arcs[i][j].adj << " T " << gTemp->arcs[i][j].weight;
			else cout << " - " << gTemp->arcs[i][j].adj << " NC ";
			
		}
		cout << endl << endl;
	}
}

GRAPH* init_graph() {

	GRAPH *gTemp = (GRAPH *)malloc(sizeof(GRAPH));

	for (int i = 0; i < MAXNODES; i++) {

		for (int j = 0; j < MAXNODES; j++) {

			gTemp->arcs[i][j].adj = 0;
			gTemp->arcs[i][j].weight = INFINITY;
		}
	}

	return gTemp;

}

void joinwt(GRAPH *ADJ, int origem, int destino, int wt) {

	ADJ->arcs[origem][destino].adj = 1;
	ADJ->arcs[origem][destino].weight = wt;
	ADJ->arcs[destino][origem].adj = 1;
	ADJ->arcs[destino][origem].weight = wt;
}

void calculaTrafego(GRAPH *g){

	for (int i = 0; i < vecCaminhos.size(); i++){
		for (int j = 0; j < vecCaminhos[i].size()-1; j++){

			g->arcs[vecCaminhos[i][j] - 1][vecCaminhos[i][j + 1] - 1].weight += vecTrafego[i].trafego;
			g->arcs[vecCaminhos[i][j + 1] - 1][vecCaminhos[i][j] - 1].weight += vecTrafego[i].trafego;

		}
	}

	for (int i = 0; i < MAXNODES; i++)
	{
		for (int j = 0; j < MAXNODES; j++)
		{
			g->arcs[i][j].weight -= 1;
		}
	}

}

void dijkstra(GRAPH *g, int ORIG, int DEST)
{
	int dist[MAXNODES], perm[MAXNODES], path[MAXNODES];
	int current, i, k, dc;
	int smalldist, newdist;
	vector<int> caminhoMIN;


	// Inicializa todos os índices de 'perm' como 0 e de 'dist' como INFINITY 
	for (i = 0; i < MAXNODES; i++) {
		perm[i] = NONMEMBER;
		dist[i] = INFINITY;
	}

	// Inclui 's' em perm (perm[s]=1) e configura(armazena) sua distancia como sendo zero 
	perm[ORIG] = MEMBER;
	dist[ORIG] = 0;

	// define 's' como origem (fonte) da busca 
	current = ORIG;
	k = current;

	while (current != DEST) {

		smalldist = INFINITY;
		dc = dist[current];

		for (i = 0; i < MAXNODES; i++) {

			//se o elemento NÃO está em perm
			if (perm[i] == NONMEMBER) {

				//calcula distância a partir do vértice corrente ao vértice adjacente i
				newdist = dc + g->arcs[current][i].weight;

				//se a distância partindo do vértice corrente for menor, atualiza o vetor 
				//de distâncias e de precedência
				if (newdist <= dist[i]) {
					dist[i] = newdist;
					path[i] = current;
				}
				//determina o vértice (entre todos os não pertencentes a perm) com menor distância
				if (dist[i] < smalldist) {
					smalldist = dist[i];
					k = i;
				}

			}

		} 

		if (current == k) {
			printf("\n\nCAMINHO NAO EXISTE\n\n");
			return;
		}

		current = k;
		perm[current] = MEMBER;
	} 

	int caminho = DEST;
	caminhoMIN.push_back(DEST + 1);

	while (caminho != ORIG)
	{
		caminhoMIN.push_back(path[caminho] + 1);
		caminho = path[caminho];

	}
	vector<int> axx;
	for (int i = caminhoMIN.size()-1;  i >= 0; i--){
		axx.push_back(caminhoMIN[i]);
	}

	vecCaminhos.push_back(axx);
} 

void leituraTopologia(){

	string tp, tf;

	cout << "\n\tDigite o nome do arquivo que contenha a topologia (sem txt): ";
	cin >> tp;
	tp += ".txt";
	cout << "\n\n\tDigite o nome do arquivo que contenha os  trafegos (sem txt) : ";
	cin >> tf;
	tf += ".txt";
	cout << "\n\n";
	ifstream infile(tp);

	string line;

	while (getline(infile, line)){
		istringstream iss(line);
		int n;
		vector<int> v;

		while (iss >> n){
			if (n > nNos )nNos = n;
			v.push_back(n);
		}

		topologia.push_back(v);
	}
	infile.close();

	ifstream infile2(tf);

	while (getline(infile2, line)){
		istringstream iss(line);
		int n;
		vector<int> v;

		while (iss >> n){
			v.push_back(n);
		}
		Trafego ax;
		ax.orig = v[0];
		ax.dest = v[1];
		ax.trafego = v[2];
		vecTrafego.push_back(ax);
	}
	infile2.close();
}

void imprimeTopologiaTrafego(GRAPH *ADJ){
	
	
	ofstream arquivo("saida.txt");

	arquivo << "O  -  D  ->  C\n\n";

	cout << "Rede com " << nNos << " nos\n" << endl << "O - D =  C\n\n";
	for (int i = 0; i < topologia.size(); i++){
		
		cout << topologia[i][0] << " - " << topologia[i][1] << " =  "<< ADJ->arcs[topologia[i][0]-1][topologia[i][1]-1].weight;
		arquivo << topologia[i][0]<<"  -  " << topologia[i][1]<< "  ->  "<< ADJ->arcs[topologia[i][0] - 1][topologia[i][1] - 1].weight << endl;
		cout << endl;

	}
	arquivo.close();
}

void imprimeTopologia(){

	cout <<"Rede com "<< nNos <<" nos" << endl << "O - D \n";
	for (int i = 0; i < topologia.size(); i++){

		cout << topologia[i][0] << " = " << topologia[i][1];
		cout << endl;

	}
}

void imprimeTrafego(){
	cout << endl << "O --- D --> C\n\n";
	for (int i = 0; i < vecTrafego.size(); i++){
		
		cout << vecTrafego[i].orig <<" --- "<< vecTrafego[i].dest <<" --> "<< vecTrafego[i].trafego<<endl;

	}
}