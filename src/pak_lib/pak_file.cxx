//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#include <fstream>
#include <pak_lib/pak_file.hxx>

namespace pak
{
    struct PAK_HEADER
    {
        char id[4];
        int32_t offset;
        int32_t size;
    };
    static_assert(sizeof(PAK_HEADER) == 12);

    struct PAK_ENTRY
    {
        char name[56];
        int32_t offset;
        int32_t size;
    };
    static_assert(sizeof(PAK_ENTRY) == 64);

    template <typename T>
    static inline void append(std::vector<uint8_t>& buffer, T const& t)
    {
        auto begin = reinterpret_cast<uint8_t const*>(&t);
        auto end = begin + sizeof(t);

        buffer.insert(buffer.end(), begin, end);
    }

    template <typename T>
    static inline void append(std::vector<uint8_t>& buffer)
    {
        T t;
        std::memset(&t, 0, sizeof(t));

        append(buffer, t);
    }

    PakEntry::PakEntry(std::string name, std::vector<uint8_t> const& contents) : mName(name), mContents(std::move(contents))
    {
        auto const fieldSize = sizeof(PAK_ENTRY::name);
        if (mName.size() > fieldSize)
        {
            auto const excess = mName.size() - fieldSize;
            mName.erase(fieldSize, excess);
        }
    }

    void PakFile::AddEntry(PakEntry const& entry)
    {
        mEntries.push_back(entry);
    }

    bool PakFile::Read(std::filesystem::path const& path)
    {
        std::vector<uint8_t> contents;
        std::ifstream in(path, std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            contents.resize(static_cast<size_t>(in.tellg()));
            in.seekg(0, std::ios::beg);
            in.read(reinterpret_cast<char*>(contents.data()), static_cast<std::streamsize>(contents.size()));
        }

        auto header = reinterpret_cast<PAK_HEADER const*>(contents.data());
        if (!header || std::memcmp(header->id, "PACK", sizeof(char) * 4))
        {
            return false;
        }

        mEntries.clear();

        std::vector<PAK_ENTRY> entries(static_cast<uint32_t>(header->size) / sizeof(PAK_ENTRY));
        if (!in.seekg(header->offset))
        {
            return false;
        }

        if (!in.read(reinterpret_cast<char*>(entries.data()), static_cast<std::streamsize>(entries.size() * sizeof(PAK_ENTRY))))
        {
            return false;
        }

        for (auto const& entry : entries)
        {
            std::vector<uint8_t> entryContents(static_cast<uint32_t>(entry.size));

            in.seekg(entry.offset);
            in.read(reinterpret_cast<char*>(entryContents.data()), static_cast<std::streamsize>(entryContents.size()));

            mEntries.emplace_back(PakEntry(entry.name, entryContents));
        }

        return true;
    }

    bool PakFile::Write(std::filesystem::path const& path)
    {
        std::vector<uint8_t> buffer;

        // Write the header.
        append<PAK_HEADER>(buffer);

        int32_t offset = sizeof(PAK_HEADER);

        // Fix-up the header.
        PAK_HEADER* header = reinterpret_cast<PAK_HEADER*>(buffer.data());
        std::memcpy(header->id, "PACK", sizeof(char) * 4);
        header->offset = offset;
        header->size = static_cast<int32_t>(sizeof(PAK_ENTRY) * mEntries.size());

        offset += sizeof(PAK_ENTRY) * mEntries.size();

        // Write the entry data.
        for (auto const& entry : mEntries)
        {
            PAK_ENTRY pakEntry;

            std::memcpy(pakEntry.name, entry.GetName().data(), sizeof(char) * sizeof(pakEntry.name));
            pakEntry.offset = offset;
            pakEntry.size = static_cast<int32_t>(entry.GetContents().size());

            append(buffer, pakEntry);

            offset += pakEntry.size;
        }

        // Write the entry contents.
        for (auto const& entry : mEntries)
        {
            auto const& entryContents = entry.GetContents();
            buffer.insert(buffer.end(), entryContents.begin(), entryContents.end());
        }

        // Write the file.
        std::ofstream out(path, std::ios::binary);
        if (out)
        {
            out.write(reinterpret_cast<char const*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
            out.close();
            return !out.bad();
        }

        return true;
    }
} // namespace pak
