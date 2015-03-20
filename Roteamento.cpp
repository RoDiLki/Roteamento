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

#define INFINITY 32768
#define MEMBER 1
#define NONMEMBER 0
#define MAXNODES 4

typedef struct{
	int orig;
	int dest;
	int trafego;
}Trafego;

typedef struct graph GRAPH;

struct arc {

	// se adj=1 então ADJACENTE; se adj=0 então NÃO ADJACENTE
	int adj;

	// peso da aresta
	int weight;
};

struct graph {

	// matriz de adjacências
	struct arc arcs[MAXNODES][MAXNODES];
};

vector<vector <int>> topologia;
vector<Trafego> vecTrafego;
vector<vector<int>> matAdjacencia;
vector < vector<int>> vecCaminhos;

int leituraTopologia();
void imprimeTopologia(int nNos);
void imprimeTrafego();
void inicializaAdjacencia(int nNos);

void imprimeAdjacencia(GRAPH *gTemp);
GRAPH* init_graph();
void joinwt(GRAPH *g, int origem, int destino, int wt);
void dijkstra(GRAPH *g, int s, int t);
void calculaTrafego(GRAPH *g);
void imprimeTopologiaTrafego(int nNos, GRAPH *g);

//void dijkstra(int nNos, int origem, int destino, GRAPH *g);

int main()
{
	int nNos = leituraTopologia();
	imprimeTopologia(nNos);
	imprimeTrafego();

	GRAPH *ADJ = init_graph();

	for (int i = 0; i < topologia.size(); i++)
	{
		joinwt(ADJ, topologia[i][0] - 1, topologia[i][1] - 1, 1);

	}


	imprimeAdjacencia(ADJ);

	
	for (int i = 0; i < vecTrafego.size(); i++)
	{
		dijkstra(ADJ, vecTrafego[i].orig-1, vecTrafego[i].dest-1);
		//dijkstra(nNos, vecTrafego[i].orig, vecTrafego[i].dest, ADJ);
	}
	calculaTrafego(ADJ);
	imprimeTopologiaTrafego(nNos, ADJ);

	imprimeAdjacencia(ADJ);

	cout << "\n\n";
	system("pause");
	return 0;
}
void imprimeAdjacencia(GRAPH *gTemp){
	cout << endl;
	for (int i = 0; i < MAXNODES; i++) {

		for (int j = 0; j < MAXNODES; j++) {

			//cout << " - " << gTemp->arcs[i][j].adj
				cout << " peso " << gTemp->arcs[i][j].weight;
			
		}
		cout << endl << endl;
	}
}


// inicializa matriz de adjacência que representa o grafo
// retorna ponteiro para esta matriz (tipo GRAPH)
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

// cria uma aresta que "liga" (incide) em dois nós e atribui o respectivo peso;
// recebe o grafo, dois nós (origem e destino) e o peso (wt) da aresta
void joinwt(GRAPH *ADJ, int origem, int destino, int wt) {

	ADJ->arcs[origem][destino].adj = 1;
	ADJ->arcs[origem][destino].weight = wt;
	ADJ->arcs[destino][origem].adj = 1;
	ADJ->arcs[destino][origem].weight = wt;
}

void calculaTrafego(GRAPH *g){

	for (int i = 0; i < vecCaminhos.size(); i++){
		for (int j = 0; j < vecCaminhos[i].size() - 1; j++){
			g->arcs[vecCaminhos[i][j]][vecCaminhos[i][j + 1]].weight += vecTrafego[i].trafego;
			g->arcs[vecCaminhos[i][j + 1]][vecCaminhos[i][j]].weight += vecTrafego[i].trafego;
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

		//embora estamos assumindo grafos ponderados e conexos, este if garante que
		//em caso de não existência de um caminho o programa não entre em loop infinito 
		if (current == k) {
			printf("\n\nCAMINHO NAO EXISTE\n\n");
			return;
		}

		current = k;
		perm[current] = MEMBER;
	} 



	printf("\n\nRESULTADO: ");
	int caminho = DEST;

	printf("%d <- ", DEST+1);
	caminhoMIN.push_back(DEST + 1);

	while (caminho != ORIG)
	{
		printf("%d", path[caminho]+1);
		caminhoMIN.push_back(path[caminho] + 1);
		caminho = path[caminho];

		if (caminho != ORIG)
			printf(" <- ");
	}
	vector<int> axx;

	for (int i = caminhoMIN.size()-1; i >= 0; i--)
	{
		axx.push_back(caminhoMIN[caminhoMIN.size()-1]);
		caminhoMIN.pop_back();
	}

	vecCaminhos.push_back(axx);
	printf("\n\ncusto: %d\n\n", dist[DEST]);
	/****************************************/

} 

int leituraTopologia(){

	ifstream infile("teste.txt");

	string line;
	int trafego = 0;
	int ja = 0;
	int nNos;
	while (getline(infile, line)){
		istringstream iss(line);
		int n;
		vector<int> v;

		while (iss >> n){
			if (ja == 0){
				nNos = n;
				ja = 1;
			}
			v.push_back(n);
		}
		if (v[0] == -1){
			trafego = 1;
		}
		if (trafego == 0 && ja!= 1){
			topologia.push_back(v);
		}
		else if (trafego == 1 && v[0] > -1){
			Trafego ax;
			ax.orig = v[0];
			ax.dest = v[1];
			ax.trafego = v[2];
			vecTrafego.push_back(ax);
		}
		else{
			ja = 2;
		}
		
	}
	infile.close();
	return nNos;
}

void imprimeTopologiaTrafego(int nNos, GRAPH *g){

	cout << "Rede com " << nNos << " nos" << endl << "O - D = Trafego\n";
	for (int i = 0; i < topologia.size(); i++){

		cout << topologia[i][0] << " - " << topologia[i][1] << " = "<< g->arcs[topologia[i][0]-1][topologia[i][1]-1].weight;
		cout << endl;

	}
}

void imprimeTopologia(int nNos){

	cout <<"Rede com "<<nNos<<" nos" << endl << "O - D \n";
	for (int i = 0; i < topologia.size(); i++){

		cout << topologia[i][0] << " = " << topologia[i][1];
		cout << endl;

	}
}

void imprimeTrafego(){
	cout << endl << "O --- D --> Trafego\n\n";
	for (int i = 0; i < vecTrafego.size(); i++){
		
		cout << vecTrafego[i].orig <<" --- "<< vecTrafego[i].dest <<" --> "<< vecTrafego[i].trafego<<endl;

	}
}