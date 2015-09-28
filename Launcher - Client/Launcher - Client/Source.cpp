#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <process.h>
#include <sstream> 
#include "VersaoCliente.h"

#define PORTA_SERVIDOR 9000
#define TRANSFER_BUFFER_SIZE 1000000

enum Comandos
{
	NUMERO_ULTIMA_VERSAO_CLIENTE_REQ,
	EXECUTAVEL_ULTIMA_VERSAO_CLIENTE_REQ
};

void receberTcp(const SOCKET& socket,
	void* buffer,
	const long& tamanhoBuffer)
{
	long bytesRecebidos = 0;
	long totalBytesRecebidos = 0;

	do
	{

		// For connection-oriented sockets (type SOCK_STREAM for example), 
		// calling recv will return as much data as is currently available—up 
		// to the size of the buffer specified.

		bytesRecebidos = recv(socket,
			(char*)buffer + totalBytesRecebidos,
			tamanhoBuffer - totalBytesRecebidos,
			NULL);

		if (bytesRecebidos == SOCKET_ERROR ||
			bytesRecebidos == 0)
		{

			std::stringstream erro;

			erro << __FUNCTION__
				<< " - A conexao foi encerrada durante a transmissao de forma inesperada. Codigo: "
				<< WSAGetLastError()
				<< std::endl;

			throw std::runtime_error(erro.str());
		}

		totalBytesRecebidos += bytesRecebidos;

	} while (totalBytesRecebidos != tamanhoBuffer);
}


void receberArquivo(const SOCKET& socket,
	const int& versao,
	const long& tamanhoArquivo)
{

	FILE* arquivo;
	char caminho[255];
	sprintf_s(caminho, "wowv%d.exe", versao);

	int e;
	e = fopen_s(&arquivo, caminho, "wb"); // implementar resumo do download? :-D

	if (e != 0)
	{
		std::stringstream erro;

		erro << __FUNCTION__
			<< " - Ocorreu um erro ao abrir o arquivo para gravacao no cliente: "
			<< e
			<< std::endl;

		throw std::runtime_error(erro.str());
	}
	else
	{
		long totalBytesRecebidos = 0L;
		long bytesRecebidos = 0L;
		char buffer[TRANSFER_BUFFER_SIZE];
		long ler;
		do
		{
			bytesRecebidos = recv(socket,
				buffer,
				TRANSFER_BUFFER_SIZE,
				NULL);

			if (bytesRecebidos == SOCKET_ERROR ||
				bytesRecebidos == 0)
			{

				std::stringstream erro;

				erro << __FUNCTION__
					<< " - A conexao foi encerrada durante a transmissao de forma inesperada. Codigo: "
					<< WSAGetLastError()
					<< std::endl;

				fclose(arquivo);

				throw std::runtime_error(erro.str());
			}

			fwrite(buffer,
				bytesRecebidos,
				sizeof(char),
				arquivo);

			totalBytesRecebidos += bytesRecebidos;

			std::cout << float(totalBytesRecebidos) / float(tamanhoArquivo) * 100.0f << "%"
				<< std::endl;

		} while (totalBytesRecebidos != tamanhoArquivo);

		fclose(arquivo);
	}

}



int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in enderecoServidor;

	inet_pton(AF_INET, "127.0.0.1", &enderecoServidor.sin_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

	SOCKET meuSocket;
	meuSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::cout << "LAUNCHER: " << std::endl;

	int r;
	r = connect(meuSocket,
		(SOCKADDR*)&enderecoServidor,
		sizeof(sockaddr_in));

	if (r != 0)
	{
		std::cout << "Nao foi possivel estabelecer uma conexao com o servidor" << std::endl;
		system("pause");
		return 1;
	}

	try
	{

		VersaoCliente versaoCliente;
		int minhaVersaoCliente = versaoCliente.Versao();

		std::cout << "Conectado ao servidor"
			<< std::endl
			<< "Versao do cliente: "
			<< minhaVersaoCliente << std::endl;


		unsigned int ultimaVersaoCliente;
		int comando = Comandos::NUMERO_ULTIMA_VERSAO_CLIENTE_REQ;

		send(meuSocket,
			(char*)&comando,
			sizeof(int),
			NULL);

		std::cout << "Procurando por atualizacoes..." << std::endl;

		receberTcp(meuSocket,
			&ultimaVersaoCliente,
			sizeof(int));

		if (ultimaVersaoCliente != minhaVersaoCliente)
		{

			std::cout << "Atualizacao encontrada: Versao "
				<< ultimaVersaoCliente
				<< std::endl;


			comando = Comandos::EXECUTAVEL_ULTIMA_VERSAO_CLIENTE_REQ;

			send(meuSocket,
				(char*)&comando,
				sizeof(int),
				NULL);

			long tamanhoArquivo;
			receberTcp(meuSocket,
				&tamanhoArquivo,
				sizeof(long));

			std::cout << "Iniciando a transferencia: "
				<< tamanhoArquivo << " bytes"
				<< std::endl;

			receberArquivo(meuSocket,
				ultimaVersaoCliente,
				tamanhoArquivo);

			std::ostringstream executavel;
			executavel << "wowv" << ultimaVersaoCliente << ".exe";

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			std::cout << "Atualizacao recebida com sucesso." << std::endl;

			versaoCliente.Atualizar(ultimaVersaoCliente);

			std::cout << "Inicializando o jogo." << std::endl;

			CreateProcess(NULL,
				(TCHAR*)executavel.str().c_str(),
				NULL,
				NULL,
				FALSE,
				CREATE_NEW_CONSOLE,
				NULL,
				NULL,
				&si,
				&pi);

		}
		else
		{
			std::cout << "Cliente esta atualizado." << std::endl;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Nao foi possivel realizar o download da ultima versao do cliente: "
			<< std::endl
			<< ex.what();
	}

	closesocket(meuSocket);

	system("pause");
	return 0;
}