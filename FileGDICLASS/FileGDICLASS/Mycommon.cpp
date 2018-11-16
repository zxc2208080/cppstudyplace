#include "stdafx.h"

CHAR szFileName[128] = "Line.bin";

BOOL LoadData()
{
	FILE *fp = fopen(szFileName, "rb");
	if (fp)
	{
		fread(&gLine, sizeof(MYLINE), 1, fp);
		fclose(fp);
		return TRUE;
	}
	return TRUE;
}
BOOL SaveData()
{
	FILE *fp = fopen(szFileName, "wb");
	if (fp)
	{
		fwrite(&gLine, sizeof(MYLINE), 1, fp);
		fclose(fp);
		return TRUE;
	}

	return TRUE;
}
