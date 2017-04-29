/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/

 // Nome1: Fabio Tadashi Kaneiwa Kubo
 // RA1: 145979

#include "knapsack.h"
#include <algorithm>
#include <math.h>

struct usuario{
	int id;
	int classe;
	int larguraBanda;
	int pagamento;
	float pagamentoPorLargura;
};

int timeout_limit;
clock_t begin_clock;

void carregarUsuarios(int n, vector<int> &p, vector<int> &w, vector<int> &c, vector<usuario> &usuarios){
	int i;
	for(i=0; i<n; i++) {
		usuario novoUsuario;
		novoUsuario.id = i;
		novoUsuario.classe = c[i];
		novoUsuario.pagamento = p[i];
		novoUsuario.larguraBanda = w[i];
		novoUsuario.pagamentoPorLargura = (float)novoUsuario.pagamento/(float)novoUsuario.larguraBanda;
		usuarios.push_back(novoUsuario);
	}
}

bool comparaPorLarguraBanda(const usuario &a, const usuario &b){
    return a.larguraBanda < b.larguraBanda;
}

bool comparaPorPrecoLarguraBanda(const usuario &a, const usuario &b){
    return a.pagamentoPorLargura > b.pagamentoPorLargura;
}

bool ehClasseDiferente(vector<int> &classesUtilizadas, int classe){
	int i;

	if(classesUtilizadas.size() == 0)
		return false;

	for(i=0; i<classesUtilizadas.size(); i++){
		if(classesUtilizadas[i] == classe){
			return false;
		}
	}
	return true;
}

void backtracking(vector<usuario> &usuarios, vector<int> classesUtilizadas, int indice, int larguraBandaDivisor, int larguraBandaTotal, int larguraBandaAcumulada,
	 int pagamentosAcumulados, int &pagamentoOtimo, vector<int> solucaoAtual, vector<int> &solucaoOtima){

	//caso tenha percorrido todas as  pessoas
	if(indice == usuarios.size()){
		if(pagamentoOtimo < pagamentosAcumulados){
			solucaoOtima.swap(solucaoAtual);
			pagamentoOtimo = pagamentosAcumulados;
		}
	}
	else{

		//se for nova classe, adiciona-se o divisor
		if(ehClasseDiferente(classesUtilizadas, usuarios[indice].classe))
			usuarios[indice].larguraBanda = usuarios[indice].larguraBanda + larguraBandaDivisor;

		//chamada sem adicionar o item atual
		solucaoAtual[usuarios[indice].id] = 0;
		backtracking(usuarios, classesUtilizadas, indice + 1, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada,
			 pagamentosAcumulados, pagamentoOtimo, solucaoAtual, solucaoOtima);

		//verifica se da pra adicionar o atual com o divisor
		if(larguraBandaAcumulada + usuarios[indice].larguraBanda <= larguraBandaTotal){
			//chamada adicionando o item atual
			solucaoAtual[usuarios[indice].id] = 1;

			backtracking(usuarios, classesUtilizadas, indice + 1, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada + usuarios[indice].larguraBanda,
		 	pagamentosAcumulados + usuarios[indice].pagamento, pagamentoOtimo, solucaoAtual, solucaoOtima);
		}
	}

	// exit the code because of timeout
	clock_t end = clock();
	double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;
	if (elapsed_secs > timeout_limit) {
		throw elapsed_secs;
	}
}

void branchAndBound(vector<usuario> &usuarios, vector<int> classesUtilizadas, int indice, int larguraBandaDivisor, int larguraBandaTotal, int larguraBandaAcumulada,
	 int pagamentosAcumulados, int &pagamentoOtimo, vector<int> solucaoAtual, vector<int> &solucaoOtima){
	int pagamentoMaximoBranch;

	//caso tenha percorrido todas as  pessoas
	if(indice == usuarios.size()){
		if(pagamentoOtimo < pagamentosAcumulados){
			solucaoOtima.swap(solucaoAtual);
			pagamentoOtimo = pagamentosAcumulados;
		}
	}
	else{

		//chamada sem adicionar o item atual
		solucaoAtual[usuarios[indice].id] = 0;
		backtracking(usuarios, classesUtilizadas, indice + 1, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada,
			 pagamentosAcumulados, pagamentoOtimo, solucaoAtual, solucaoOtima);

		 //se for nova classe, adiciona-se o divisor
 		if(ehClasseDiferente(classesUtilizadas, usuarios[indice].classe))
 			usuarios[indice].larguraBanda = usuarios[indice].larguraBanda + larguraBandaDivisor;

		//verifica se da pra adicionar o atual
		if(larguraBandaAcumulada + usuarios[indice].larguraBanda <= larguraBandaTotal){
			//chamada adicionando o item atual
			solucaoAtual[usuarios[indice].id] = 1;

			larguraBandaAcumulada += usuarios[indice].larguraBanda;
			pagamentosAcumulados += usuarios[indice].pagamento;

			indice++;
			//se n for o ultimo
			if(indice < usuarios.size()){

					pagamentoMaximoBranch = (larguraBandaTotal - larguraBandaAcumulada) * ceil(usuarios[indice].pagamentoPorLargura);

					if(pagamentoOtimo <= pagamentosAcumulados + pagamentoMaximoBranch){
						backtracking(usuarios, classesUtilizadas, indice, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada,
							 pagamentosAcumulados, pagamentoOtimo, solucaoAtual, solucaoOtima);
					}
			}
			else if(pagamentoOtimo < pagamentosAcumulados){
					solucaoOtima.swap(solucaoAtual);
					pagamentoOtimo = pagamentosAcumulados;
			}
		}
	}

	// exit the code because of timeout
	clock_t end = clock();
	double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;
	if (elapsed_secs > timeout_limit) {
		throw elapsed_secs;
	}
}

// Bactracking function:
///Tries to find the most valuable solution. This function stops branching if the actual branch
// finds its maximum possible value.
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	vector<int> solucaoAtual(n, 0);
	vector<int> solucaoOtima(n, 0);
	vector<usuario> usuarios;
	vector<int> classesUtilizadas;
	bool passed = true;
	int pagamentoOtimo = -1;

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//carrega os usuarios e ordena crescentemente de acordo com a largura de banda
	carregarUsuarios(n, p, w, c, usuarios);
	std::sort(usuarios.begin(), usuarios.end(), comparaPorLarguraBanda);

	try{
		backtracking(usuarios, classesUtilizadas, 0, d, B, 0, 0, pagamentoOtimo, solucaoAtual, solucaoOtima);
	 	//inicializa o vector de solucao
		sol.swap(solucaoOtima);

		clock_t end = clock();
		double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;

		printf("solucao otima: %d\n", pagamentoOtimo);
		printf("tempo de execucao: %f\n", elapsed_secs);

	} catch (double timeout) {
		passed = false;
		printf("solucao ate o momento: %d\n", pagamentoOtimo);
		printf("timeout\n");
	}
	return true;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	vector<int> solucaoAtual(n, 0);
	vector<int> solucaoOtima(n, 0);
	vector<usuario> usuarios;
	vector<int> classesUtilizadas;
	bool passed = true;
	int pagamentoOtimo = -1;

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//carrega os usuarios e ordena crescentemente de acordo com a largura de banda
	carregarUsuarios(n, p, w, c, usuarios);
	std::sort(usuarios.begin(), usuarios.end(), comparaPorLarguraBanda);

	try{
		branchAndBound(usuarios, classesUtilizadas, 0, d, B, 0,0, pagamentoOtimo,
			solucaoAtual, solucaoOtima);
		//inicializa o vector de solucao
		sol.swap(solucaoOtima);

		clock_t end = clock();
		double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;

		printf("solucao otima: %d\n", pagamentoOtimo);
		printf("tempo de execucao: %f\n", elapsed_secs);

	} catch (double timeout) {
		passed = false;
		printf("timeout\n");
		printf("solucao ate o momento: %d\n", pagamentoOtimo);
	}
	return true;
}
