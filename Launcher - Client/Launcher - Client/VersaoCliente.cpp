#include "VersaoCliente.h"
#include <iostream>
#include <sstream>

int VersaoCliente::Versao() const{

	FILE* arquivo;
	int r;
	r = fopen_s(&arquivo, "versao.txt", "r");
	if (r != 0)
		return -1;

	int versao;
	fread(&versao, sizeof(int), 1, arquivo);
	fclose(arquivo);

	return versao;
}

void VersaoCliente::Atualizar(int versao)
{
	FILE* arquivo;
	int r;
	r = fopen_s(&arquivo, "versao.txt", "w");
	
	if (r != 0)
	{
		std::stringstream erro;
		erro << __FUNCTION__
			<< " - Nao foi possivel salvar o arquivo de configuracao de versoes: "
			<< r
			<< std::endl;

		throw std::runtime_error(erro.str());
	}

	fwrite(&versao, sizeof(int), 1, arquivo);
	fclose(arquivo);
}