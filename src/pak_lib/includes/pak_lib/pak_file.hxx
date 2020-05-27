//
// Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
//

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace pak
{
    /// An entry in a PAK file.
    class PakEntry
    {
    public:
        /// Constructor.
        /// \param name The name of the entry.
        /// \param contents The contents of the entry.
        PakEntry(std::string name, std::vector<uint8_t> const& contents);

        /// Contents accessor.
        /// \return The contents of entry.
        auto const& GetContents() const
        {
            return mContents;
        }

        /// Name accessor.
        /// \return The name of the entry.
        auto const& GetName() const
        {
            return mName;
        }

    private:
        std::string mName;
        std::vector<uint8_t> mContents;
    };
    
    /// Represents a PAK file.
    class PakFile final
    {
    public:
        /// Add an entry to the file.
        /// \param entry The entry to append to the file.
        void AddEntry(PakEntry const& entry);

        /// Entries accessor.
        /// \return The entries in the PAK file.
        auto const& GetEntries() const
        {
            return mEntries;
        }
        
        /// Read a PAK file.
        /// \param path Fully qualified path of the .pak file to read.
        /// \return true on success; false otherwise.
        bool Read(std::filesystem::path const& path);

        /// rite a PAK file.
        /// \param path Fully qualified path of the .pak file to write.
        /// \return true on success; false otherwise.
        bool Write(std::filesystem::path const& path);

    private:
        std::vector<PakEntry> mEntries;
    };
} // namespace pak
