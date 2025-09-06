// ConsoleApplication3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <locale>
#include <random>
#include <iostream>

using namespace std;

class Creature{ //виртуальный родительский класс всех существ
protected:
	
	// установочные данные для всех существ
	static const int maxHealth = 100; // максимум возможного здоровья
	static const int minAtDef = 1; // минимальная атака и защита
	static const int maxAtDef = 30; // максимальная атака и защита
	static const int minDam = 1; // минимальный урон
	static const int maxDam = 30; // максимальный урон
	static const int kolHeals = 4; // количество раз восстановления здоровья у игрока
	static const int maxHealsPercent = 30; // процент восстановления здоровья у игрока
public:
	// кол-во здоровья, текущее здоровье, атака, защита, мин и макс урон
	int maxLive, live, attack, defense, minDamage, maxDamage;
	string name; // имя (игрок или монстр)
	
	// нанесение удара этем существом по другому (enemy)
	// расчитывается по предложеному алгоритму, вместо кубиков рандомайзер, возвращает колличество нанесенного урона
	// также уменьшает количество здоровья у атакованного существа
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

	// проверка инициализации обьекта класса
	// в случае ошибочных данных 
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
	virtual int liveTime() = 0{}; // виртуальный метод жизни объекта
};

class Player : public Creature{
public:
	int heals;// текущее количество восстановлений здоровья

	// конструктор, после прописывания параметров существа, запускает родительский метод init
	// для проверки правильности ввода данных
	Player(int _live = 0, int _attack = 0, int _defense = 0, int _minDamage = 0, int _maxDamage = 0){
		live = _live;
		attack = _attack;
		defense = _defense;
		minDamage = _minDamage;
		maxDamage = _maxDamage;
		name = "игрок";
		init();
		heals = kolHeals;
	};

	// метод жизни объекта игрока, возвращает 0 если умер, 1 если жив
	// или количество восстановленного здоровья + 2
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
	// конструктор, после прописывания параметров существа, запускает родительский метод init
	// для проверки правильности ввода данных
	Monstr(int _live = 0, int _attack = 0, int _defense = 0, int _minDamage = 0, int _maxDamage = 0){
		live = _live;
		attack = _attack;
		defense = _defense;
		minDamage = _minDamage;
		maxDamage = _maxDamage;
		name = "монстр";
		init();
	};
	// метод жизни объекта монстра, возвращает 0 если умер, 1 если жив
	int liveTime(){
		if (live <= 0) return 0;
		return 1;
	}
	~Monstr(){};
};

int _tmain(int argc, _TCHAR* argv[])
{
	struct uzel{ // двусвязанный список обьектов существ игры
		Creature* unit;
		uzel *next, *prev;
	};
	uzel *nach, *tmp1, *tmp2, *tmp3;
	int kolvo, i, x, y, res;
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	//выбор количества существ, ограничил цифрами 2-5
	cout << "Введите количество существ (2-5) =>";
	cin >> kolvo;
	if (kolvo < 2 || kolvo > 5) kolvo = 5;
	cout << "Выбрано " << kolvo << "существ" << endl;	
	// создание списка существ
	// существа создаются рандомно
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

	// цикл игры, игра длится пока не останится одно существо
	while (true){
		cout << "           список существ:" << endl;
		cout << "         жизнь " << "атака " << "защита " << "урон" << endl;
		tmp1 = nach;
		i = 1;
		while (tmp1 != NULL){ // вывод списка и параметров существ
			cout << i << " " << tmp1->unit->name << "   " << tmp1->unit->live << "    " << tmp1->unit->attack << "    "
				<< tmp1->unit->defense << "     " << tmp1->unit->minDamage << "-" << tmp1->unit->maxDamage << endl;
			tmp1 = tmp1->next;
			++i;
		}
		// ввод кто кого бьет
		cout << "введите два числа через пробел, кто кого ударит, или нули для выхода =>";
		cin >> x >> y;
		if (x == 0 || y == 0) break;
		if (x<1 || x>kolvo || y<1 || y>kolvo || x == y){
			cout << "не верные данные" << endl;
			continue;
		}
		tmp3 = nach;
		i = 1;
		res = 0;
		// поиск в списке двух противников
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
		i = tmp1->unit->hit(tmp2->unit); // попытка удара
		// обработка события в случае удачного удара
		if (i == 0){
			cout << "Удар неудачен..." << endl;
		}
		else{
			cout << tmp1->unit->name << " №" << x << " нанес урон: " << i << endl;
			res=tmp2->unit->liveTime();
			if (res == 0){
				--kolvo;
				if (kolvo == 1){
					cout << tmp2->unit->name << " №" << y << " погиб!" << endl;
					cout<<tmp1->unit->name<< " №"<< x << " победил!" << endl;
					break;
				}
				else{
					cout << tmp2->unit->name << " №" << y << " погиб!" << endl;
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
				cout << tmp2->unit->name << " №" << y << " вылечил "<< res-2 << endl;
			}
		}
	}
	system("pause");
	return 0;
}

