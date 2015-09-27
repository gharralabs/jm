#include "ClienteJogo.h"
#include <exception>
#include <sstream>

ClienteJogo::ClienteJogo()
{
	CarregarVersaoDoDisco();
	CarregarDadosDoDisco();
}

void ClienteJogo::CarregarVersaoDoDisco(){

	FILE* arquivo;
	int r;
	r = fopen_s(&arquivo, "versao.txt", "r");
	if (r != 0)
	{
		std::stringstream erro;
		erro << __FUNCTION__
			 << " - Não foi possivel abrir o arquivo de versão do cliente: " 
			 << r 
			 << std::endl;
		
		throw std::runtime_error(erro.str());
	}
	
	fread(&m_versao, sizeof(int), 1, arquivo);
	fclose(arquivo);
	
}

void ClienteJogo::CarregarDadosDoDisco()
{
	// TODO: Carregar mútiplos arquivos

	std::stringstream caminho;
	caminho << "wowv"
		    << m_versao
			<< ".exe";

	int e;
	FILE* arquivo;
	e = fopen_s(&arquivo, 
				caminho.str().c_str(), 
				"rb");

	if (e != 0)
	{
		std::stringstream erro;
		erro << __FUNCTION__
			 << "- Ocorreu um erro ao abrir o arquivo - Codigo: "
			 << e
			 << std::endl;

		throw std::runtime_error(erro.str());
	}
	else
	{
		fseek(arquivo, 0, SEEK_END);
		m_tamanho = ftell(arquivo);
		rewind(arquivo);

		m_dados = new char[m_tamanho];

		fread(m_dados,
			  sizeof(char),
			  m_tamanho,
			  arquivo);

		fclose(arquivo);
	}
}

char* ClienteJogo::Dados() const
{
	return m_dados;
}

long ClienteJogo::Tamanho() const
{
	return m_tamanho;
}

int ClienteJogo::Versao() const
{
	return m_versao;
}

ClienteJogo::~ClienteJogo()
{
	delete m_dados;
}