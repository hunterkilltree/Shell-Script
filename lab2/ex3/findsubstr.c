#include "findsubstr.h"

int find_sub_string(const char *str, const char *sub) {

	int i = 0, j = 0;
	int nTemp = 0;
	int nStrLen = 0;
	int nStrSubLen = 0;
	int res = -1;


	if (*sub == '\0')
		return 0;

	// stlen
	while(str[nStrLen]) {
		nStrLen++;
	}

	// strlen
	while (sub[nStrSubLen]) {
		nStrSubLen++;
	}

	for(i = 0; i < nStrLen; i++)
	{
		nTemp = i;
		for(j = 0; j < nStrSubLen; j++)
		{

			if(str[nTemp] == sub[j])
			{
				if (j == 0) res = nTemp;

				if(j == nStrSubLen - 1)
					return res;
				nTemp++;
			}
			else
				break;
		}
	}
	return -1;
}
