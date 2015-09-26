#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define VERSAO_CLIENTE_ATUAL 2

#define PORTA_SERVIDOR 9000
int main()
{
	WSAData wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockaddr_in enderecoServidor;
	
	inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(PORTA_SERVIDOR);

	SOCKET principalSocket;
	principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bind(principalSocket, 
		(SOCKADDR*) &enderecoServidor, 
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

		
		int bytesRecebidos = 0; 
		int totalBytesRecebidos = 0;
		
		int versaoCliente;

		do
		{
			bytesRecebidos = recv(clienteSocket,
								  (char*)&versaoCliente + totalBytesRecebidos,
								  sizeof(int) - totalBytesRecebidos,
								  NULL);
			
			if (bytesRecebidos == SOCKET_ERROR ||
				bytesRecebidos == 0)
			{
				std::cout << "Ocorreu um erro ao receber o tamanho do arquivo" << std::endl;
				exit(1);
			}
				
			totalBytesRecebidos += bytesRecebidos;

		} while (totalBytesRecebidos != sizeof(int));
		
		
		if (VERSAO_CLIENTE_ATUAL != versaoCliente)
		{
			FILE* arq;
			int e;
			char path[255];
			sprintf_s(path, "wowv%d.exe", VERSAO_CLIENTE_ATUAL);
			e = fopen_s(&arq, path, "r");
			
			if (e == 0)
			{
				std::cout << "Ocorreu um erro ao abrir o arquivo: " << path << " para leitura.";
				exit(1);
			}

			fseek(arq, 0L, SEEK_END);
			long tamanhoExecCliente = ftell(arq);


			send(clienteSocket,
				(char*)&tamanhoExecCliente, sizeof(long),
				 NULL);

			
			
			fclose(arq);
		}
	}

	WSACleanup();

	return 0;
}