# pak

*.pak* is [Quake's container file format](https://quakewiki.org/wiki/.pak#Format_specification). 

It is very simple and is an uncompressed archive format. The archive is used to store data to be loaded by a Quake game engine. That data may include graphics, maps and textures etc.

This repository provides a collection of tools that can be used to create, read and update .pak files.


## pak_editor

A Qt5-based user interface for editing PAK files.


## pak_extract

Extract a PAK file to a directory. Usage:

    pak_extract <path_to_file_to_extract.pak> <path_to_extract_assets>


## pak_make

Create a PAK file from a directory recursively. Usage:

    pak_make <path_to_file_to_create.pak> <path_containing_assets>


## pak_test

A very basic unittest-like parsing of a PAK file. Usage:

    pak_test <path_to_file.pak>