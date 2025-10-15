#include <filesystem>
#include <iostream>

using namespace std;
using namespace filesystem;



int main()
{
	path main_path("/data/data/com.termux/files/home");

	const int char_limit = 30;
	
	unsigned pointPos = 0;

	path copied;

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
		
		if (in == "s" || in == "S") pointPos++;
		else if ((in == "w" || in == "W") && pointPos > 0) pointPos--;
		else if (cur.is_directory() && (in == "n" || in == "next"))
		{
			main_path += "/" + cur.path().filename().string();
			pointPos = 0;
		}
		else if ((in == "r" || in == "return") && main_path.string() != "/"){
			main_path = main_path.parent_path();
			pointPos = 0;
		}
		else if (in == "c" || in == "copy"){ 
			copied = cur.path();
		}
		else if ((in == "p" || in == "paste") && exists(copied)){
			copy(copied, main_path);
		}
		else if (in == "rm" || in == "remove"){
			cout << "do u really wanna delete this file or all files in this directory?? y/n: ";
			cin >> in;
			if (in == "y" || in == "Y"){
				remove_all(cur.path());
			}
		}
		else if (in == "ren" || in == "rename"){
			cout << "rename to..: ";
			cin >> in;
			
			rename(cur.path(), main_path.string() + "/" + in);
		}

#ifdef  _WIN32
		system("cls");
#else
		system("clear");
#endif 

	}
}	
