#pragma once
#ifndef DS_UTIL_FILE_NAME_PARSER_H
#define DS_UTIL_FILE_NAME_PARSER_H

#include <exception>
#include <string>
#include <vector>
#include <cinder/Vector.h>
#include <ds/util/string_util.h>


namespace ds {

/**
 * \class ds::ParseFileMetaException
 */
class ParseFileMetaException : public std::exception {
public:
	ParseFileMetaException(const std::string& metaValue);
	const char *what() const throw();

private:
	std::string mWhat;
};

/**
 * \class ds::FileMetaData
 * Collection of file meta data.
 */
class FileMetaData {
public:
	FileMetaData();

	void				parse(const std::string& filename);

	size_t				keyCount() const;
	bool				contains(const std::string& key) const;
	const std::string&	keyAt(const size_t index) const;
	const std::string&	valueAt(const size_t index) const;
	const std::string&	findValue(const std::string& key) const;
	template <typename T>
	T					findValueType(const std::string& key, const T error) const;

private:
	std::vector<std::pair<std::string, std::string>>
						mAttrib;
};

// throws ParseFileMetaException when parsing fails.
int parseFileMetaData(const std::string& filename, const std::string& metaValue);
// does not catch parseFileMetaData exception, so consider parseFileMetaDataSize throwing on failure.
ci::Vec2f parseFileMetaDataSize(const std::string& filename);

template <typename T>
T FileMetaData::findValueType(const std::string& key, const T error) const {
	const std::string&	v = findValue(key);
	if (v.empty()) return error;
	T					ans;
	if (!ds::string_to_value(v, ans)) return error;
	return ans;
}

}

#endif//DS_UTIL_FILE_NAME_PARSER_H
