#include "FileCache.hpp"

#include "common/Utils.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/fs/Resources.hpp"

#include <boost/property_tree/xml_parser.hpp>

using ptree = boost::property_tree::ptree;

void FileCache::loadFrom(const FilePath& cacheFile)
{
    cacheFile_ = cacheFile;
    loadFileHashes(cacheFile);
}

void FileCache::loadFileHashes(const FilePath& path)
{
    if (FileSystem::exists(path))
    {
        boost::property_tree::read_xml(path, fileCache_);
    }
}

// TODO: strong type
bool FileCache::valid(const std::string& filename) const
{
    auto node = fileCache_.get_child_optional(ptree::path_type(filename, '|'));

    return node.has_value() && node->get<bool>(ptree::path_type("valid", '|'));
}

// TODO: strong type
bool FileCache::cached(const std::string& filename, const std::string& hash) const
{
    auto node = fileCache_.get_child_optional(ptree::path_type(filename, '|'));

    return node.has_value() && node->get<std::string>(ptree::path_type("md5", '|')) == hash;
}

// TODO: strong type
void FileCache::save(const std::string& fileName, const std::string& fileContent, const std::string& md5)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;

    addToCache(fileName, Utils::md5hash(fileContent), md5);
}

// TODO: strong type
void FileCache::markAsInvalid(const std::string& filename)
{
    auto node = fileCache_.get_child_optional(ptree::path_type(filename, '|'));

    if (node)
    {
        node->put(ptree::path_type("valid", '|'), false);
    }
}

// TODO: strong type
void FileCache::addToCache(const std::string& filename, const std::string& hash, const std::string& target)
{
    ptree node;
    node.put(ptree::path_type("md5", '|'), hash);
    node.put(ptree::path_type("valid", '|'), hash == target);

    fileCache_.put_child(ptree::path_type(filename, '|'), node);

    saveFileHashes(cacheFile_);
}

void FileCache::saveFileHashes(const FilePath& path)
{
    boost::property_tree::write_xml(path, fileCache_);
}
