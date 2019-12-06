#include "stdafx.h"
#include <stdlib.h>


namespace IT
{

	Entry::Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype)
	{
		strncpy_s(this->id, ID_MAXSIZE, id, ID_MAXSIZE - 1);
		this->idxfirstLE = idxLT;
		this->iddatatype = datatype;
		this->idtype = idtype;
		if (datatype==STR) {
			this->value.vstr.len = TI_STR_DEFAULT;
		}
		else {
			this->value.vshort = TI_SHORT_DEFAULT;
		}
	}
	IdTable Create(int size)
	{
		IdTable idtable;
		idtable.table = new Entry[size];
		idtable.size = NULL;
		return idtable;
	}
	void Add(IdTable &idtable, Entry entry)
	{
		strncpy_s(idtable.table[idtable.size].id, entry.id, ID_MAXSIZE - 1);
		idtable.table[idtable.size].line = entry.line;
		idtable.table[idtable.size].idxfirstLE = entry.idxfirstLE;
		idtable.table[idtable.size].iddatatype = entry.iddatatype;
		idtable.table[idtable.size].idtype = entry.idtype;
		idtable.table[idtable.size].value.vshort = entry.value.vshort;
		idtable.table[idtable.size].value.vstr.len = entry.value.vstr.len;
		idtable.table[idtable.size].value.vstr.str[0] = entry.value.vstr.str[0];
		idtable.size++;
	}
	Entry GetEntry(IdTable &idtable, int n)
	{
		return idtable.table[n];
	}
	int IsId(IdTable &idtable, char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == NULL)
				return i;
		}
		return -1;
	}
	int GetSize(IdTable &idtable)
	{
		return idtable.size;
	}
	void SetValue(IdTable &idtable, int indTI, char* token)
	{
		if (idtable.table[indTI].iddatatype == STR)
		{
			if (idtable.table[indTI].value.vstr.len > TI_STR_MAXSIZE)
				throw ERROR_THROW(6);
			idtable.table[indTI].value.vstr.len = strlen(token);
			strcpy_s(idtable.table[indTI].value.vstr.str, TI_STR_MAXSIZE, token);
		}
		else
		{
			int tempShort = atoi(token);
			if (tempShort > SHORT_MAXSIZE)
			{
				throw ERROR_THROW(6);
			}
			else
			{
				if (tempShort < SHORT_MINUS_MAXSIZE) {
					throw ERROR_THROW(6);
				}
			}
			idtable.table[indTI].value.vshort = tempShort;

		}
	}
}