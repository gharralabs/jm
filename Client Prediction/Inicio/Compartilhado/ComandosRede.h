#pragma once

#define MAX_PACKET_SIZE 512
#define CMD_ENTRAR 1
#define CMD_INICIAR 2
#define CMD_MOVER_REQ 3
#define CMD_MOVER_RES 4


struct Comando
{
	int comando;
};


struct MoverComandoRequisicao : Comando
{
	int x, y;
};


struct MoverComandoResposta : Comando
{
	bool permitido;
};