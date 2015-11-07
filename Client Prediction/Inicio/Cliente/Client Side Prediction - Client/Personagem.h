#pragma once
#include "DrawableGameObject.h"
#define PERSONAGEM_PARADO 1
#define PERSONAGEM_ANDANDO 2

class Personagem : public DrawableGameObject
{
private:
	int m_quadroAtual = 0;
	unsigned int m_proximoQuadro = 0;

	unsigned int m_spriteSheet;
	
	int m_estado = PERSONAGEM_PARADO;

public:
	Personagem(const char* caminhoSprite, int largura, int altura);
	~Personagem();
	void Atualizar();
	void Desenhar();
	
};

