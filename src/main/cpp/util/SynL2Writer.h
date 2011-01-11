/*
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <map>

#include "../core/DefaultModule.h"

using std::map;
using std::set;

class SynL2Writer : virtual public DefaultModule {
public:
    SynL2Writer();
    ~SynL2Writer();
    void process(Context& context);
    void start(Context& context);
    void stop(Context& context);

private:

    template<class K, class V>
    bool exists(const map<K, V>& map, const K& key) const {
        return map.find(key) != map.end();
    }
    void createNcVar(const Variable& variable, const Grid& grid);
    vector<string> variablesToWrite;

    map<string, int> ncIdMap;
    map<string, vector<int> > ncIdsMap;
};

#endif	/* SYNL2WRITER_H */

