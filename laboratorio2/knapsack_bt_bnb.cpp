/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"
#include <algorithm>

struct usuario{
	int id;
	int classe;
	int larguraBanda;
	int pagamento;
	float pagamentoPorLargura;
};

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

int timeout_limit;
clock_t begin_clock;

void backtracking(vector<usuario> &usuarios, vector<int> classesUtilizadas, int indice, int larguraBandaDivisor, int larguraBandaTotal, int larguraBandaAcumulada,
	 int pagamentosAcumulados, int pagamentoOtimo, vector<int> solucaoAtual, vector<int> &solucaoOtima){

	//caso tenha percorrido todas as  pessoas
	if(indice == usuarios.size()){
		if(pagamentoOtimo < pagamentosAcumulados){
			solucaoOtima.swap(solucaoAtual);
		}
	}
	else{

		//se for nova classe, adiciona-se o divisor
		if(ehClasseDiferente(classesUtilizadas, usuarios[indice].classe)){
			usuarios[indice].larguraBanda = usuarios[indice].larguraBanda + larguraBandaDivisor;
		}

		//caso a largura de banda acumulada somada a atual ultrapasse o limite
		if(larguraBandaAcumulada + usuarios[indice].larguraBanda > larguraBandaTotal){
			if(pagamentoOtimo < pagamentosAcumulados){
				solucaoOtima.swap(solucaoAtual);
			}
		}
		else{

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
	}

	// exit the code because of timeout
	clock_t end = clock();
	double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;
	if (elapsed_secs > timeout_limit) {
		throw elapsed_secs;
	}
}

void branchAndBound(int qtdPessoas, int larguraBandaDivisor, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol){


	// exit the code because of timeout
	clock_t end = clock();
	double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;
	if (elapsed_secs > timeout_limit) {
		throw elapsed_secs;
	}
}



// Nome1: Fabio Tadashi Kaneiwa Kubo
// RA1: 145979
// Bactracking function:
///Tries to find the most valuable solution. This function stops branching if the actual branch
// finds its maximum possible value.
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	vector<int> aux(n, 0);
	vector<usuario> usuarios;

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//inicializa o vector de solucao
	sol.swap(aux);

	//carrega os usuarios e ordena crescentemente de acordo com a largura de banda
	carregarUsuarios(n, p, w, c, usuarios);
	std::sort(usuarios.begin(), usuarios.end(), comparaPorLarguraBanda);

	try{
		backtracking(0, 0, B, 0, 0, sol, sol);
	} catch (double timeout) {
		passed = false;
		printf("timeout\n");
	}
	return true;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	vector<int> aux(n, 0);
	vector<usuario> usuarios;

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//inicializa o vector de solucao
	vector<int> aux(n, 0);
	sol.swap(aux);

	//carrega os usuarios e ordena decrescentemente de acordo com o pagamento/larguraBanda
	carregarUsuarios(n, p, w, c, usuarios);
	std::sort(usuarios.begin(), usuarios.end(), comparaPorPrecoLarguraBanda);

	try{
			//TODO chamar branchandBound
	} catch (double timeout) {
		passed = false;
		printf("timeout\n");
	}

	return false;
}
