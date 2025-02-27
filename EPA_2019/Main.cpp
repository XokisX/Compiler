#include "stdafx.h"
#include "Generation.h"
#include "SA.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	bool sintaksis = false;
	bool semantika = false;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);					// ��������� ����������		
		log = Log::getlog(parm.log);									// �������� log				
		Log::LOG out = Log::INITLOG;									// ������������� log �����	
		out = Log::getlog(parm.out);									// ������� out �����		
		Log::writeLog(log);												// ������ ���������			
		Log::writeParm(log, parm);										// ������ ����������		
		In::IN in = In::getin(parm.in, parm.out);						// ��������� �������� ����������
		Log::writeIn(log, in);											// ������ �������� ����������	
		LeX::LEX tables = LeX::INITLEX(in, log);				     	// ��������� ������� ������		
		Log::writeAllTokens(log, in);									// ������ ���� ����			
		Log::writeLexicalTable(log, tables.Lextable);	             	// ������ ������� ����.		
		Log::writeIDtable(log, tables.IDtable, in, tables.Lextable);	                     	// ������ ������� ������.	
		Log::writeIntermediateCode(log, tables.Lextable);               // ������ �������������� ����
		semantika = SemanticAnalyze(tables.Lextable, in, tables.IDtable, log); //������ �������������� �������
		if (semantika) {
			cout << "work!" << endl;
		}
		if (semantika == true)
		{
			MFST::Mfst mfst(tables, GRB::getGreibach());                    // ������������� ��� ��������������� �����������
			MFST_TRACE_START(log);                                        // �����
			sintaksis = mfst.start(log);                                    // ����� ��������������� �������
			if (sintaksis == false)
				throw ERROR_THROW(8);
			mfst.savededucation();                                          // ��������� ������� ������
			mfst.printrules(log);                                           // ������ ������ �������
		}
		if (sintaksis == true)
		{
			Generation(tables.Lextable, in, tables.IDtable);         // ���������� ����
			PN Polish;                                                      // ���������� ���������� Polish �����.���� PN
			if (Polish.CreatePolishNotation(&tables))                       // ���������� �������� �������
			{
				*log.stream << "\n-------�������� ������ ���������!-------------------";
				Log::writeIntermediateCode(log, tables.Lextable);
			}
		}
		Log::Close(log);												// �������� log �����		
		Log::Close(out);												// �������� out �����

		cout << "Finish!\n";

	}
	catch (Error::ERROR e)
	{
		cout << e.id << "," << e.message << endl;
		Log::writeError(log, e);                                        // ������� ������ � ������ �� ���������
	}
	system("start START.bat");
	system("pause");
	return 0;
}