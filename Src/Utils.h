#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <stdint.h>

#define CELL_WIDTH 32 
#define CELL_HEIGHT 32


class Object;

// ---------- Map Cell ---------- //
class Cell{
public:

	int Cost; // walk coast of one call 0-5 (9-non walkable), changes when some objects set on it
	Object* ground;
	Object* eniv;
	Object* npc;
};

// ---------- Direction ---------- //
enum Direction{
	Left,
	Right,
	Up,
	Down
};

// ---------- Poistion on map in cells (x,y) ---------- //
struct Point{
	Point(int mx, int my){
		X = mx;
		Y = my;
	}
	int X;
	int Y;
};

// ---------- Types of objects on map ---------- //
enum ObjectType{
	hero,    // main hero
	npc,     // npc
	wall,    // wall
	flor,    // tile of floor
	monster, // monster
	box,      // box
	empty     // Empty type for array initialization
};


// For map loading
struct MCell{
public:
	ObjectType ground;
	ObjectType eniv;
	ObjectType npc;
};

//---------New Map---------//
struct MMap{
	int height, width; // map height and width in cells
	std::vector<std::vector<MCell> >cells; // MCell array
};

// ---------- Layer ---------- //
enum Layer{
	GROUND,
	ENIVORMENT,
	NPC
};

// ---------- Multi Array ---------- //
template <class T>
class MultiArray
{

private:
	std::vector<T> m_data;
	size_t m_x_width, m_y_height;

public:

	MultiArray() 
		:m_x_width(0)
		,m_y_height(0)
	{    
	}

	MultiArray(size_t new_x_width,size_t new_y_height) 
		:m_x_width(0)
		,m_y_height(0) //��� ���������� ����������
	{
		resize(new_x_width, new_y_height);
	}

	void resize(size_t new_x_width,size_t new_y_height)
	{  //������ �� integer overflow ��� ���������. 
        if(int64_t(new_x_width) * int64_t(new_y_height) > int64_t(0x80000000))
			//��� �� ������� std::out_of_range, ��� �������� ��������, �� �� ������� �� std::string.
			throw std::bad_alloc(); 

		m_data.resize(new_x_width * new_y_height); //may throw.
		m_x_width = new_x_width; //���� ��������� ����, �� �� ����� strong-exception-safe. � �����, ��� ����� ���������.
		m_y_height = new_y_height;

		//��� ����� ����� ���� �������� ��� ��� ����. copy-and-swap idiom, ���������� 
		//����������� ������, ��� �� � ����� array(3, 7) ������� �� ��� �� ����� ��� 
		//� ��� array(3, 7), ������� if(����� ������ != ������ ������) � ��.
	}

	size_t x_width() { return m_x_width; }
	size_t y_height() { return m_y_height; }

	T &operator()(size_t x, size_t y)
	{
		assert(x < m_x_width && y < m_y_height); //���� ������, �������� out_of_range. ������, ����� ������ ����.
		return m_data[m_x_width * y + x];
	}

	const T &operator()(size_t x, size_t y) const
	{
		assert(x < m_x_width && y < m_y_height);
		return m_data[m_x_width * y + x];
	}

	T *get_data_ptr()
	{
		return m_data.size() ? &m_data[0] : NULL;
	}

	const T *get_data_ptr() const
	{
		return m_data.size() ? &m_data[0] : NULL;
	}
};
#endif // UTILS_H