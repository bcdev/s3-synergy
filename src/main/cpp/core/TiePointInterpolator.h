/*
 * TiePointInterpolator.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef TIEPOINTINTERPOLATOR_H_
#define TIEPOINTINTERPOLATOR_H_

#include <algorithm>
#include <cmath>
#include <valarray>
#include <vector>

using std::copy;
using std::lower_bound;
using std::sort;
using std::valarray;
using std::vector;

template<class W>
class TiePointInterpolator {
public:
	TiePointInterpolator(const valarray<W>& lons, const valarray<W>& lats,
			const vector<valarray<W> >& fields);
	~TiePointInterpolator();

	void findIndexes(W lon, W lat, valarray<size_t>& indexes) const;

private:
	class Cmp {
	public:
		Cmp(const valarray<W>& lons, const valarray<W>& lats) :
				lons(lons), lats(lats) {
		}

		~Cmp() {
		}

		bool operator()(const size_t i1, const size_t i2) const {
			return lats[i1] < lats[i2];
		}
	private:
		const valarray<W>& lons;
		const valarray<W>& lats;

	};
	void reorder(valarray<W>& array, const valarray<size_t>& reordering) const;
	W greatCircleDistance(W lon, W lat, size_t i) const;

	valarray<W> lons;
	valarray<W> lats;
	valarray<valarray<W> > fields;

	static const W RAD = W(3.14159265358979323846) / W(180.0);
};

template<class W>
TiePointInterpolator<W>::TiePointInterpolator(const valarray<W>& lons,
		const valarray<W>& lats, const vector<valarray<W> >& fields) :
		lons(lons), lats(lats), fields(fields) {
	valarray<size_t> indexes(lats.size());
	for (size_t i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}
	sort(&indexes[0], &indexes[indexes.size()], Cmp(lons, lats));
	reorder(lons, indexes);
	reorder(lats, indexes);
	for (size_t i = 0; i < fields.size(); i++) {
		reorder(fields[i], indexes);
	}
}

template<class W>
TiePointInterpolator<W>::~TiePointInterpolator() {
}

template<class W>
void TiePointInterpolator<W>::reorder(valarray<W>& array,
		const valarray<size_t>& reordering) const {
	const valarray<W> reorderedArray = array[reordering];
	copy(&reorderedArray[0], &reorderedArray[reorderedArray.size()], &array[0]);
}

template<class W>
void TiePointInterpolator<W>::findIndexes(W lon, W lat,
		valarray<size_t>& indexes) const {

	const size_t foundIndex = lower_bound(&lats[0], &lats[lats.size()], lat)
			- &lats[0];
	const size_t firstIndex = foundIndex >= 100 ? foundIndex - 100 : 0;
	const size_t lastIndex =
			firstIndex + 200 <= lat.size() ? firstIndex + 200 : lat.size();

	valarray<W> distances(W(-1), 4);

	for (size_t i = firstIndex; i < lastIndex; i++) {
		const W d = greatCircleDistance(lon, lat, i);
		for (size_t k = 0; k < 4; k++) {
			if (d > distances[k]) {
				for (size_t l = 3; l > k; l--) {
					distances[l] = distances[l - 1];
					indexes[l] = indexes[l - 1];
				}
				distances[k] = d;
				indexes[k] = i;
			}
		}
	}
}

template<class W>
inline W TiePointInterpolator<W>::greatCircleDistance(W lon, W lat,
		size_t i) const {
	using std::abs;
	using std::cos;
	using std::sin;

	return abs(
			sin(lat * RAD) * sin(lats[i] * RAD)
					+ cos(lon * RAD) * cos(lons[i] * RAD)
							* cos((lon - lons[i]) * RAD));
}

#endif /* TIEPOINTINTERPOLATOR_H_ */
