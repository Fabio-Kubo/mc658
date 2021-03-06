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

void carregarUsuarios(int n, vector<int> &p, vector<int> &w, vector<int> &c, vector<usuario> &usuarios, int &maiorClasse){
	int i;
	maiorClasse = -1;

	for(i=0; i<n; i++) {
		usuario novoUsuario;
		novoUsuario.id = i;
		novoUsuario.classe = c[i];
		novoUsuario.pagamento = p[i];
		novoUsuario.larguraBanda = w[i];
		novoUsuario.pagamentoPorLargura = (float)novoUsuario.pagamento/(float)novoUsuario.larguraBanda;
		usuarios.push_back(novoUsuario);

		if(maiorClasse < novoUsuario.classe){
			maiorClasse = novoUsuario.classe;
		}
	}
}

bool comparaPorPrecoLarguraBanda(const usuario &a, const usuario &b){
    return b.pagamentoPorLargura < a.pagamentoPorLargura;
}

void backtracking(vector<usuario> &usuarios, vector<bool> classesUtilizadas, int indice, int larguraBandaDivisor, int larguraBandaTotal, int larguraBandaAcumulada,
	 int pagamentosAcumulados, int &pagamentoOtimo, vector<int> solucaoAtual, vector<int> &solucaoOtima, bool ehPrimeiraClasse){

	int auxLarguraBandaAtual;

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
			 pagamentosAcumulados, pagamentoOtimo, solucaoAtual, solucaoOtima, ehPrimeiraClasse);

		 auxLarguraBandaAtual = usuarios[indice].larguraBanda;
		 if(!ehPrimeiraClasse && !classesUtilizadas[usuarios[indice].classe])
	 			auxLarguraBandaAtual += larguraBandaDivisor;

		//verifica se da pra adicionar o atual com o divisor
		if(larguraBandaAcumulada + auxLarguraBandaAtual <= larguraBandaTotal){
			//chamada adicionando o item atual
			solucaoAtual[usuarios[indice].id] = 1;
			classesUtilizadas[usuarios[indice].classe] = true;

			backtracking(usuarios, classesUtilizadas, indice + 1, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada + auxLarguraBandaAtual,
		 	pagamentosAcumulados + usuarios[indice].pagamento, pagamentoOtimo, solucaoAtual, solucaoOtima, false);
		}
	}

	// exit the code because of timeout
	clock_t end = clock();
	double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;
	if (elapsed_secs > timeout_limit) {
		throw elapsed_secs;
	}
}

void branchAndBound(vector<usuario> &usuarios, vector<bool> classesUtilizadas, int indice, int larguraBandaDivisor, int larguraBandaTotal, int larguraBandaAcumulada,
	 int pagamentosAcumulados, int &pagamentoOtimo, vector<int> solucaoAtual, vector<int> &solucaoOtima, bool ehPrimeiraClasse){
	int pagamentoMaximoBranch, auxLarguraBandaAtual;

	if(larguraBandaAcumulada > larguraBandaTotal)
		return;

		if(pagamentoOtimo < pagamentosAcumulados){
			solucaoOtima.swap(solucaoAtual);
			pagamentoOtimo = pagamentosAcumulados;
		}

	//caso tenha percorrido todas as  pessoas
	if(indice == usuarios.size())
			return;

	//chamada sem adicionar o item atual
	solucaoAtual[usuarios[indice].id] = 0;
	branchAndBound(usuarios, classesUtilizadas, indice + 1, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada,
		 pagamentosAcumulados, pagamentoOtimo, solucaoAtual, solucaoOtima, ehPrimeiraClasse);

	 //se for n for primeira classe e for uma nova classe, adiciona-se o divisor
	 auxLarguraBandaAtual = usuarios[indice].larguraBanda;
		if(!ehPrimeiraClasse && !classesUtilizadas[usuarios[indice].classe])
			auxLarguraBandaAtual += larguraBandaDivisor;

		pagamentoMaximoBranch = (larguraBandaTotal - larguraBandaAcumulada) * usuarios[indice].pagamentoPorLargura;

		if(pagamentoOtimo + 1 <=pagamentosAcumulados + pagamentoMaximoBranch){

			solucaoAtual[usuarios[indice].id] = 1;
			classesUtilizadas[usuarios[indice].classe] = true;
			larguraBandaAcumulada += auxLarguraBandaAtual;
			pagamentosAcumulados += usuarios[indice].pagamento;

			branchAndBound(usuarios, classesUtilizadas, indice + 1, larguraBandaDivisor, larguraBandaTotal, larguraBandaAcumulada,
				 pagamentosAcumulados, pagamentoOtimo, solucaoAtual, solucaoOtima, false);
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
	int maiorClasse;
	vector<usuario> usuarios;

	bool passed = true;
	int pagamentoOtimo = -1;

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//carrega os usuarios e ordena crescentemente de acordo com a largura de banda
	carregarUsuarios(n, p, w, c, usuarios, maiorClasse);

	vector<bool> classesUtilizadas(maiorClasse, false);

	std::sort(usuarios.begin(), usuarios.end(), comparaPorPrecoLarguraBanda);

	try{
		backtracking(usuarios, classesUtilizadas, 0, d, B, 0, 0, pagamentoOtimo, solucaoAtual, solucaoOtima, true);
	 	//inicializa o vector de solucao
		sol.swap(solucaoOtima);

		clock_t end = clock();
		double elapsed_secs = double(end - begin_clock) / CLOCKS_PER_SEC;

		printf("solucao otima: %d\n", pagamentoOtimo);
		printf("tempo de execucao: %f\n", elapsed_secs);

	} catch (double timeout) {
		passed = false;
		printf("timeout:\n");
		printf("solucao ate o momento: %d\n", pagamentoOtimo);
		printf("tempo: %f\n", timeout);
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
	int maiorClasse;
	bool passed = true;
	int pagamentoOtimo = -1;

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//carrega os usuarios e ordena crescentemente de acordo com a preco por largura de banda
	carregarUsuarios(n, p, w, c, usuarios, maiorClasse);

	vector<bool> classesUtilizadas(maiorClasse, false);

	std::sort(usuarios.begin(), usuarios.end(), comparaPorPrecoLarguraBanda);

	try{
		branchAndBound(usuarios, classesUtilizadas, 0, d, B, 0,0, pagamentoOtimo,
			solucaoAtual, solucaoOtima, true);
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
		printf("tempo: %f\n", timeout);
	}
	return true;
}
