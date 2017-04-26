/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"

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
}

void branchAndBound(int qtdPessoas, int larguraBandaDivisor, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol){

}



// Nome1: Fabio Tadashi Kaneiwa Kubo
// RA1: 145979
// Bactracking function:
///Tries to find the most valuable solution. This function stops branching if the actual branch
// finds its maximum possible value.
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){

	vector<int> aux(n, 0);
	//inicializa o vector de solucao
	sol.swap(aux);

	// fazer quicksort ordenando crescentemente na largura de banda
	backtracking(0, 0, B, 0, 0, sol, sol);
	return true;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){

	//inicializa o vector de solucao
	vector<int> aux(n, 0);
	sol.swap(aux);
	// fazer quicksort ordenando decrescentemente na divisao lucro/larguraBanda.

	return false;
}
