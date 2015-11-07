#include "GameObject.h"

void GameObject::SetX(int x){
	m_x = x;
}

void GameObject::SetY(int y){
	m_y = y;
}

int GameObject::GetX() const{
	return m_x;
}

int GameObject::GetY() const{
	return m_y;
}
