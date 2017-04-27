/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"
#include <algorithm>

struct cliente{
	int id,
	int classe,
	int larguraBanda,
	int pagamento,
	float pagamentoPorLargura
};

void carregarClientes(vector<int> &p, vector<int> &w, vector<int> &c, vector<cliente> &clientes){
	int i;
	for(i=0; i<clientes.size(); i++) {
		cliente novoCliente;
		novoCliente[i].id = i;
		novoCliente[i].classe = c[i];
		novoCliente[i].pagamento = p[i];
		novoCliente[i].larguraBanda = w[i];
		novoCliente[i].pagamentoPorLargura = (float)novoCliente[i].pagamento/(float)novoCliente[i].larguraBanda;
		clientes.push_back(novoCliente);
	}
}

bool comparaPorLarguraBanda(const cliente &a, const cliente &b){
    return a.larguraBanda < b.larguraBanda;
}

bool comparaPorPrecoLarguraBanda(const cliente &a, const cliente &b){
    return a.pagamentoPorLargura > b.pagamentoPorLargura;
}

int timeout_limit;
clock_t begin_clock;

void backtracking(int qtdPessoas, int larguraBandaDivisor, int larguraBandaTotal,
	vector<int> &p, vector<int> &w, vector<int> &c, int iPessoa, int larguraBandaAcumulada,
	int pagamentos, int pagamentoOtimo, vector<int> solucaoAtual, vector<int> &solucaoOtima){

	int classe, larguraBanda, pagamento, larguraBandaComDivisor;

	//caso tenha percorrido todas as  pessoas
	if(iPessoa == qtdPessoas+1){
		if(pagamentoOtimo < pagamentos){
			solucaoOtima.swap(solucaoAtual);
		}
	}
	else{
		classe = c[iPessoa];
		pagamento = w[iPessoa];
		larguraBanda = p[iPessoa];
		larguraBandaComDivisor = retornaLarguraBandaComDivisor(classe, larguraBanda, larguraBandaDivisor, controleUtilizacaoClasses);

		//caso a largura de banda acumulada somada a atual ultrapasse o limite
		if(larguraBandaAcumulada + larguraBanda > larguraBandaTotal){
			if(pagamentoOtimo < pagamentos){
				solucaoOtima.swap(solucaoAtual);
			}
		}
		else{

			//chamada sem adicionar o item atual
			solucaoAtual[iPessoa] = 0;
			backtracking(iPessoa++, larguraBandaAcumulada, larguraBandaTotal,
				pagamentos, pagamentoOtimo, solucaoAtual, solucaoOtima);

			//verifica se da pra adicionar o atual com o divisor
			if(larguraBandaAcumulada + larguraBandaComDivisor <= larguraBandaTotal){
				//chamada adicionando o item atual
				solucaoAtual[iPessoa] = 1;
				backtracking(iPessoa++, larguraBandaAcumulada + larguraBandaComDivisor, larguraBandaTotal,
					pagamentos + pagamento, pagamentoOtimo, solucaoAtual, solucaoOtima);
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

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//inicializa o vector de solucao
	sol.swap(aux);

	// quicksort ordenando crescentemente na largura de banda
	std::sort(info.begin(), info.end(), comparaPorLarguraBanda);

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

	// start clock
	begin_clock = clock();
	timeout_limit = t;

	//inicializa o vector de solucao
	vector<int> aux(n, 0);
	sol.swap(aux);

	// quicksort ordenando decrescentemente por pagamento/larguraBanda.
	std::sort(info.begin(), info.end(), comparaPorPrecoLarguraBanda);

	try{
			//TODO chamar branchandBound
	} catch (double timeout) {
		passed = false;
		printf("timeout\n");
	}

	return false;
}
