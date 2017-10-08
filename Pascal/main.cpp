#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	char filename[100];
	if (argc == 1)
	{
		cout << "Enter filename to compile" << endl;
		cin >> filename;
	}
	else
	{
		char *p = argv[1];
		int fl;
		for (fl = 0; *p != '\0'; p++, fl++)
			filename[fl] = *p;
		filename[fl] = '\0';
	}

	cout << "Reading from \"" << filename << "\"" << endl;

	system("pause");
	return 0;
}