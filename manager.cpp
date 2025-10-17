
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


void myCopy(path& copied, path& to, bool toCut){
	string dir = to.string() + "/" + copied.filename().string();

	if (is_directory(copied)){
		create_directory(dir);
	}
	
	copy(copied, dir, copy_options::recursive);
	
	if (toCut){
	    remove_all(copied);
	}
}


int main()
{
    path main_path("//data/data/com.termux/files/home/cpp-projects-termux");

    const int char_limit = 30;

    int pointPos = 0;

    path copied = "";

    string in;

    bool toCut = false;

    vector<path> paths{};
    
	//main cycle
    while (true)
    {  
        cout << "\033[2J\033[1;1H";
		
        directory_entry cur;
        
        cout << "type help to see commands" << '\n' << endl;
        cout << "current path: " << main_path.string() << '\n' << endl;

		while (true){
			path par(main_path.parent_path());

			// if cannot open...
			try {
				 directory_iterator{main_path};
			}
			
			catch(filesystem_error& err){
				cout << "WARNING!!!" << endl;
			    cout << "cannot open this directory: " << '\n' << err.what() << endl;
				 
				if (main_path == "/")
				{
					cout << "its the main directory so i gotta kill this program. Bye!" << endl;
					exit(0);
				}
				cout << "press any key to go to the parent directory... ";
				cin >> in;
				main_path = par;
			}
			// else
			if (main_path != par){
				for (directory_iterator next(main_path), end; next != end; next++){
					paths.push_back(next -> path());
				}
				break;
			}
		}

		// cycle to paths
		sort(paths.begin(), paths.end(), [](const path& left, const path& right){return is_directory(left) > is_directory(right);});
		
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


		// keywords
        cin >> in;

        in = Lower(in);

        if (in == "s"){
        	if (++pointPos >= paths.size()){
        		pointPos = 0;
        	}
        }

        else if (in == "w") {
        	if (--pointPos < 0){
        		pointPos = paths.size() - 1;
        	}
        }

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
        	toCut = false;
            copied = paths[pointPos];
        }

        else if ((in == "v" || in == "paste") && copied != "")
        {
            string dir = main_path.string() + "/" + copied.filename().string();

            if (exists(dir))
            {
                cout << "the copied file exists here. Overwrite?? y/n: ";
                cin >> in;

                if (in == "y")
                {
                	myCopy(copied, main_path, toCut);
                }
            }
            else
            {
                myCopy(copied, main_path, toCut);
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

        else if (in == "x" || in == "cut"){
        	copied = paths[pointPos];
        	toCut = true;
        }

        else if (in == "help")
        {
            cout << "w/s are up and down" << endl;
            cout << "n or next to open directory" << endl;
            cout << "r or return to go to parent directory" << endl;
            cout << "rm or remove to remove file/directory" << endl;
            cout << "ren or rename to rename file/directory" << endl;
            cout << "c or copy to copy file/directory" << endl;
			cout << "x or cut to cut file/directory" << endl;
            cout << "v or paste to paste copied file/directory" << endl;
            cout << "ex or exit to kill this program" << endl;
            cout << "any key to continue:..." << endl;
			
            cin >> in;
        }

        else if (in == "ren" || in == "rename")
        {
            cout << "rename to..: ";
            cin >> in;

            rename(cur.path(), main_path.string() + "/" + in);
        }

        else if (in == "ex" || in == "exit") exit(0);

		else if (in == "f" || in == "find"){
			cout << "enter the filename: ";

			cin >> in;

			for (recursive_directory_iterator next(main_path), end; next != end; next++){
				if (next -> path().filename() == in){
					cout << next -> path() << ' ' << next -> file_size() << 'B' << endl; 
				}
			}

			cout << "any key to continue... ";
			cin >> in;
		}

	
		paths.clear();
		paths.shrink_to_fit();
    }
}
