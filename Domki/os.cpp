#include "os.h"

#include <experimental/filesystem>
#include<clocale>

#include <shobjidl.h> 
#include <sstream>

pair<string, string> split_parent(const string& path)
{
	size_t found;
	found = path.find_last_of("/\\");
	return { path.substr(0, found), path.substr(found + 1) };
}

string join(const string& path, const string& subpath)
{
	if (path.size())
		return path + "\\" + subpath;
	else
		return subpath;
}

vector<string> get_all_names_within_folder(string folder, bool folder_mode, bool get_full_paths)
{
    vector<string> names;
    // std::setlocale(LC_ALL, ".1250"); nie dziala
    // std::locale::global(std::locale(".1250"));
    for (const auto & entry : std::experimental::filesystem::directory_iterator(folder))
    {
        auto path = entry.path();
        if (folder_mode ^ std::experimental::filesystem::is_directory(path))
            continue;
        if (!get_full_paths)
            path = path.filename();
        names.push_back(path.generic_string());
    }
    return names;
}

string next_folder_within_folder(string path)
{
	auto dir_name = split_parent(path);
	auto all_files = get_all_names_within_folder(dir_name.first, true);
	auto pos = find(all_files.begin(), all_files.end(), dir_name.second);
	pos++;
	if (pos == all_files.end())
		pos = all_files.begin();
	return *pos;
}

string open_file_from_folder(string folder) {

    string sciezka = "";

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        wstring ws(pszFilePath);
                        sciezka = string(ws.begin(), ws.end());
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }


    return sciezka;
}

string open_folder_from_folder(string folder) {

    string sciezka = "";

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        wstring ws(pszFilePath);
                        sciezka = string(ws.begin(), ws.end());
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }


    return sciezka;
}
