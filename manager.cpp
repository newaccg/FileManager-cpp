
#include <filesystem>
#include <iostream>
#include <vector>


using namespace std;
using namespace filesystem;

string Lower(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}


int main()
{
    path main_path("//data/data/com.termux/files/home/cpp-projects-termux");

    const int char_limit = 30;

    unsigned pointPos = 0;

    path copied = "";

    string in;

    while (true)
    {
        cout << "\033[2J\033[1;1H";

        vector<path> paths{};

        directory_entry cur;
        cout << "current path: " << main_path.string() << '\n' << endl;

		while (true){
			path par(main_path.parent_path());
			try {
				 directory_iterator{main_path};
			}
			catch(filesystem_error& err){
			    cout << "cannot open this directory: " << err.what();
				 
				if (main_path == "/")
				{
					cout << "its the main directory so i gotta kill this program. Bye!";
					exit(0);
				}
				cout << "press any key to go to the parent directory";
				cin >> in;
				main_path = par;
			}
			if (main_path != par){
				for (directory_iterator next(main_path), end; next != end; next++){
					paths.push_back((*next).path());
				}
				break;
			}
		}

        for (int i = 0; i < paths.size(); i++)
        {
            directory_entry file(paths[i]);
            string filename = paths[i].filename().string();
            size_t size = filename.size();

            if (size > char_limit)
            {
                filename.replace(char_limit, size - char_limit, "...");
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

        cin >> in;

        in = Lower(in);

        if (in == "s" && pointPos != paths.size() - 1) pointPos++;

        else if (in == "w" && pointPos > 0) pointPos--;

        else if (cur.is_directory() && (in == "n" || in == "next"))
        {
            main_path += "/" + cur.path().filename().string();
            pointPos = 0;
        }

        else if ((in == "r" || in == "return") && main_path.string() != "/")
        {
            main_path = main_path.parent_path();
            pointPos = 0;
        }

        else if (in == "c" || in == "copy")
        {
            copied = paths[pointPos];
        }

        else if ((in == "p" || in == "paste") && copied != "")
        {
            string dir = main_path.string() + "/" + copied.filename().string();

            if (exists(dir))
            {
                cout << "the copied file exists here. Overwrite?? y/n: ";
                cin >> in;

                if (in == "y")
                {
                    create_directory(dir);
                    copy(copied, dir, copy_options::recursive);
                }
            }
            else
            {
                create_directory(dir);
                copy(copied, dir, copy_options::recursive);
            }
        }

        else if (in == "rm" || in == "remove")
        {
            if (cur.is_directory())
            {
                cout << "delete ALLL files at this directory?? y/n: ";
                cin >> in;

                if (in == "y")
                {
                    remove_all(cur.path());
                }
            }
            else
            {
                cout << "remove this file?? y/n: ";
                cin >> in;

                if (in == "y")
                {
                    remove(cur.path());
                }
            }
        }

        else if (in == "help")
        {
            cout << "w/s is are up and down" << endl;
            cout << "n or next to open directory" << endl;
            cout << "r or return to go to parent directory" << endl;
            cout << "rm or remove to remove file/directory" << endl;
            cout << "ren or rename to rename file/directory" << endl;
            cout << "c or copy to copy file/directory" << endl;
            cout << "p or paste to paste copied file/directory" << endl;
            cout << "e or exit to kill this program" << endl;
            cout << "any key to continue:..." << endl;

            cin >> in;
        }

        else if (in == "ren" || in == "rename")
        {
            cout << "rename to..: ";
            cin >> in;

            rename(cur.path(), main_path.string() + "/" + in);
        }

        else if (in == "e" || in == "exit") exit(0);
    }
}
