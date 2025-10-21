
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


void myCopy(path& copied, path& to, bool toCut, path& dir)
{
    if (copied != dir && dir.string().find(copied.string()) == -1)
    {
        if (is_directory(copied))
        {
            create_directory(dir);

            copy(copied, dir, copy_options::recursive);
        }
        else
        {
            copy(copied, to, copy_options::recursive);
        }

        if (toCut)
        {
            remove_all(copied);
        }
    }
}


int main()
{
    path main_path("/data/data/com.termux/files/home/cpp-projects-termux");
    path copied = "";
    path abs_path("/");

    const int char_limit = 30;

    int pointPos = 0;

    string in;

    bool toCut = false;
    bool found = false;

    directory_entry cur;

    vector<path> paths{};

    //main cycle
    while (true)
    {
        cout << "\033[2J\033[1;1H";
        cout << "type help to see commands" << '\n' << endl;
        cout << "current path: " << main_path.string() << '\n' << endl;

        path par(main_path.parent_path());

        // if cannot open...
        try
        {
            directory_iterator{main_path};
        }
        catch (filesystem_error& err)
        {
            cout << "WARNING!!!" << endl;
            cout << "cannot open this directory: " << '\n' << err.what() << endl;

            if (main_path == abs_path)
            {
                cout << "its the main directory so i gotta kill this program. Bye!" << endl;
                return 0;
            }
            cout << "press any key to go to the parent directory... ";
            cin >> in;
            main_path = par;

            continue;
        }
        // else
        for (directory_iterator next(main_path), end; next != end; next++)
        {
            paths.push_back(next->path().generic_string());
        }

		if (pointPos > paths.size() - 1) pointPos = paths.size() - 1;
	
		

        sort(paths.begin(), paths.end(), [](const path& left, const path& right) { return is_directory(left) > is_directory(right); });

        // cycle to paths
        for (int i = 0; i < paths.size(); i++)
        {
            directory_entry file;
            try
            {
                file = directory_entry(paths[i]);
            }
            catch (const filesystem_error& fs)
            {
                cout << "cannot open this file/dir: " << paths[i];

                if (i == pointPos)
                {
                    found = false;
                    cout << " <--" << endl;
                }
                else cout << endl;

                continue;
            }
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
                found = true;
                cur = file;
                cout << " <--" << endl;
            }
            else cout << endl;
        }



        // keywords
        cin >> in;

        in = Lower(in);

        // down
        if (in == "s")
        {
            if (++pointPos >= paths.size())
            {
                pointPos = 0;
            }
        }

        // up
        else if (in == "w")
        {
            if (--pointPos < 0)
            {
                pointPos = paths.size() - 1;
            }
        }

        else if (in == "top") pointPos = 0;

        else if (in == "bot") pointPos = paths.size() - 1;

        // return
        else if ((in == "r" || in == "return") && main_path.string() != "/")
        {
            main_path = main_path.parent_path();
        }

        // paste
        else if ((in == "v" || in == "paste") && copied != "")
        {
            path dir = main_path / copied.filename();
           
            if (exists(dir))
            {
                cout << "the copied file exists here. Overwrite?? y/n: ";
                cin >> in;

                if (in == "y")
                {
                	remove(dir);
                    myCopy(copied, main_path, toCut, dir);
                }
            }
            else
            {
                myCopy(copied, main_path, toCut, dir);
            }

            in = "";
        }

        // help
        else if (in == "help")
        {
            cout << "w/s are up and down" << endl;
            cout << "top/bot are go to top/bottom of the list" << endl;
            cout << "n or next to open directory" << endl;
            cout << "r or return to go to parent directory" << endl;
            cout << "rm or remove to remove file/directory" << endl;
            cout << "ren or rename to rename file/directory" << endl;
            cout << "c or copy to copy file/directory" << endl;
            cout << "x or cut to cut file/directory" << endl;
            cout << "v or paste to paste copied file/directory" << endl;
            cout << "ex or exit to kill this program" << endl;
            cout << "f or find to find some files/directories (WIP)" << endl;
			cout << "mkdir or makedirectory to make directory lol" << endl;
			cout << "any key to continue:..." << endl;
			
            cin >> in;

            in = "";
        }

        //exit
        else if (in == "ex" || in == "exit") return 0;

        // find
        else if (in == "f" || in == "find")
        {
            cout << "enter the filename: ";

            cin >> in;

            for (recursive_directory_iterator next(main_path), end; next != end; next++)
            {
                if (next->path().filename() == in)
                {
                    cout << next->path() << ' ' << next->file_size() << 'B' << endl;
                }
            }

            cout << "any key to continue... ";
            cin >> in;

            in = "";
        }

		// make directory
        else if (in == "mkdir" || in == "makedirectory"){
        	cout << "enter the directory name: ";
        	cin >> in;

        	path dir = main_path / in;
        	if (!create_directory(dir)){
        		cout << "already have a file/dir with this name here. Delete this file/dir? y/n: ";
        		cin >> in;
        		if (in == "y"){
        			remove_all(dir);
        			create_directory(dir);
        		}
        	}

        	in = "";
        }



        // commands those need a "not bad" file
        if (found)
        {
            // next
            if (cur.is_directory() && (in == "n" || in == "next"))
            {
                main_path /= cur.path();
            }

            // copy
            else if (in == "c" || in == "copy")
            {
                toCut = false;
                copied = paths[pointPos];
            }

            // remove
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

                in = "";
            }

            // rename
            else if (in == "ren" || in == "rename")
            {
                cout << "rename to..: ";
                cin >> in;

                rename(cur.path(), main_path.string() + "/" + in);

                in = "";
            }

            // cut
            else if (in == "x" || in == "cut")
            {
                copied = paths[pointPos];
                toCut = true;
            }
        }


        

        paths.clear();
        paths.shrink_to_fit();
    }
}
