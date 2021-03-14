// s2-lr2.cpp : ���� ���� �������� ������� "main". ����� ���������� � ������������� ���������� ���������.
//
//������� ����� clinic �� ���������� ���������� :
//������ - ����� ������ :
//char* fio; //��� �����
//int specialty; (pri) // �������������(1-��������, 2-���������, 3- ����������)
//int qualification; (pri) //������������ (0-������, 1- ������, 2- ������)
//static int count; // ������� ������������ �������� ������
//������� - ����� ������
//1. ������� ������������ �������, ��������� � ����� �����������(��).
//2. ������� ������� ��� ������� �������� :
// ����� ����� �� ���(��)
// ������ �� ������������ � �������������(����� ������ ��������
//	������������� � ������������) (��)
//	 ����������� �� ��������(��)

#include <iostream>
#include <cstring>
#include <Windows.h>

#pragma warning (disable : 4996) //��� ����� �������� �� fopen � strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define FIO_WITH 60
#define BASE_NAME "clinicBase.txt"

class Clinic {
	char* fio;
	int specialty;
	int qualification;
	static int count;
public:

	Clinic() //����������� ��-���������
	{
		std::cout << "Clinic::K���������� ��-���������" << std::endl;
		fio = new char[FIO_WITH];
		strcpy(fio, "");
	}

	Clinic(char* _fio, int _specialty=3, int _qualification=2): //����������� c �����������
		specialty(_specialty), qualification(_qualification), fio(new char[strlen(_fio) + 1])
	{
		std::cout << "Clinic::K���������� c �����������" << std::endl;
		strcpy(fio, _fio);
	}

	Clinic(const Clinic& CL): 
		specialty(CL.specialty), qualification(CL.qualification), fio(new char[strlen(CL.fio)+1])
		//K���������� �����������
	{
		std::cout << "Clinic::K���������� �����������" << std::endl;
		strcpy(fio, CL.fio);
	}

	~Clinic()
	{
		std::cout << "~Clinic::����������" << std::endl;
		delete[] fio;
	}

	// ���������� ��������� ������������
	Clinic& operator= (const Clinic &CL)
	{
		std::cout << "Clinic::�������� ������������" << std::endl;
		specialty = CL.specialty;
		qualification = CL.qualification;
		strcpy(fio, CL.fio);
		return *this;
	}

	void friend showOneRecord(Clinic, int);

	//�� - ������ ������ ===============================================

	void enterNewRecord(); //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
	Clinic* addNewRecord(Clinic*); //��������� ������ ��� ����� ������� � ���������� �� �������

	void show() //��� ������������, ���������� ����
	{ 
		std::cout << "fio: " << fio << " :: specialty: " << specialty << " :: qualification: " << qualification << std::endl;
	}

	int get_count() { return count; } //�������� ���������� ������� � ������
	void add_count() { ++count; } //��������� ���������� ������� � ������ �� 1 (��� ���������� ������)
};

int Clinic::count = 0;

//�������� �������
void showAllRecords(Clinic*); //���������� ��� ������ � ������
void showRecordHeader(const char*); //����� ��������� ������� ��� �������
int menu(); //���� ������� ����

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
		case 1: //1 - �������� ����� ������
			tmpClynic.enterNewRecord();
			myClynic = tmpClynic.addNewRecord(myClynic);
			break;
		case 2: //2 - �������� ��� ������
			showAllRecords(myClynic);
			break;
		case 3: //3 - ������ �� ������������
			break;
		case 4: //4 - ������ �� �������������
			break;
		case 5: //5 - ���������� � ���������� �������
			break;
		default:
			break;
		}
	} while (choice != 0);


} //main


void Clinic::enterNewRecord() //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
{
	char buffer[FIO_WITH];
	int spec, qual;

	std::cout << "==================================" << std::endl;
	std::cout << "���������� ����� ������:" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "��� �����: ";
	std::cin.getline(buffer, FIO_WITH);

	strcpy(this->fio, buffer);


	do
	{
		std::cout << "C������������ (1-��������, 2-���������, 3-����������): ";
		std::cin >> spec;
		std::cin.get();
	} while (spec < 1 || spec > 3);

	do
	{
		std::cout << "������������ (0-������, 1-������, 2-������): ";
		std::cin >> qual;
		std::cin.get();
	} while (qual < 0 || qual > 2);

	this->specialty = spec;
	this->qualification = qual;

} // enterNewRecord()

Clinic* Clinic::addNewRecord(Clinic* P) //��������� ������ ��� ����� ������� � ���������� �� �������
{
		int amount = this->get_count();

		if (amount == 0)
		{
			P = new Clinic(this->fio, this->specialty, this->qualification);
		}
		else  
		{
			Clinic *T = new Clinic[amount + 1];

			for (int i = 0; i < amount; ++i)  // �������� �� ��������� ������	
			{
				T[i] = P[i];
				P[i].~Clinic(); // ������� ������ ������ �� ������ (delete[] P; - �� ��������) �������� ���������� ���� ��� ������ ������
			}

			T[amount] = Clinic(this->fio, this->specialty, this->qualification); //��������� ����� ������
			//delete[] P; - �� �������� ��. ����
			P = T; //��������� ������ ���������� "����������"
		}

		this->add_count(); // ����������� ������� (static-����������)

		return P;
} // addNewRecord()


void showRecordHeader(const char* title) //��������������� ������� - ������ ��������� �������
{
	std::cout << std::endl << "===================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "===================================================================" << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(6);
	std::cout << " �";
	std::cout.setf(std::ios::left);
	std::cout.width(FIO_WITH);
	std::cout << "��� �����";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "C������������";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "������������";
	std::cout << std::endl << "===================================================================" << std::endl;
}

void showOneRecord(Clinic Record, int number = 1) //��������������� ������� - ������ ����� ������
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

void showAllRecords(Clinic* P) //���������� ��� ������ � ������
{
	showRecordHeader("��� ������ � ���� �����������:");

	for (int i = 0; i < P->get_count(); ++i) {
		showOneRecord(P[i], i + 1);
	} 
	std::cout << "===================================================================" << std::endl;
}// showAllRecords()

int menu() //���� ������� ����
{
		int select;
		std::cout << "==================================" << std::endl;
		std::cout << "����:" << std::endl;
		std::cout << "==================================" << std::endl;
		std::cout << "1 - �������� ����� ������" << std::endl;
		std::cout << "2 - �������� ��� ������" << std::endl;
		std::cout << "3 - ������ �� ������������" << std::endl;
		std::cout << "4 - ������ �� �������������" << std::endl;
		std::cout << "5 - ���������� � ���������� �������" << std::endl;
		std::cout << "0 - ����� �� ��������� � ������" << std::endl;
		std::cout << "----------------------------------" << std::endl;

		do
		{
			std::cout << "������� ����� ������ ����: ";
			std::cin >> select;
			std::cin.get();
		} while (select < 0 || select > 5);

		return select;
} // menu()


