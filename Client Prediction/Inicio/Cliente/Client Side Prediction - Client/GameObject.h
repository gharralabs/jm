#pragma once
class GameObject
{

private:
	int m_x,
		m_y;
public:
	virtual void Atualizar() = 0;
	int GetX() const;
	int GetY() const;
	void SetX(int x);
	void SetY(int y);

};

