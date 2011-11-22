/*
 * File:   DictionaryParser.h
 * Author: ralf
 *
 * Created on August 10, 2011, 3:19 PM
 */

#ifndef DICTIONARYPARSER_H
#define	DICTIONARYPARSER_H

#include <set>

#include "../core/Dictionary.h"

#include "XmlParser.h"

using std::set;

class DictionaryParser {
public:
    DictionaryParser();
    virtual ~DictionaryParser();

    shared_ptr<Dictionary> parse(const string& path);

private:
    DictionaryParser(const DictionaryParser& orig);

    bool contains(const set<string>& list, const string& key) {
        return list.find(key) != list.end();
    }

    void parseVariables(const string& variableDefPath, const string& file, ProductDescriptor& prod) const;
    void parseAttributes(const string& file, const string& variableName, VariableDescriptor& var) const;
    void parseDimensions(const string& file, const string& variableName, VariableDescriptor& var) const;
    vector<string> getDirectoryNames(const string& path) const;
    vector<string> getFileNames(const string& path) const;
    int toType(const string& typeString) const;

    XmlParser xmlParser;
    set<string> exclusionSet;
};

#endif	/* DICTIONARYPARSER_H */

