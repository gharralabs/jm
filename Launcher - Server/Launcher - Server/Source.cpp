#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <thread>
#include <future>
#include <vector>
#include "ClienteJogo.h"


enum Comandos
{
	NUMERO_ULTIMA_VERSAO_CLIENTE_REQ,
	EXECUTAVEL_ULTIMA_VERSAO_CLIENTE_REQ
};


#define PORTA_SERVIDOR 9000

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


void tratarCliente(SOCKET clienteSocket,
	ClienteJogo* ultimoCliente)
{
	for (;;)
	{
		try
		{
			int comando;
			int ultimaVersaoCliente;

			receberTcp(clienteSocket, &comando, sizeof(int));

			// TODO: Aplicar Command Pattern
			switch (comando)
			{
			case Comandos::NUMERO_ULTIMA_VERSAO_CLIENTE_REQ:

				ultimaVersaoCliente = ultimoCliente->Versao();

				send(clienteSocket,
					(char*)&ultimaVersaoCliente,
					sizeof(int),
					NULL);
				break;


			case Comandos::EXECUTAVEL_ULTIMA_VERSAO_CLIENTE_REQ:

				// TODO: IMPLEMENTAR LISTA DE ARQUIVOS
				const char* executavel = ultimoCliente->Dados();
				long tamanhoExecutavel = ultimoCliente->Tamanho();

				send(clienteSocket,
					(char*)&tamanhoExecutavel,
					sizeof(long),
					NULL);

				send(clienteSocket,
					executavel,
					tamanhoExecutavel,
					NULL);
				break;
			}

		}
		catch (std::exception& ex)
		{
			std::cout << ex.what();
			break;
		}
	}
}

int main()
{
	try
	{
		ClienteJogo* ultimoCliente = new ClienteJogo();

		WSAData wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		sockaddr_in enderecoServidor;

		inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);
		enderecoServidor.sin_family = AF_INET;
		enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

		SOCKET principalSocket;
		principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		bind(principalSocket,
			(SOCKADDR*)&enderecoServidor,
			sizeof(sockaddr_in));


		listen(principalSocket, SOMAXCONN);

		SOCKET clienteSocket;
		sockaddr_in clienteEndereco;
		int clienteEnderecoTam = sizeof(sockaddr_in);

		
		for (;;)
		{
			clienteSocket = accept(principalSocket,
				(SOCKADDR*)&clienteEndereco,
				&clienteEnderecoTam);

			std::async(std::launch::async,
				tratarCliente,
				clienteSocket,
				ultimoCliente);
		}


		WSACleanup();

		delete ultimoCliente;

	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
		system("pause");
		return 1;
	}



	return 0;
}