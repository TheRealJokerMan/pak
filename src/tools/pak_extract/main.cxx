//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pak_lib/pak_file.hxx>

namespace pak
{
    static bool processPak(std::filesystem::path pakFilePath, std::filesystem::path destinationDirectory)
    {
        if (!std::filesystem::exists(pakFilePath))
        {
            std::cerr << "The PAK file <" << pakFilePath << "> does not exist." << std::endl;
            return false;
        }

        if (!std::filesystem::exists(destinationDirectory))
        {
            if (!std::filesystem::create_directories(destinationDirectory))
            {
                std::cerr << "The destination directory <" << destinationDirectory << "> does not exist and/or could not be created." << std::endl;
                return false;
            }
        }

        PakFile pakFile;
        if (!pakFile.Read(pakFilePath))
        {
            std::cerr << "The PAK file <" << pakFilePath << "> could not be read." << std::endl;
            return false;
        }

        for (auto const& entry : pakFile.GetEntries())
        {
            std::filesystem::path entryPath = destinationDirectory / entry.GetName();
            entryPath.make_preferred();

            std::filesystem::path entryDirPath = entryPath.parent_path();

            if (!std::filesystem::exists(entryDirPath))
            {
                if (!std::filesystem::create_directories(entryDirPath))
                {
                    std::cerr << "The destination directory <" << entryDirPath << "> does not exist and/or could not be created." << std::endl;
                    return false;
                }
            }

            std::ofstream out(entryPath, std::ios::binary);
            if (out)
            {
                auto const& contents = entry.GetContents();
                out.write(reinterpret_cast<char const*>(contents.data()), static_cast<std::streamsize>(contents.size()));
                out.close();

                if (out.bad())
                {
                    std::cerr << "The PAK entry <" << entry.GetName() << "> could not be created." << std::endl;
                    return false;
                }
            }
        }

        return true;
    }
} // namespace pak

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "USAGE: " << argv[0] << "<pak_file> <extraction_directory>" << std::endl;
        return EXIT_FAILURE;
    }

    return pak::processPak(argv[1], argv[2]) ? EXIT_SUCCESS : EXIT_FAILURE;
}
