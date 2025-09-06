// ConsoleApplication3.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <locale>
#include <random>
#include <iostream>

using namespace std;

class Creature{ //����������� ������������ ����� ���� �������
protected:
	
	// ������������ ������ ��� ���� �������
	static const int maxHealth = 100; // �������� ���������� ��������
	static const int minAtDef = 1; // ����������� ����� � ������
	static const int maxAtDef = 30; // ������������ ����� � ������
	static const int minDam = 1; // ����������� ����
	static const int maxDam = 30; // ������������ ����
	static const int kolHeals = 4; // ���������� ��� �������������� �������� � ������
	static const int maxHealsPercent = 30; // ������� �������������� �������� � ������
public:
	// ���-�� ��������, ������� ��������, �����, ������, ��� � ���� ����
	int maxLive, live, attack, defense, minDamage, maxDamage;
	string name; // ��� (����� ��� ������)
	
	// ��������� ����� ���� ��������� �� ������� (enemy)
	// ������������� �� ������������ ���������, ������ ������� �����������, ���������� ����������� ����������� �����
	// ����� ��������� ���������� �������� � ������������ ��������
	int hit(Creature *enemy){
		int modAttack, i, x;
		if (enemy == NULL) return 0;
		modAttack = attack - enemy->defense + 1;
		if (modAttack < 1) modAttack = 1;
		for (i = 0; i < modAttack; ++i){
			if ((rand() % 6 + 1) >= 5){
				x = rand() % (maxDamage - minDamage + 1) + minDamage;
				enemy->live -= x;
				return x;
			}
		}
		return 0;
	}

	// �������� ������������� ������� ������
	// � ������ ��������� ������ 
	void init(){
		int tmp;
		if (live <= 0 || live > maxHealth){
			maxLive = rand() % (maxHealth + 1);
			live = maxLive;
		}
		else{
			maxLive = live;
			live = live;
		}
		if (attack < minAtDef || attack > maxAtDef) attack = rand() % maxAtDef + 1;
		else attack = attack;
		if (defense < minAtDef || defense > maxAtDef) defense = rand() % maxAtDef + 1;
		else defense = defense;
		if (minDamage < minDam || minDamage > maxDam) minDamage = minDam;
		if (maxDamage < minDam || maxDamage > maxDam) maxDamage = maxDam;
		if (minDamage > maxDamage){
			tmp = minDamage;
			minDamage = maxDamage;
			maxDamage = tmp;
		}
	};

	virtual ~Creature() = 0{};
	virtual int liveTime() = 0{}; // ����������� ����� ����� �������
};

class Player : public Creature{
public:
	int heals;// ������� ���������� �������������� ��������

	// �����������, ����� ������������ ���������� ��������, ��������� ������������ ����� init
	// ��� �������� ������������ ����� ������
	Player(int _live = 0, int _attack = 0, int _defense = 0, int _minDamage = 0, int _maxDamage = 0){
		live = _live;
		attack = _attack;
		defense = _defense;
		minDamage = _minDamage;
		maxDamage = _maxDamage;
		name = "�����";
		init();
		heals = kolHeals;
	};

	// ����� ����� ������� ������, ���������� 0 ���� ����, 1 ���� ���
	// ��� ���������� ���������������� �������� + 2
	int liveTime(){
		int x;
		if (live <= 0) return 0;
		if (heals >= 1){
			x = maxLive*maxHealsPercent / 100;
			if (maxLive - x > live){
				live += x;
				--heals;
				return 2 + x;
			}
		}
		return 1;		
	}
	~Player(){};
};

class Monstr : public Creature{
public:
	// �����������, ����� ������������ ���������� ��������, ��������� ������������ ����� init
	// ��� �������� ������������ ����� ������
	Monstr(int _live = 0, int _attack = 0, int _defense = 0, int _minDamage = 0, int _maxDamage = 0){
		live = _live;
		attack = _attack;
		defense = _defense;
		minDamage = _minDamage;
		maxDamage = _maxDamage;
		name = "������";
		init();
	};
	// ����� ����� ������� �������, ���������� 0 ���� ����, 1 ���� ���
	int liveTime(){
		if (live <= 0) return 0;
		return 1;
	}
	~Monstr(){};
};

int _tmain(int argc, _TCHAR* argv[])
{
	struct uzel{ // ������������ ������ �������� ������� ����
		Creature* unit;
		uzel *next, *prev;
	};
	uzel *nach, *tmp1, *tmp2, *tmp3;
	int kolvo, i, x, y, res;
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	//����� ���������� �������, ��������� ������� 2-5
	cout << "������� ���������� ������� (2-5) =>";
	cin >> kolvo;
	if (kolvo < 2 || kolvo > 5) kolvo = 5;
	cout << "������� " << kolvo << "�������" << endl;	
	// �������� ������ �������
	// �������� ��������� ��������
	tmp1 = NULL;
	tmp2 = NULL;
	nach = new uzel;
	for (i = 0; i < kolvo; ++i){
		if (tmp1 == NULL){			
			tmp1 = nach;
		}
		else{
			tmp1 = new uzel;
			tmp2->next = tmp1;
		}
		tmp1->prev = tmp2;
		tmp1->next = NULL;
		if (rand() % 2 == 1) tmp1->unit = new Player();
		else tmp1->unit = new Monstr();
		tmp2 = tmp1;
	}

	// ���� ����, ���� ������ ���� �� ��������� ���� ��������
	while (true){
		cout << "           ������ �������:" << endl;
		cout << "         ����� " << "����� " << "������ " << "����" << endl;
		tmp1 = nach;
		i = 1;
		while (tmp1 != NULL){ // ����� ������ � ���������� �������
			cout << i << " " << tmp1->unit->name << "   " << tmp1->unit->live << "    " << tmp1->unit->attack << "    "
				<< tmp1->unit->defense << "     " << tmp1->unit->minDamage << "-" << tmp1->unit->maxDamage << endl;
			tmp1 = tmp1->next;
			++i;
		}
		// ���� ��� ���� ����
		cout << "������� ��� ����� ����� ������, ��� ���� ������, ��� ���� ��� ������ =>";
		cin >> x >> y;
		if (x == 0 || y == 0) break;
		if (x<1 || x>kolvo || y<1 || y>kolvo || x == y){
			cout << "�� ������ ������" << endl;
			continue;
		}
		tmp3 = nach;
		i = 1;
		res = 0;
		// ����� � ������ ���� �����������
		while (tmp3 != NULL){
			if (x == i){
				tmp1 = tmp3;
				++res;
			}
			if (y == i){
				tmp2 = tmp3;
				++res;
			}
			if (res == 2) break;
			i++;
			tmp3 = tmp3->next;
		}
		i = tmp1->unit->hit(tmp2->unit); // ������� �����
		// ��������� ������� � ������ �������� �����
		if (i == 0){
			cout << "���� ��������..." << endl;
		}
		else{
			cout << tmp1->unit->name << " �" << x << " ����� ����: " << i << endl;
			res=tmp2->unit->liveTime();
			if (res == 0){
				--kolvo;
				if (kolvo == 1){
					cout << tmp2->unit->name << " �" << y << " �����!" << endl;
					cout<<tmp1->unit->name<< " �"<< x << " �������!" << endl;
					break;
				}
				else{
					cout << tmp2->unit->name << " �" << y << " �����!" << endl;
					if (tmp2 == nach) {
						nach = tmp2->next;
						nach->prev = NULL;
						delete tmp2->unit;
					}
					else{
						tmp2->prev->next = tmp2->next;
						if (tmp2->next != NULL) tmp2->next->prev = tmp2->prev;
					}
				}
				continue;
			}
			if (res >1){
				cout << tmp2->unit->name << " �" << y << " ������� "<< res-2 << endl;
			}
		}
	}
	system("pause");
	return 0;
}

