//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <pak_lib/pak_file.hxx>

namespace pak
{
    static bool processFile(std::filesystem::path path)
    {
        if (!std::filesystem::exists(path))
        {
            std::cerr << "The PAK file <" << path << "> does not exist." << std::endl;
            return false;
        }

        PakFile pakFile;
        if (!pakFile.Read(path))
        {
            std::cerr << "The PAK file <" << path << "> could not be read." << std::endl;
            return false;
        }

        auto const& pakEntries = pakFile.GetEntries();
        if (pakEntries.empty())
        {
            std::cerr << "The PAK file <" << path << "> has no entries." << std::endl;
            return false;
        }

        for (auto const& pakEntry : pakEntries)
        {
            auto const& entryName = pakEntry.GetName();
            if (entryName.empty())
            {
                std::cerr << "A PAK entry in <" << path << "> has no name." << std::endl;
                return false;
            }

            auto const& entryContents = pakEntry.GetContents();
            if (entryContents.empty())
            {
                std::cerr << "The PAK entry <" << entryName << "> has no contents." << std::endl;
                return false;
            }

            std::cout << "Entry: " << entryName << ", contents size: " << entryContents.size() << "." << std::endl;
        }

        return true;
    }
} // namespace pak

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "USAGE: " << argv[0] << "<pak_file>" << std::endl;
        return EXIT_FAILURE;
    }

    return pak::processFile(argv[1]) ? EXIT_SUCCESS : EXIT_FAILURE;
}
