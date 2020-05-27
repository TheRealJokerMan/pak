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
    static bool process(std::filesystem::path pakFilePath, std::filesystem::path sourceDirectory)
    {
        if (!std::filesystem::exists(sourceDirectory))
        {
            std::cerr << "The destination directory <" << sourceDirectory << "> does not exist and/or could not be created." << std::endl;
            return false;
        }

        PakFile pakFile;

        for(auto const& entry : std::filesystem::recursive_directory_iterator(sourceDirectory))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            auto entryName = std::filesystem::relative(entry, sourceDirectory).string();

            std::ifstream in(entry, std::ios::binary);
            if (in)
            {
                std::vector<uint8_t> entryContents;
                in.seekg(0, std::ios::end);
                entryContents.resize(static_cast<size_t>(in.tellg()));
                in.seekg(0, std::ios::beg);
                in.read(reinterpret_cast<char*>(entryContents.data()), static_cast<std::streamsize>(entryContents.size()));
                in.close();

                if (in.bad())
                {
                    std::cerr << "The file <" << entry.path() << "> could not be read." << std::endl;
                    return false;
                }

                PakEntry pakEntry(entryName, entryContents);
                pakFile.AddEntry(pakEntry);
            }
        }

        if (!pakFile.Write(pakFilePath))
        {
            std::cerr << "The PAK file <" << pakFilePath << "> could not be written." << std::endl;
            return false;
        }

        return true;
    }
} // namespace pak

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "USAGE: " << argv[0] << "<pak_file> <source_directory>" << std::endl;
        return EXIT_FAILURE;
    }

    return pak::process(argv[1], argv[2]) ? EXIT_SUCCESS : EXIT_FAILURE;
}
