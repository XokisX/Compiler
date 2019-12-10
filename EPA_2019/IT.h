#pragma once
#define ID_MAXSIZE		8			//макс число символов идентификатора
#define TI_MAXSIZE		4096		//макс число количество строк в таблице идентификаторов
#define TI_SHORT_DEFAULT	0xffff		//значение по умолчанию дл€ int
#define TI_STR_DEFAULT	NULL		//значение по умолчанию дл€ string
#define TI_NULLIDX		0xffffffff	//нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255         //максимально допустима€ длина строки
#define SHORT_MAXSIZE     65534       //максимально допустимое значение
#define SHORT_MINUS_MAXSIZE   0  //минимально допустимое значение


namespace IT
{
	enum IDDATATYPE { SHR = 1, STR = 2};	        //типы данных идентификаторов
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };		//типы идентификаторов: переменна€, функци€, параметр, литерал,стандарт.библ.
	struct Entry
	{
		int			idxfirstLE;						//индекс в таблице лексем	
		int         line;                           //индекс строки в исходном коде
		char		id[ID_MAXSIZE];					//идентификатор, автоматом усекаетс€ до ID_MAXSIZE
		IDDATATYPE	iddatatype;						//тип данных
		IDTYPE		idtype;							//тип идентификатора
		union
		{
			int	vshort;								//значение	
			struct
			{
				int len;							//количество символов в string
				char str[TI_STR_MAXSIZE - 1];		//символы string
			}vstr;	
			bool boolvalue;
			//значение string
		}value;										//значение идентификатора
		Entry()                                     //конструктор без параметров
		{
		
		}
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype); //конструктор с параметрами
	};
	struct IdTable									//экземпл€р таблицы идентификаторов
	{
		int maxsize;								//Ємкость таблицы идентификаторов < TI_MAXSIZE
		int size;									//текущий размер таблицы идентификаторов < maxsize
		Entry* table;								//массив строк таблицы идентификаторов
	};
	IdTable Create(int size = NULL);				//Ємкость таблицы идентификаторов < TI_MAXSIZE
	void Add(										//добавить строку в таблицу идентификаторов
		IdTable &idtable,							//экземпл€р таблицы идентификаторов
		Entry entry);								//строка таблицы идентификаторов 
	Entry GetEntry(									//получить строку таблицы идентификаторов
		IdTable &idtable,							//экземпл€р таблицы идентификаторов
		int n);										//номер получаемой строки
	int IsId(										//возврат: номер строки(если есть), TI_NULLIDX(если есть)
		IdTable &idtable,							//экземпл€р таблицы идентификаторов
		char id[ID_MAXSIZE]);						//идентификатор
	int GetSize(IdTable &idtable);					//получить размер
	void SetValue(IdTable &idtable, int, char*);	//задать значение идентификатора
}