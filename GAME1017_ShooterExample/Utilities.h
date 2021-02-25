#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <class T>
void CleanVector(vector<T>& v, bool& b)
{
	cout << "Cleaning vector!" << endl;
	if (!v.empty())
	{
		v.erase(remove(v.begin(), v.end(), nullptr), v.end());
		v.shrink_to_fit();
	}
	b = false;
}

// Keyboard utility function.
inline bool KeyDown(SDL_Scancode c, const Uint8* m_iKeystates)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}