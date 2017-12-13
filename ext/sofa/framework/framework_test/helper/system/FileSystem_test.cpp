
#include <sofa/helper/system/FileSystem.h>
#include <gtest/gtest.h>
#include <exception>
#include <algorithm>

using sofa::helper::system::FileSystem;

static std::string getPath(std::string s) {
    return std::string(FRAMEWORK_TEST_RESOURCES_DIR) + std::string("/") + s;
}

// Mmmh, we can't have empty directories in git...
// TEST(FileSystemTest, listDirectory_empty)
// {
//     std::vector<std::string> fileList;
//     FileSystem::listDirectory(getPath("empty-directory"), fileList);
// 	EXPECT_TRUE(fileList.empty());
// }

TEST(FileSystemTest, listDirectory_nonEmpty)
{
    std::vector<std::string> fileList;
    FileSystem::listDirectory(getPath("non-empty-directory"), fileList);
    // Workaround: svn adds a '.svn' directory in each subdirectory
    if (std::find(fileList.begin(), fileList.end(), ".svn") != fileList.end())
        EXPECT_EQ(fileList.size(), 4u);
    else
        EXPECT_EQ(fileList.size(), 3u);
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileA.txt") != fileList.end());
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileB.txt") != fileList.end());
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileC.so") != fileList.end());
}

TEST(FileSystemTest, listDirectory_nonEmpty_trailingSlash)
{
    std::vector<std::string> fileList;
    FileSystem::listDirectory(getPath("non-empty-directory/"), fileList);
    // Workaround: svn adds a '.svn' directory in each subdirectory
    if (std::find(fileList.begin(), fileList.end(), ".svn") != fileList.end())
        EXPECT_EQ(fileList.size(), 4u);
    else
        EXPECT_EQ(fileList.size(), 3u);
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileA.txt") != fileList.end());
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileB.txt") != fileList.end());
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileC.so") != fileList.end());
}

TEST(FileSystemTest, listDirectory_withExtension_multipleMatches)
{
    std::vector<std::string> fileList;
    FileSystem::listDirectory(getPath("non-empty-directory/"), fileList, "txt");
    EXPECT_EQ(fileList.size(), 2u);
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileA.txt") != fileList.end());
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileB.txt") != fileList.end());
}

TEST(FileSystemTest, listDirectory_withExtension_oneMatch)
{
    std::vector<std::string> fileList;
    FileSystem::listDirectory(getPath("non-empty-directory/"), fileList, "so");
    EXPECT_EQ(fileList.size(), 1u);
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "fileC.so") != fileList.end());
}

TEST(FileSystemTest, listDirectory_withExtension_noMatch)
{
    std::vector<std::string> fileList;
    FileSystem::listDirectory(getPath("non-empty-directory/"), fileList, "h");
    EXPECT_TRUE(fileList.empty());
}

TEST(FileSystemTest, createDirectory)
{
    EXPECT_FALSE(FileSystem::createDirectory("createDirectoryTestDir"));
    EXPECT_TRUE(FileSystem::exists("createDirectoryTestDir"));
    EXPECT_TRUE(FileSystem::isDirectory("createDirectoryTestDir"));
    // Cleanup
    FileSystem::removeDirectory("createDirectoryTestDir");
}

TEST(FileSystemTest, createDirectory_alreadyExists)
{
    FileSystem::createDirectory("createDirectoryTestDir");
    EXPECT_TRUE(FileSystem::createDirectory("createDirectoryTestDir"));
    // Cleanup
    FileSystem::removeDirectory("createDirectoryTestDir");
}

TEST(FileSystemTest, removeDirectory)
{
    FileSystem::createDirectory("removeDirectoryTestDir");
    EXPECT_FALSE(FileSystem::removeDirectory("removeDirectoryTestDir"));
    EXPECT_FALSE(FileSystem::exists("removeDirectoryTestDir"));
}

TEST(FileSystemTest, removeDirectory_doesNotExists)
{
    EXPECT_TRUE(FileSystem::removeDirectory("removeDirectoryTestDir"));
    EXPECT_FALSE(FileSystem::exists("removeDirectoryTestDir"));
}

TEST(FileSystemTest, exists_yes)
{
    EXPECT_TRUE(FileSystem::exists(getPath("non-empty-directory/fileA.txt")));
}

TEST(FileSystemTest, exists_yes_directory)
{
    EXPECT_TRUE(FileSystem::exists(getPath("non-empty-directory")));
}

TEST(FileSystemTest, exists_yes_directory_trailingSlash)
{
    EXPECT_TRUE(FileSystem::exists(getPath("non-empty-directory/")));
}

TEST(FileSystemTest, exists_nope)
{
    EXPECT_FALSE(FileSystem::exists(getPath("thisFileDoesNotExist.txt")));
}

TEST(FileSystemTest, isDirectory_yes)
{
    EXPECT_TRUE(FileSystem::isDirectory(getPath("non-empty-directory")));
}

TEST(FileSystemTest, isDirectory_yes_trailingSlash)
{
    EXPECT_TRUE(FileSystem::isDirectory(getPath("non-empty-directory/")));
}

TEST(FileSystemTest, isDirectory_nope)
{
    EXPECT_FALSE(FileSystem::isDirectory(getPath("non-empty-directory/fileA.txt")));
}

TEST(FileSystemTest, isAbsolute)
{
    EXPECT_FALSE(FileSystem::isAbsolute(""));
    EXPECT_FALSE(FileSystem::isAbsolute("abc"));
    EXPECT_FALSE(FileSystem::isAbsolute("abc/def"));
    EXPECT_TRUE(FileSystem::isAbsolute("/"));
    EXPECT_TRUE(FileSystem::isAbsolute("/abc"));
    EXPECT_TRUE(FileSystem::isAbsolute("/abc/"));
    EXPECT_TRUE(FileSystem::isAbsolute("/abc/def"));
    EXPECT_TRUE(FileSystem::isAbsolute("A:/"));
    EXPECT_TRUE(FileSystem::isAbsolute("B:/abc"));
    EXPECT_TRUE(FileSystem::isAbsolute("C:/abc/"));
    EXPECT_TRUE(FileSystem::isAbsolute("D:/abc/def"));
}

TEST(FileSystemTest, cleanPath)
{
    EXPECT_EQ("", FileSystem::cleanPath(""));
    EXPECT_EQ("/abc/def/ghi/jkl/mno", FileSystem::cleanPath("/abc/def//ghi/jkl///mno"));
    EXPECT_EQ("C:/abc/def/ghi/jkl/mno", FileSystem::cleanPath("C:\\abc\\def\\ghi/jkl///mno"));
}

TEST(FileSystemTest, convertBackSlashesToSlashes)
{
    EXPECT_EQ("", FileSystem::convertBackSlashesToSlashes(""));
    EXPECT_EQ("abc/def/ghi", FileSystem::convertBackSlashesToSlashes("abc/def/ghi"));
    EXPECT_EQ("abc/def/ghi", FileSystem::convertBackSlashesToSlashes("abc/def\\ghi"));
    EXPECT_EQ("abc/def/ghi", FileSystem::convertBackSlashesToSlashes("abc\\def\\ghi"));
    EXPECT_EQ("C:/abc/def/ghi", FileSystem::convertBackSlashesToSlashes("C:\\abc\\def\\ghi"));
    EXPECT_EQ("C:/abc/def/ghi", FileSystem::convertBackSlashesToSlashes("C:\\abc\\def/ghi"));
}

TEST(FileSystemTest, removeExtraSlashes)
{
    EXPECT_EQ("", FileSystem::removeExtraSlashes(""));
    EXPECT_EQ("/", FileSystem::removeExtraSlashes("/"));
    EXPECT_EQ("/", FileSystem::removeExtraSlashes("//"));
    EXPECT_EQ("/", FileSystem::removeExtraSlashes("///"));
    EXPECT_EQ("/", FileSystem::removeExtraSlashes("////"));
    EXPECT_EQ("/", FileSystem::removeExtraSlashes("/////"));
    EXPECT_EQ("/", FileSystem::removeExtraSlashes("//////"));
    EXPECT_EQ("/abc/def", FileSystem::removeExtraSlashes("/abc/def"));
    EXPECT_EQ("/abc/def/", FileSystem::removeExtraSlashes("/abc/def/"));
    EXPECT_EQ("/abc/def/ghi/jkl/", FileSystem::removeExtraSlashes("/abc//def//ghi/jkl///"));
}

TEST(FileSystemTest, getParentDirectory)
{
    EXPECT_EQ("/abc/def", FileSystem::getParentDirectory("/abc/def/ghi"));
    EXPECT_EQ("/abc/def", FileSystem::getParentDirectory("/abc/def/ghi/"));
    EXPECT_EQ("/", FileSystem::getParentDirectory("/abc/"));
    EXPECT_EQ("/", FileSystem::getParentDirectory("/abc"));
    EXPECT_EQ("/", FileSystem::getParentDirectory("/"));
    EXPECT_EQ(".", FileSystem::getParentDirectory("."));
    EXPECT_EQ(".", FileSystem::getParentDirectory(""));

    EXPECT_EQ("abc/def", FileSystem::getParentDirectory("abc/def/ghi"));
    EXPECT_EQ("abc/def", FileSystem::getParentDirectory("abc/def/ghi/"));
    EXPECT_EQ("abc/def", FileSystem::getParentDirectory("abc/def/ghi//"));
    EXPECT_EQ("abc/def", FileSystem::getParentDirectory("abc/def/ghi///"));
    EXPECT_EQ("abc/def", FileSystem::getParentDirectory("abc/def/ghi////"));
    EXPECT_EQ("abc", FileSystem::getParentDirectory("abc/def"));
    EXPECT_EQ(".", FileSystem::getParentDirectory("abc"));

    EXPECT_EQ("C:/abc/def", FileSystem::getParentDirectory("C:/abc/def/ghi"));
    EXPECT_EQ("C:/abc/def", FileSystem::getParentDirectory("C:/abc/def/ghi/"));
    EXPECT_EQ("C:/", FileSystem::getParentDirectory("C:/abc/"));
    EXPECT_EQ("C:/", FileSystem::getParentDirectory("C:/abc"));
    EXPECT_EQ("C:/", FileSystem::getParentDirectory("C:///"));
    EXPECT_EQ("C:/", FileSystem::getParentDirectory("C://"));
    EXPECT_EQ("C:/", FileSystem::getParentDirectory("C:/"));
}

TEST(FileSystemTest, stripDirectory)
{
    EXPECT_EQ("", FileSystem::stripDirectory(""));
    EXPECT_EQ("/", FileSystem::stripDirectory("/"));
    EXPECT_EQ("/", FileSystem::stripDirectory("C:/"));
    EXPECT_EQ("abc", FileSystem::stripDirectory("abc"));
    EXPECT_EQ("abc", FileSystem::stripDirectory("/abc"));
    EXPECT_EQ("abc", FileSystem::stripDirectory("/abc/"));
    EXPECT_EQ("abc", FileSystem::stripDirectory("C:/abc"));
    EXPECT_EQ("abc", FileSystem::stripDirectory("C:/abc/"));
    EXPECT_EQ("def", FileSystem::stripDirectory("abc/def"));
    EXPECT_EQ("def", FileSystem::stripDirectory("/abc/def"));
    EXPECT_EQ("def", FileSystem::stripDirectory("/abc/def/"));
    EXPECT_EQ("def", FileSystem::stripDirectory("C:/abc/def"));
    EXPECT_EQ("def", FileSystem::stripDirectory("C:/abc/def/"));
    EXPECT_EQ("ghi", FileSystem::stripDirectory("/abc/def/ghi"));
    EXPECT_EQ("ghi", FileSystem::stripDirectory("/abc/def/ghi/"));
    EXPECT_EQ("ghi", FileSystem::stripDirectory("abc/def/ghi"));
    EXPECT_EQ("ghi", FileSystem::stripDirectory("abc/def/ghi/"));
    EXPECT_EQ("ghi", FileSystem::stripDirectory("C:/abc/def/ghi"));
    EXPECT_EQ("ghi", FileSystem::stripDirectory("C:/abc/def/ghi/"));
}
