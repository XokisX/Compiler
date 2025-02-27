#include "stdafx.h"
#include "SA.h"


bool  SemanticAnalyze(LT::LexTable &Lextable, In::IN &InStruct, IT::IdTable &idtable, Log::LOG &log)
{
	bool Standart_LIB = false;
	int isGlavn = 0;
	bool isFunction = false;
	bool choise = true;
	int isBrace = 0;
	bool isBack = false;
	bool Viragenia_Num = true;

	int temp = 0;

	for (int i = 0, j; i < Lextable.size; i++)
	{
		switch (Lextable.table[i].lexema)
		{
		case LEX_DIRSLASH: {
			if (IT::IsId(idtable, InStruct.tokens[i+1].token) != -1 && idtable.table[IT::IsId(idtable, InStruct.tokens[i+1].token)].value.vshort==0) {
				throw ERROR_THROW_IN(109, Lextable.table[i+1].sn, NULL);
				choise = false;
				break;
			}
			if (!strcmp(InStruct.tokens[Lextable.table[i+1].tokenId].token,"0")) {
				throw ERROR_THROW_IN(109, Lextable.table[i + 1].sn, NULL);
				choise = false;
				break;
			}
			break;
		}
		case LEX_SHORT:
		{
			j = i;
			if (Lextable.table[i + 1].lexema == LEX_FUNCTION)
			{
				while (Lextable.table[j].lexema != LEX_SEPARATOR)
				{
					if (Lextable.table[j].lexema == LEX_LEFTBRACE)
						break;
					j++;
				}
				if (Lextable.table[j].lexema == LEX_SEPARATOR)
					throw ERROR_THROW(8);
				break;
			}
			break;
		}
		case LEX_FUNCTION:
		{
			
			if (Lextable.table[i - 1].lexema == LEX_SHORT && Lextable.table[i + 1].lexema == LEX_ID && Lextable.table[i + 2].lexema == LEX_LEFTHESIS)
			{
				isFunction = true;	
				int temp = i;
				int Counterbraces = 0;
					do {
						if (Lextable.table[temp].lexema==LEX_LEFTBRACE) {
							while (temp < Lextable.size) {
								if (Lextable.table[temp].lexema == LEX_LEFTBRACE) {
									Counterbraces++;
								}
								if (Lextable.table[temp].lexema == LEX_BRACELET) {
									Counterbraces--;
								}
								if (Lextable.table[temp].lexema == LEX_BACK) {
									isBack = true;
								}
								if (Counterbraces == 0) {
									break;
								}
								temp++;
							}
							break;
						}
							temp++;
					} while (temp<Lextable.size);
			}
			break;
		}

		case LEX_STANDART:
		{
			Standart_LIB = true;
			break;
		}
		case LEX_GLAVN:
		{
			if (isFunction == true || isBack == true)
			{
				throw ERROR_THROW_IN(126, Lextable.table[i].sn, NULL);
				choise = false;
				break;
			}
			isBack = true;
			isGlavn++;
			break;
		}
		case LEX_EQUAL:
		{
			if (Lextable.table[i - 1].lexema == LEX_ID && Lextable.table[i + 1].lexema == LEX_ID)
			{
				if (idtable.table[Lextable.table[i - 1].idxTI].iddatatype != idtable.table[Lextable.table[i + 1].idxTI].iddatatype)
				{
					throw ERROR_THROW_IN(3, Lextable.table[i].sn, NULL);
					choise = false;
					break;
				}
			}
			if (Lextable.table[i + 1].lexema == LEX_ID && Lextable.table[i + 2].lexema != LEX_LEFTHESIS && Lextable.table[i + 2].lexema != LEX_SEPARATOR)
			{
				int  p = 0;
				int k = i;
				while (Lextable.table[k].lexema != LEX_SEPARATOR)
				{
					if (Lextable.table[k].lexema == LEX_ID)
					{
						p = Lextable.table[k].idxTI;
						if (idtable.table[p].iddatatype != IT::SHR)
						{
							Viragenia_Num = false;
							break;
						}
					}
					k++;
				}
				if (Viragenia_Num == false)
				{
					throw ERROR_THROW_IN(107, Lextable.table[i].sn, NULL);
					choise = false;
					break;
				}
			}
			break;
		}
		case LEX_ID:
		{
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].idtype == IT::F && Lextable.table[i + 1].lexema == LEX_EQUAL)
			{
				throw ERROR_THROW_IN(119, Lextable.table[i].sn, NULL);
				choise = false;
			}
			/*if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && (Lextable.table[i - 2].lexema == LEX_IF))
			{

				if (IT::GetEntry(idtable, IT::IsId(idtable, InStruct.tokens[i].token)).value.vshort == 0)
					throw ERROR_THROW_IN(3, Lextable.table[i].sn, NULL);
			}*/
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].idtype == IT::F)
			{

				int kol2 = 0;

				int a = idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].idxfirstLE;
				int saveA = a;

				while (Lextable.table[a].lexema != LEX_RIGHTTHESIS)
				{
					if (Lextable.table[a].lexema == LEX_COMMA)
						kol2++;
					a++;
				}
				int kol = 0;

				while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
				{
					if (Lextable.table[i].lexema == LEX_COMMA)
						kol++;
					i++;
				}
				if (kol != kol2)
				{

					throw ERROR_THROW_IN(122, Lextable.table[i].sn, NULL);
					choise = false;
				}
				else if (a != i) {
					int numI = 1;
					int numA = 1;

						for (int z = 0; z <= kol; z++) {
							if (IT::IsId(idtable, InStruct.tokens[i - numI].token) != -1 && IT::IsId(idtable, InStruct.tokens[a - numA].token) != -1) {
								if (idtable.table[IT::IsId(idtable, InStruct.tokens[i - numI].token)].iddatatype == idtable.table[IT::IsId(idtable, InStruct.tokens[a - numA].token)].iddatatype) {
									numI += 2;
									numA += 3;

								}
								else {
									throw ERROR_THROW_IN(114, Lextable.table[i].sn, NULL);
								}
							}
							else {
								throw ERROR_THROW_IN(601, Lextable.table[i].sn, NULL);
							}

					}

				}
				while (Lextable.table[a].lexema != LEX_BRACELET) {
					a++;
					if (Lextable.table[a].lexema == LEX_BACK) {
						break;
					}
				}
				/*if (Lextable.table[a].lexema == LEX_BRACELET) {
					throw ERROR_THROW(126);
					choise = false;
				}
				if (IT::IsId(idtable, InStruct.tokens[a + 1].token) == -1 || idtable.table[IT::IsId(idtable, InStruct.tokens[a + 1].token)].iddatatype != idtable.table[IT::IsId(idtable, InStruct.tokens[saveA].token)].iddatatype) {
					throw ERROR_THROW_IN(3, Lextable.table[i].sn, NULL);
				}*/
				break;
			}
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].idtype == IT::F)
			{
				int kol2 = 0;
				int a = idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].idxfirstLE;
				while (Lextable.table[a].lexema != LEX_RIGHTTHESIS)
				{
					if (Lextable.table[a].lexema == LEX_COMMA)
						kol2++;
					a++;
				}
				int kol = 0;
				while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
				{
					if (Lextable.table[i].lexema == LEX_COMMA)
						kol++;
					i++;
				}
				if (kol != kol2)
				{
					throw ERROR_THROW_IN(122, Lextable.table[i].sn, NULL);
					choise = false;
				}
				break;
			}
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && (Lextable.table[i - 2].lexema == LEX_STRLEN))
			{
				int check;
				check = IT::IsId(idtable, InStruct.tokens[i].token);
				if (idtable.table[check].iddatatype == IT::SHR && (Lextable.table[i - 2].lexema == LEX_STRLEN))
				{
					throw ERROR_THROW_IN(127, Lextable.table[i].sn, NULL);
					choise = false;
					break;
				}
				break;
			}
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].iddatatype == IT::SHR && InStruct.tokens[i + 2].token[0] == char(34))
			{
				throw ERROR_THROW_IN(4, Lextable.table[i].sn, NULL);
				choise = false;
			}
			if (Lextable.table[i-2].lexema==LEX_DEF&&(IT::IsId(idtable, InStruct.tokens[i].token) != -1 && idtable.table[IT::IsId(idtable, InStruct.tokens[i].token)].iddatatype == IT::STR && !InStruct.tokens[i + 2].isStr))
			{
				if (IT::IsId(idtable, InStruct.tokens[i+2].token) == -1) {
					throw ERROR_THROW_IN(613, Lextable.table[i].sn, NULL);
					choise = false;
				}
				else {
					throw ERROR_THROW_IN(4, Lextable.table[i].sn, NULL);
					choise = false;
				}
			}
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1)
			{
				if (Lextable.table[i-1].lexema==LEX_SHORT && Lextable.table[i - 2].lexema != LEX_DEF){
					throw ERROR_THROW_IN(105, Lextable.table[i].sn, NULL);
					choise = false;
					break;
				}else{
					if (Lextable.table[i - 1].lexema == LEX_FUNCTION && Lextable.table[i - 2].lexema != LEX_SHORT) {
						throw ERROR_THROW_IN(105, Lextable.table[i].sn, NULL);
						choise = false;
						break;
					}
					//if (/*Lextable.table[i - 2].lexema != LEX_FUNCTION && Lextable.table[i - 1].lexema == LEX_SHORT&&*/IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i + 1].lexema == LEX_EQUAL) {
					//	int j = i + 2;
					//	while (Lextable.table[j].lexema != LEX_SEPARATOR)
					//	{
					//		if (Lextable.table[j].lexema != LEX_SHORT && Lextable.table[j].lexema != LEX_ID && Lextable.table[j].lexema != LEX_PLUS
					//			&& Lextable.table[j].lexema != LEX_MINUS && Lextable.table[j].lexema != LEX_STAR && Lextable.table[j].lexema != LEX_DIRSLASH
					//			&& Lextable.table[j].lexema != LEX_LITERAL && Lextable.table[j].lexema != LEX_RIGHTTHESIS && Lextable.table[j].lexema != LEX_LEFTHESIS
					//			&& Lextable.table[j].lexema != LEX_CMP && Lextable.table[j].lexema != LEX_STRLEN && Lextable.table[j].lexema != LEX_COMMA) {
					//			throw ERROR_THROW_IN(127, Lextable.table[i].sn, NULL);
					//			choise = false;
					//			break;
					//		}
					//		j++;
					//	}
					//}
				}
			
			}
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i+2].lexema==LEX_MINUS) {
				throw ERROR_THROW_IN(6, Lextable.table[i].sn, NULL);
				choise = false;
				break;
			}
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i-1].lexema==LEX_SHORT && Lextable.table[i - 2].lexema == LEX_DEF 
				&& Lextable.table[i + 1].lexema != LEX_EQUAL && Lextable.table[i + 1].lexema != LEX_SEPARATOR) {
				throw ERROR_THROW_IN(127, Lextable.table[i].sn, NULL);
				choise = false;
				break;
			}
			if ((IT::IsId(idtable, InStruct.tokens[i].token) != -1 &&
				Lextable.table[i + 1].lexema == LEX_EQUAL &&
				Lextable.table[i + 2].lexema == LEX_ID &&
				idtable.table[Lextable.table[i + 2].idxTI].value.vshort== TI_SHORT_DEFAULT&&
				idtable.table[Lextable.table[i + 2].idxTI].value.vstr.len == TI_SHORT_DEFAULT&&
				Lextable.table[i + 3].lexema == LEX_SEPARATOR))
			{
				throw ERROR_THROW_IN(105, Lextable.table[i].sn, NULL);
				choise = false;
			}
			if ((IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i + 1].lexema == LEX_EQUAL && InStruct.tokens[i + 2].isStr == true && (Lextable.table[i + 3].lexema == LEX_PLUS || Lextable.table[i + 3].lexema == LEX_MINUS || Lextable.table[i + 3].lexema == LEX_DIRSLASH || Lextable.table[i + 3].lexema == LEX_STAR) && InStruct.tokens[i + 4].isStr == true))
			{
				throw ERROR_THROW_IN(108, Lextable.table[i].sn, NULL);
				choise = false;
			}
			if ((IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i + 1].lexema == LEX_EQUAL && (Lextable.table[i + 2].lexema == LEX_ID || Lextable.table[i + 2].lexema == LEX_LITERAL) && Lextable.table[i + 3].lexema == LEX_DIRSLASH && InStruct.tokens[i + 4].token[0] == char(48) && Lextable.table[i + 5].lexema == LEX_SEPARATOR))
			{
				throw ERROR_THROW_IN(109, Lextable.table[i].sn, NULL);
				choise = false;
			}
			
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i + 1].lexema == LEX_EQUAL && (Lextable.table[i + 2].lexema == LEX_ID && idtable.table[IT::IsId(idtable, InStruct.tokens[i + 2].token)].iddatatype == IT::SHR) && (Lextable.table[i + 4].lexema == LEX_ID && idtable.table[IT::IsId(idtable, InStruct.tokens[i + 4].token)].iddatatype == IT::SHR))
				break;
			if (IT::IsId(idtable, InStruct.tokens[i].token) != -1 && Lextable.table[i + 1].lexema == LEX_EQUAL && (Lextable.table[i + 2].lexema == LEX_LITERAL && InStruct.tokens[i + 2].isStr == false) && (Lextable.table[i + 4].lexema == LEX_LITERAL && InStruct.tokens[i + 4].isStr == false))
				break;
			break;
		}
		case LEX_BACK:
		{
			if (isFunction == true)
			{
				isBack = false;
				isFunction = false;
				break;
			}
			else
			{
				if (isGlavn == 0)
					throw ERROR_THROW_IN(126, Lextable.table[i].sn, NULL);
				isBack = false;
				break;
			}
			break;
		}
		case LEX_LEFTBRACE:
		{
			isBrace++;

			break;
		}
		case LEX_BRACELET:
		{
			isBrace--;
			break;
		}
		case LEX_STRLEN:
		{
			int schet = 0;
			int b = i;
			while (Lextable.table[b].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[b].lexema == LEX_COMMA)
					schet++;
				b++;
			}
			if (schet != 0)
				throw ERROR_THROW_IN(122, Lextable.table[i].sn, NULL);
			if (Standart_LIB == false)
			{
				throw ERROR_THROW_IN(116, Lextable.table[i].sn, NULL);
				choise = false;
			}
			if (Lextable.table[i + 2].lexema == LEX_LITERAL && InStruct.tokens[i + 2].isStr == false)
			{
				throw ERROR_THROW_IN(127, Lextable.table[i].sn, NULL);
				choise = false;
			}
		
			break;
		}
		case LEX_CMP:
		{
			if ((IT::IsId(idtable, InStruct.tokens[i+2].token) == -1 || IT::IsId(idtable, InStruct.tokens[i + 4].token) == -1)||
				(idtable.table[Lextable.table[i + 2].idxTI].iddatatype != IT::STR || idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::STR)||
				(idtable.table[Lextable.table[i + 2].idxTI].value.vstr.len == 0 || idtable.table[Lextable.table[i + 4].idxTI].value.vstr.len == 0)) {
 				throw ERROR_THROW_IN(601, Lextable.table[i].sn, NULL);
				choise = false;
			}
			
			if (Standart_LIB == false)
			{
				throw ERROR_THROW_IN(116, Lextable.table[i].sn, NULL);
				choise = false;
			}
			break;
		}
		case LEX_IF:
		{
			if ((Lextable.table[i + 5].lexema != LEX_RIGHTTHESIS) || (Lextable.table[i + 3].lexema != LEX_LAGRER && Lextable.table[i + 3].lexema != LEX_LESS && Lextable.table[i + 3].lexema != LEX_NOT_EQUALS
				&& Lextable.table[i + 3].lexema != LEX_LAE && Lextable.table[i + 3].lexema != LEX_MAE && Lextable.table[i + 3].lexema != LEX_RAVN)) {
				throw ERROR_THROW_IN(607, InStruct.tokens[i + 2].line, NULL);
				choise = false;
			}
			if ((idtable.table[Lextable.table[i + 2].idxTI].value.vshort == TI_SHORT_DEFAULT && idtable.table[Lextable.table[i + 2].idxTI].idtype != IT::P)
				||(idtable.table[Lextable.table[i + 4].idxTI].value.vshort == TI_SHORT_DEFAULT && idtable.table[Lextable.table[i + 4].idxTI].idtype != IT::P))
			{
				throw ERROR_THROW_IN(604, Lextable.table[i].sn, NULL);
				choise = false;
			}
			if (idtable.table[Lextable.table[i + 2].idxTI].iddatatype != IT::SHR || idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::SHR)
			{
				throw ERROR_THROW_IN(608, Lextable.table[i].sn, NULL);
				choise = false;
			}
			break;
		}
		case LEX_OUT:
		{
			int schet = 0;
			int b = i;
			while (Lextable.table[b].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[b].lexema == LEX_SEPARATOR)
					schet++;
				b++;
			}
			
			if (schet != 0)
				throw ERROR_THROW_IN(609, Lextable.table[i].sn, NULL);
			if (idtable.table[Lextable.table[i + 2].idxTI].value.vshort == TI_SHORT_DEFAULT && idtable.table[Lextable.table[i + 2].idxTI].value.vstr.len == 0)
			{
				throw ERROR_THROW_IN(609, Lextable.table[i].sn, NULL);
				choise = false;
			}

			break;
		}

		case LEX_DEF:
		{
			if (Lextable.table[i + 2].lexema == LEX_IF || Lextable.table[i + 2].lexema == LEX_BACK || Lextable.table[i + 2].lexema == LEX_OUT || Lextable.table[i + 2].lexema == LEX_GLAVN ||
				Lextable.table[i + 2].lexema == LEX_WHILE || Lextable.table[i + 2].lexema == LEX_FUNCTION || Lextable.table[i + 2].lexema == LEX_DEF || Lextable.table[i + 2].lexema == LEX_STANDART ||
				Lextable.table[i + 2].lexema == LEX_STRLEN || Lextable.table[i + 2].lexema == LEX_CMP || Lextable.table[i + 2].lexema == LEX_SHORT|| Lextable.table[i + 2].lexema == LEX_STR||
				!strcmp(InStruct.tokens[Lextable.table[i+2].tokenId].token,"null")|| !strcmp(InStruct.tokens[Lextable.table[i + 2].tokenId].token, "var"))
			{
				throw ERROR_THROW_IN(610, Lextable.table[i].sn, NULL);
				choise = false;
			}
 			break;
		}
		case LEX_WHILE: {
			if (((IT::IsId(idtable, InStruct.tokens[i + 2].token)) == -1|| (IT::IsId(idtable, InStruct.tokens[i + 4].token)) == -1) || (Lextable.table[i + 5].lexema != LEX_RIGHTTHESIS) || (Lextable.table[i + 3].lexema != LEX_LAGRER && Lextable.table[i + 3].lexema != LEX_LESS && Lextable.table[i + 3].lexema != LEX_NOT_EQUALS
				&& Lextable.table[i + 3].lexema != LEX_LAE && Lextable.table[i + 3].lexema != LEX_MAE && Lextable.table[i + 3].lexema != LEX_RAVN)) {
				throw ERROR_THROW_IN(611, InStruct.tokens[i + 2].line, NULL);
				choise = false;
			}
			if (idtable.table[Lextable.table[i+2].idxTI].iddatatype!=IT::SHR|| idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::SHR ) {
				throw ERROR_THROW_IN(612, InStruct.tokens[i + 2].line, NULL);
				choise = false;
			}
			break;

		}
		}


	}

	if (isBrace != 0)
	{
		throw ERROR_THROW(600);
		choise = false;
	}

	if (isBack == true)
	{
		throw ERROR_THROW(126);
		choise = false;
	}
	if (isGlavn == 0)
	{
		throw ERROR_THROW(123);
		choise = false;
	}
	else if (isGlavn > 1)
	{
		throw ERROR_THROW(118);
		choise = false;
	}
	return choise;

}