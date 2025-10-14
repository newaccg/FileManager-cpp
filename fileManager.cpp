
#include <filesystem>
#include <iostream>
//#include <ftxui>

using namespace std;
using namespace filesystem;



int main()
{
	path main_path("/data/data/com.termux/files/home");

	const int char_limit = 30;
	unsigned pointPos = 0;

	while (true)
	{
		directory_entry cur;
		cout << "current path: " << main_path.string() << '\n' << endl;

		unsigned i{};
		for (directory_iterator next(main_path), end; next != end; next++, i++)
		{	
			string filename = path(*next).filename().string();
			size_t size = filename.size();

			if (size > char_limit)
			{
				filename.replace(char_limit, size - char_limit, "...");
				cout << filename;
			}
			else
			{
				cout << filename;
				// + 3 cause of "..."
				for (int i = 0; i < char_limit - size + 3; i++)
				{
					cout << '-';
				}
			}

			cout << "-----";
			if ((*next).is_regular_file())
			{
				cout << (*next).file_size() << " B";
			}

			if (i == pointPos)
			{
				cout << " <--" << endl;
				cur = *next;
			}
			else cout << endl;
		}

		string in;
		cin >> in;

		if (in[0] == 's' || in[0] == 'S') pointPos++;
		else if ((in[0] == 'w' || in[0] == 'W') && pointPos > 0) pointPos--;
		else if (cur.is_directory())
		{
			main_path += "/" + path(cur).filename().string();
			pointPos = 0;
		}

#ifdef  _WIN32
		system("cls");
#else
		system("clear");
#endif 

	}
}	
