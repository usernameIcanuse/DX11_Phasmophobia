#include"LoadSave.h"


bool	CMemory::Loading()
{
	int		iNum;
	while (1)
	{
		system("cls");
		cout << "=========================================\n";
		FILE*	fp[3] = { nullptr,nullptr, nullptr };
		errno_t	err[3];

		err[0] = fopen_s(fp, "../TextRPG/Save1.txt", "rb");
		err[1] = fopen_s(fp + 1, "../TextRPG/Save2.txt", "rb");
		err[2] = fopen_s(fp + 2, "../TextRPG/Save3.txt", "rb");

		cout << "File 1 : ";
		if (0 == err[0])
		{
			cout << "Save 1 \n";
		}
		else
			cout << "Empty\n";

		cout << "File 2 : ";
		if (0 == err[1])
		{
			cout << "Save 2 \n";
		}
		else
			cout << "Empty\n";

		cout << "File 3 : ";
		if (0 == err[2])
		{
			cout << "Save 3 \n";
		}
		else
			cout << "Empty\n";
		cout << "=========================================\n";

		cout << "불러올 파일을 입력하세요(4. 뒤로가기) : ";
		cin >> iNum;

		if (4 == iNum)
		{
			for (int i = 0; i < 3; ++i)
				if(0 == err[i])
					fclose(*(fp+i));
			return true;
		}
		else if (1 > iNum && 3 < iNum) continue;
		

		dynamic_cast<CPlayer*>(m_pPlayer)->Loading(fp[iNum-1]);

		for (int i = 0; i < 3; ++i)
			if (0 == err[i])
				fclose(*(fp + i));
		return false;

	}
}
void	CMemory::Saving() 
{
	int iNum;
	while (1)
	{
		system("cls");
		FILE*	fp = nullptr;
		errno_t		err;
		cout << "=========================================\n";
		cout << "1. Save File 1\t2. Save File 2\t3. Save File 3\n";

		cout << "=========================================\n";

		cout << "저장할 파일을 입력하세요(4. 뒤로가기) : ";
		cin >> iNum;

		switch (iNum)
		{
		case 1:
			err = fopen_s(&fp, "../TextRPG/Save1.txt", "wb");
			break;

		case 2:
			err = fopen_s(&fp, "../TextRPG/Save2.txt", "wb");
			break;

		case 3:
			err = fopen_s(&fp, "../TextRPG/Save3.txt", "wb");
			break;

		case 4:
			continue;

		default:
			break;

		}
		
		if (0 == err)
		{

			dynamic_cast<CPlayer*>(m_pPlayer)->Saving(fp);
			fclose(fp);
			
			return;
		}

		else
		{
			cout << "저장하지 못했습니다...\n";
			system("pause");
			return;
		}

	}
	
}