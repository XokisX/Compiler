#include "stdafx.h"
#include "Generation.h"

ofstream fout("Gen.html");
void Head()
{
	fout << "<html>" << endl;
	fout << "<head>" << endl;
	fout << "<title>" << endl;
	fout << "EPA-2019" << endl;
	fout << "</title" << endl;
	fout << "<body>" << endl;
	fout << "<script>" << endl << endl;
}
void Down()
{
	fout << "</script>" << endl;
	fout << "</body>" << endl;
	fout << "</head>" << endl;
	fout << "</html>" << endl;
	fout.close();
}



void Generation(LT::LexTable &Lextable, In::IN &InStruct, IT::IdTable &idtable)
{
	Head();
	for (int i = 0; i < Lextable.size; i++)
	{
		switch (Lextable.table[i].lexema)
		{
		case LEX_PLUS:
		case LEX_STAR:
		case LEX_MINUS:
		case LEX_DIRSLASH:
		case  LEX_EQUAL:
		{
			fout << " " << Lextable.table[i].lexema << " ";
			break;
		}
		case LEX_COMMA:

		case LEX_RIGHTTHESIS:
		case LEX_LEFTHESIS:
		{
			fout << Lextable.table[i].lexema;
			break;
		}
		case LEX_LEFTBRACE:
		{

			fout << endl << Lextable.table[i].lexema << endl;
			break;
		}
		case LEX_BRACELET:
		{

			fout << Lextable.table[i].lexema << endl << endl;
			break;
		}
		case LEX_SEPARATOR:
		{
				if (Lextable.table[i - 1].lexema != LEX_STANDART) {
					fout << Lextable.table[i].lexema << endl;
				}
	
			break;
		}
		case  LEX_DEF:
		{
			fout << "var ";
			break;
		}
		case LEX_LITERAL:
		case LEX_ID:
		{
			if (Lextable.table[i-2].lexema==LEX_STRLEN) {
				fout << InStruct.tokens[i].token<< ".length";
			}
			else {
				if (idtable.table[i].idtype==IT::F) {
					fout << InStruct.tokens[i].token;
				}
				else {
					bool flagMain = false;
					for (int k = i; k > 0; k--) {
						if (Lextable.table[k].lexema == LEX_GLAVN) {
							flagMain = true;
						}
						if (Lextable.table[k].lexema == LEX_FUNCTION) {
							break;
						}
					}
					if (Lextable.table[i - 1].lexema == LEX_BACK && flagMain) {
					
					}
					else {
						if (Lextable.table[i-2].lexema != LEX_CMP || Lextable.table[i - 2].lexema != LEX_CMP) {
							fout << InStruct.tokens[i].token;
						}
					}
					
				}
			}
			
			break;
		}
		case LEX_OUT:
		{
			fout << "document.writeln";
			break;
		}
		case LEX_WHILE: {
			fout << "while";
			break;
		}
		case LEX_LESS: {
			fout << InStruct.tokens[i].token;
			break;
		}
		case LEX_LAGRER: {
			fout << InStruct.tokens[i].token;
			break;
		}
		case LEX_LAE: {

			fout << "<=";
			break; }
		case LEX_MAE: {

			fout << ">=";
			break; }
		case LEX_NOT_EQUALS: {

			fout << "!=";
			break; }
		case LEX_RAVN: {

			fout << "==";
			break;
		}

		case LEX_BACK: {
			bool flagMain = false;
			for (int k = i; k > 0; k--) {
				if (Lextable.table[k].lexema == LEX_GLAVN) {
					flagMain = true;
				}
				if (Lextable.table[k].lexema==LEX_FUNCTION) {
					break;
				}
			}
			if (!flagMain) {
				fout << "return ";
			}
			break;

		}
		case LEX_FUNCTION: {
			fout << "function ";
			break; 
		}
		case LEX_IF: {
			fout << InStruct.tokens[i].token;
			break;
		}
		case LEX_CMP: {
			fout << InStruct.tokens[i+2].token<<".localeCompare("<< InStruct.tokens[i + 4].token << ")";
			i += 5;
			break;
		}
		}
	
	}
	Down();

}
