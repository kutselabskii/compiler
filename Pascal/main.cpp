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
		for (int i = 0; *p; p++, i++)
			filename[i] = *p;
	}

	cout << "Reading from \"" << filename << "\"" << endl;

	system("pause");
	return 0;
}