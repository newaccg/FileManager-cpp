#include <filesystem>
#include <iostream>
#include <vector>


using namespace std;
using namespace filesystem;



int main()
{
	path main_path("/data/data/com.termux/files/home");

	const int char_limit = 30;
	
	unsigned pointPos = 0;

	path copied = "";

	while (true)
	{
		#ifdef  _WIN32
		system("cls");
 		#else
		system("clear");
		#endif
	
		vector<path> paths{};
	
		directory_entry cur;
		cout << "current path: " << main_path.string() << '\n' << endl;
		
		for (directory_iterator next(main_path), end; next != end; next++){
			paths.push_back((*next).path());
		}

		
		for (int i = 0; i < paths.size(); i++)
		{	
			directory_entry file(paths[i]);
			string filename = paths[i].filename().string();
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
			if (file.is_regular_file())
			{
				cout << file.file_size() << " B";
			}

			if (i == pointPos)
			{
				cur = file;
				cout << " <--" << endl;
			}
			else cout << endl;
		}

		string in;
		cin >> in;
		
		if ((in == "s" || in == "S") && pointPos != paths.size() - 1) pointPos++;
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
			copied = paths[pointPos];
		}
		else if ((in == "p" || in == "paste") && copied != ""){
			string dir = main_path.string() + "/" + copied.filename().string();

			create_directory(dir);
			copy(copied, dir, copy_options::recursive);
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
	}
}	
