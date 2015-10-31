#pragma once

class Grid
{
private:
	char** m_tijolos;
	int m_largura;
	int m_altura;
	int m_quantidadeTijolosX;
	int m_quantidadeTijolosY;
	int m_quantidadeItens;
	int m_posicaoHorizontalItem;
	int m_posicaoVerticalItem;
	

	

public:

	Grid(int largura, 
		 int altura, 
		 int quantidadeTijolosX,
		 int quantidadeTijolosY,
		 int quantidadeItens)
	{
			m_largura = largura;
			m_altura = altura;
			m_quantidadeTijolosX = quantidadeTijolosX;
			m_quantidadeTijolosY = quantidadeTijolosY;
			m_quantidadeItens = quantidadeItens;
			

		m_tijolos = new char*[largura];
		
		for (int i = 0; i < largura; i++)
		{
			m_tijolos[i] = new char[altura];			
			memset(m_tijolos[i], 0, altura);
		}
			
	}

	void SortearItem()
	{
		ObterItemAtual() = 0;

		int x = rand() % m_quantidadeTijolosX;
		int y = rand() % m_quantidadeTijolosY;
		int item = rand() % 3 + 1;
		m_tijolos[x][y] = item;
	}

	char& ObterItemAtual()
	{
		return m_tijolos[m_posicaoHorizontalItem][m_posicaoVerticalItem];
	}

	

	~Grid()
	{
		// desalocar m_tijolos
	}
};