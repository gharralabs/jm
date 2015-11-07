#include "Personagem.h"
#include <c2d2/chien2d2.h>

Personagem::Personagem(const char* caminhoSprite, int largura, int altura)
{
	m_spriteSheet = C2D2_CarregaSpriteSet("ken.png", 63, 79);
}


Personagem::~Personagem()
{
	C2D2_RemoveSpriteSet(m_spriteSheet);
}


void Personagem::Atualizar()
{
	switch (m_estado)
	{
		case PERSONAGEM_PARADO:
			m_quadroAtual = 9;
			break;
	
		case PERSONAGEM_ANDANDO:
			if (m_proximoQuadro < C2D2_TempoSistema())
			{
				m_quadroAtual = (m_quadroAtual + 1) % 11;
				m_proximoQuadro = C2D2_TempoSistema() + 100;
			}
			break;
	}
}

void Personagem::Desenhar(){

}