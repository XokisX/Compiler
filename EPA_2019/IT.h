#pragma once
#define ID_MAXSIZE		8			//���� ����� �������� ��������������
#define TI_MAXSIZE		4096		//���� ����� ���������� ����� � ������� ���������������
#define TI_SHORT_DEFAULT	0		//�������� �� ��������� ��� int
#define TI_STR_DEFAULT	NULL		//�������� �� ��������� ��� string
#define TI_NULLIDX		0xffffffff	//��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255         //����������� ���������� ����� ������
#define SHORT_MAXSIZE     32767       //����������� ���������� ��������
#define SHORT_MINUS_MAXSIZE   -32767  //���������� ���������� ��������


namespace IT
{
	enum IDDATATYPE { SHR = 1, STR = 2 };	        //���� ������ ���������������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };		//���� ���������������: ����������, �������, ��������, �������,��������.����.
	struct Entry
	{
		int			idxfirstLE;						//������ � ������� ������	
		int         line;                           //������ ������ � �������� ����
		char		id[ID_MAXSIZE];					//�������������, ��������� ��������� �� ID_MAXSIZE
		IDDATATYPE	iddatatype;						//��� ������
		IDTYPE		idtype;							//��� ��������������
		union
		{
			int	vshort;								//��������	
			struct
			{
				int len;							//���������� �������� � string
				char str[TI_STR_MAXSIZE - 1];		//������� string
			}vstr;									//�������� string
		}value;										//�������� ��������������
		Entry()                                     //����������� ��� ����������
		{
			this->value.vshort = TI_SHORT_DEFAULT;
			this->value.vstr.len = NULL;
		}
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype); //����������� � �����������
	};
	struct IdTable									//��������� ������� ���������������
	{
		int maxsize;								//������� ������� ��������������� < TI_MAXSIZE
		int size;									//������� ������ ������� ��������������� < maxsize
		Entry* table;								//������ ����� ������� ���������������
	};
	IdTable Create(int size = NULL);				//������� ������� ��������������� < TI_MAXSIZE
	void Add(										//�������� ������ � ������� ���������������
		IdTable &idtable,							//��������� ������� ���������������
		Entry entry);								//������ ������� ��������������� 
	Entry GetEntry(									//�������� ������ ������� ���������������
		IdTable &idtable,							//��������� ������� ���������������
		int n);										//����� ���������� ������
	int IsId(										//�������: ����� ������(���� ����), TI_NULLIDX(���� ����)
		IdTable &idtable,							//��������� ������� ���������������
		char id[ID_MAXSIZE]);						//�������������
	int GetSize(IdTable &idtable);					//�������� ������
	void SetValue(IdTable &idtable, int, char*);	//������ �������� ��������������
}