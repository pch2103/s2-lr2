// s2-lr2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//Создать класс clinic со следующими элементами :
//данные - члены класса :
//char* fio; //фио врача
//int specialty; (pri) // специальность(1-терапевт, 2-кардиолог, 3- стоматолог)
//int qualification; (pri) //квалификация (0-высшая, 1- первая, 2- вторая)
//static int count; // счетчик существующих объектов класса
//функции - члены класса
//1. Создать обязательные функции, указанные в общих требованиях(МК).
//2. Создать функции для данного варианта :
// Поиск врача по фио(ДФ)
// Фильтр по квалификации и специальности(найти врачей заданной
//	специальности и квалификации) (МК)
//	 Сортировать по алфавиту(МК)

#include <iostream>
#include <cstring>
#include <Windows.h>

#pragma warning (disable : 4996) //Без этого ругается на fopen и strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define FIO_WITH 60
#define BASE_NAME "clinicBase.txt"

class Clinic {
	char* fio;
	int specialty;
	int qualification;
	static int count;
public:

	Clinic() //конструктор по-умолчанию
	{
		std::cout << "Clinic::Kонструктор по-умолчанию" << std::endl;
		fio = new char[FIO_WITH];
		strcpy(fio, "");
	}

	Clinic(char* _fio, int _specialty=3, int _qualification=2): //конструктор c параметрами
		specialty(_specialty), qualification(_qualification), fio(new char[strlen(_fio) + 1])
	{
		std::cout << "Clinic::Kонструктор c параметрами" << std::endl;
		strcpy(fio, _fio);
	}

	Clinic(const Clinic& CL): 
		specialty(CL.specialty), qualification(CL.qualification), fio(new char[strlen(CL.fio)+1])
		//Kонструктор Копирования
	{
		std::cout << "Clinic::Kонструктор Копирования" << std::endl;
		strcpy(fio, CL.fio);
	}

	~Clinic()
	{
		std::cout << "~Clinic::Деструктор" << std::endl;
		delete[] fio;
	}

	// Перегрузка оператора присваивания
	Clinic& operator= (const Clinic &CL)
	{
		std::cout << "Clinic::Оператор присваивания" << std::endl;
		specialty = CL.specialty;
		qualification = CL.qualification;
		strcpy(fio, CL.fio);
		return *this;
	}

	void friend showOneRecord(Clinic, int);

	//МК - методы класса ===============================================

	void enterNewRecord(); //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
	Clinic* addNewRecord(Clinic*); //выделение памяти под новую структу и заполнение ее данными

	void show() //для тестирования, показывает поля
	{ 
		std::cout << "fio: " << fio << " :: specialty: " << specialty << " :: qualification: " << qualification << std::endl;
	}

	int get_count() { return count; } //получить количество записей в памяти
	void add_count() { ++count; } //увеличить количество записей в памяти на 1 (при добавлении записи)
};

int Clinic::count = 0;

//протоипы функций
void showAllRecords(Clinic*); //показывает все записи в памяти
void showRecordHeader(const char*); //вывод заголовка таблицы для записей
int menu(); //Меню функций базы

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choice;
	
	Clinic* myClynic = nullptr;
	Clinic tmpClynic;

	do {
		choice = menu();


		switch (choice)
		{
		case 1: //1 - Добавить новую запись
			tmpClynic.enterNewRecord();
			myClynic = tmpClynic.addNewRecord(myClynic);
			break;
		case 2: //2 - Показать все записи
			showAllRecords(myClynic);
			break;
		case 3: //3 - Фильтр по квалификации
			break;
		case 4: //4 - Фильтр по специальности
			break;
		case 5: //5 - Сортировка в алфавитном порядке
			break;
		default:
			break;
		}
	} while (choice != 0);


} //main


void Clinic::enterNewRecord() //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
{
	char buffer[FIO_WITH];
	int spec, qual;

	std::cout << "==================================" << std::endl;
	std::cout << "Добавление новой записи:" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "ФИО врача: ";
	std::cin.getline(buffer, FIO_WITH);

	strcpy(this->fio, buffer);


	do
	{
		std::cout << "Cпециальность (1-терапевт, 2-кардиолог, 3-стоматолог): ";
		std::cin >> spec;
		std::cin.get();
	} while (spec < 1 || spec > 3);

	do
	{
		std::cout << "Квалификация (0-высшая, 1-первая, 2-вторая): ";
		std::cin >> qual;
		std::cin.get();
	} while (qual < 0 || qual > 2);

	this->specialty = spec;
	this->qualification = qual;

} // enterNewRecord()

Clinic* Clinic::addNewRecord(Clinic* P) //выделение памяти под новую структу и заполнение ее данными
{
		int amount = this->get_count();

		if (amount == 0)
		{
			P = new Clinic(this->fio, this->specialty, this->qualification);
		}
		else  
		{
			Clinic *T = new Clinic[amount + 1];

			for (int i = 0; i < amount; ++i)  // копируем во временный объект	
			{
				T[i] = P[i];
				P[i].~Clinic(); // Удаляем ранние записи из памяти (delete[] P; - не работает) вызываем деструктор явно для каждой записи
			}

			T[amount] = Clinic(this->fio, this->specialty, this->qualification); //Добавляем новую запись
			//delete[] P; - не работает см. выше
			P = T; //временный объект становится "присланным"
		}

		this->add_count(); // Увеличиваем счетчик (static-переменная)

		return P;
} // addNewRecord()


void showRecordHeader(const char* title) //вспомогательная функция - печать заголовка таблицы
{
	std::cout << std::endl << "===================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "===================================================================" << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(6);
	std::cout << " №";
	std::cout.setf(std::ios::left);
	std::cout.width(FIO_WITH);
	std::cout << "ФИО врача";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "Cпециальность";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "Квалификация";
	std::cout << std::endl << "===================================================================" << std::endl;
}

void showOneRecord(Clinic Record, int number = 1) //вспомогательная функция - печать одной записи
{
	std::cout.setf(std::ios::left);
	std::cout << " ";
	std::cout.width(5);
	std::cout << number;
	std::cout.setf(std::ios::left);
	std::cout.width(FIO_WITH);
	std::cout << Record.fio;
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << Record.specialty;
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << Record.qualification;
	std::cout << std::endl;
}

void showAllRecords(Clinic* P) //показывает все записи в памяти
{
	showRecordHeader("Все записи в базе поликлиники:");

	for (int i = 0; i < P->get_count(); ++i) {
		showOneRecord(P[i], i + 1);
	} 
	std::cout << "===================================================================" << std::endl;
}// showAllRecords()

int menu() //Меню функций базы
{
		int select;
		std::cout << "==================================" << std::endl;
		std::cout << "Меню:" << std::endl;
		std::cout << "==================================" << std::endl;
		std::cout << "1 - Добавить новую запись" << std::endl;
		std::cout << "2 - Показать все записи" << std::endl;
		std::cout << "3 - Фильтр по квалификации" << std::endl;
		std::cout << "4 - Фильтр по специальности" << std::endl;
		std::cout << "5 - Сортировка в алфавитном порядке" << std::endl;
		std::cout << "0 - Выход из программы и запись" << std::endl;
		std::cout << "----------------------------------" << std::endl;

		do
		{
			std::cout << "Введите номер пункта меню: ";
			std::cin >> select;
			std::cin.get();
		} while (select < 0 || select > 5);

		return select;
} // menu()


