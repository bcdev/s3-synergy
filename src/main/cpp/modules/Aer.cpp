/*
 * Aer.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

#include "Aer.h"
#include "../core/TiePointInterpolator.h"
#include "../util/ErrorMetric.h"
#include "../util/LookupTableReader.h"
#include "../util/AuxdataProvider.h"

using std::copy;
using std::min;
using std::numeric_limits;
using std::set;

static void copyValarray(const std::valarray<double>& source, std::valarray<double>& target) {
	target.resize(source.size());
	copy(&source[0], &source[source.size()], &target[0]);
}

class PixelInitializer {

public:
	PixelInitializer(const Context& context);
	~PixelInitializer();
	Pixel& getPixel(long k, long l, long m, Pixel& p) const;

private:
	const Context& context;

	const Segment& averagedSegment;
	const Segment& olcInfoSegment;
	const Segment& slnInfoSegment;
	const Segment& sloInfoSegment;

	const Grid& averagedGrid;
	const Grid& olcInfoGrid;
	const Grid& slnInfoGrid;
	const Grid& sloInfoGrid;

	const Accessor& synFlags;
	const Accessor& lat;
	const Accessor& lon;

	const TiePointInterpolator<double> tiePointInterpolatorOlc;
	const TiePointInterpolator<double> tiePointInterpolatorSln;
	const TiePointInterpolator<double> tiePointInterpolatorSlo;

	vector<Accessor*> radiances;
	vector<Accessor*> solarIrradiancesOlc;
	vector<Accessor*> solarIrradiancesSln;
	vector<Accessor*> solarIrradiancesSlo;

	const valarray<double>& cO3;

	valarray<double> szaTiePointsOlc;
	valarray<double> saaTiePointsOlc;
	valarray<double> vzaTiePointsOlc;
	valarray<double> vaaTiePointsOlc;
	valarray<double> vzaTiePointsSln;
	valarray<double> vaaTiePointsSln;
	valarray<double> vzaTiePointsSlo;
	valarray<double> vaaTiePointsSlo;

	valarray<double> ozoneTiePoints;
	valarray<double> airPressureTiePoints;
	valarray<double> waterVapourTiePoints;
};

PixelInitializer::PixelInitializer(const Context& context) :
		context(context), averagedSegment(context.getSegment(Constants::SEGMENT_SYN_AVERAGED)), olcInfoSegment(context.getSegment(Constants::SEGMENT_OLC_INFO)), slnInfoSegment(
				context.getSegment(Constants::SEGMENT_SLN_INFO)), sloInfoSegment(context.getSegment(Constants::SEGMENT_SLO_INFO)), averagedGrid(averagedSegment.getGrid()), olcInfoGrid(
				olcInfoSegment.getGrid()), slnInfoGrid(slnInfoSegment.getGrid()), sloInfoGrid(sloInfoSegment.getGrid()), synFlags(averagedSegment.getAccessor("SYN_flags")), lat(
				averagedSegment.getAccessor("latitude")), lon(averagedSegment.getAccessor("longitude")), tiePointInterpolatorOlc(
				context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lat").getDoubles()), tiePointInterpolatorSln(
				context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lat").getDoubles()), tiePointInterpolatorSlo(
				context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lat").getDoubles()),
				cO3(((AuxdataProvider&) context.getObject(Constants::AUX_ID_SYRTAX)).getVectorDouble("C_O3")) {
	for (size_t b = 0; b < 30; b++) {
		radiances.push_back(&averagedSegment.getAccessor("L_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 0; b < 18; b++) {
		solarIrradiancesOlc.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 18; b < 24; b++) {
		solarIrradiancesSln.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 24; b < 30; b++) {
		solarIrradiancesSlo.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}

	const Segment& olciTiepointSegment = context.getSegment(Constants::SEGMENT_OLC_TP);
	const Segment& slnTiepointSegment = context.getSegment(Constants::SEGMENT_SLN_TP);
	const Segment& sloTiepointSegment = context.getSegment(Constants::SEGMENT_SLO_TP);

	const Accessor& szaOlc = olciTiepointSegment.getAccessor("SZA");
	const Accessor& saaOlc = olciTiepointSegment.getAccessor("SAA");
	const Accessor& vzaOlc = olciTiepointSegment.getAccessor("OLC_VZA");
	const Accessor& vaaOlc = olciTiepointSegment.getAccessor("OLC_VAA");
	const Accessor& vzaSln = slnTiepointSegment.getAccessor("SLN_VZA");
	const Accessor& vaaSln = slnTiepointSegment.getAccessor("SLN_VAA");
	const Accessor& vzaSlo = sloTiepointSegment.getAccessor("SLO_VZA");
	const Accessor& vaaSlo = sloTiepointSegment.getAccessor("SLO_VAA");


	copyValarray(szaOlc.getDoubles(), szaTiePointsOlc);
	copyValarray(saaOlc.getDoubles(), saaTiePointsOlc);
	copyValarray(vzaOlc.getDoubles(), vzaTiePointsOlc);
	copyValarray(vaaOlc.getDoubles(), vaaTiePointsOlc);
	copyValarray(vzaSln.getDoubles(), vzaTiePointsSln);
	copyValarray(vzaSlo.getDoubles(), vzaTiePointsSlo);
	copyValarray(vaaSln.getDoubles(), vaaTiePointsSln);
	copyValarray(vaaSlo.getDoubles(), vaaTiePointsSlo);

	copyValarray(olciTiepointSegment.getAccessor("ozone").getDoubles(), ozoneTiePoints);
	copyValarray(olciTiepointSegment.getAccessor("air_pressure").getDoubles(), airPressureTiePoints);

	if (olciTiepointSegment.hasVariable("water_vapour")) {
		copyValarray(olciTiepointSegment.getAccessor("water_vapour").getDoubles(), waterVapourTiePoints);
	}
}


PixelInitializer::~PixelInitializer() {
}

Pixel& PixelInitializer::getPixel(long k, long l, long m, Pixel& p) const {
	p.k = k;
	p.l = l;
	p.m = m;
	p.index = averagedGrid.getIndex(k, l, m);

	/*
	 * Radiances
	 */
	for (size_t b = 0; b < 30; b++) {
		const Accessor& radiance = *radiances[b];
		if (!radiance.isFillValue(p.index)) {
			p.radiances[b] = radiance.getDouble(p.index);
		} else {
			p.radiances[b] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	/*
	 * Solar irradiances
	 */
	for (size_t i = 0; i < 18; i++) {
		const Accessor& accessor = *solarIrradiancesOlc[i];
		if (!accessor.isFillValue(p.index)) {
			p.solarIrradiances[i] = accessor.getDouble(p.index);
		} else {
			p.solarIrradiances[i] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	for (size_t i = 0; i < 6; i++) {
		const Accessor& accessor = *solarIrradiancesSln[i];
		if (!accessor.isFillValue(p.index)) {
			p.solarIrradiances[i + 18] = accessor.getDouble(p.index);
		} else {
			p.solarIrradiances[i + 18] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	for (size_t i = 0; i < 6; i++) {
		const Accessor& accessor = *solarIrradiancesSlo[i];
		if (!accessor.isFillValue(p.index)) {
			p.solarIrradiances[i + 24] = accessor.getDouble(p.index);
		} else {
			p.solarIrradiances[i + 24] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	/*
	 * Ozone coefficients
	 */
	// TODO - remove from pixel
	for (size_t i = 0; i < 30; i++) {
		p.cO3[i] = cO3[i];
	}

	/*
	 * Flags
	 */
	p.synFlags = synFlags.getUShort(p.index);
	// set flags SYN_success, SYN_negative_curvature, SYN_too_low, and SYN_high_error to false
	p.synFlags &= 3887;

	/*
	 * Geo-location
	 */
	p.lat = lat.getDouble(p.index);
	p.lon = lon.getDouble(p.index);

	/*
	 * Tie Point data
	 */
	valarray<double> tpiWeights(1);
	valarray<size_t> tpiIndexes(1);
	tiePointInterpolatorOlc.prepare(p.lat, p.lon, tpiWeights, tpiIndexes);

	p.sza = tiePointInterpolatorOlc.interpolate(szaTiePointsOlc, tpiWeights, tpiIndexes);
	p.saa = tiePointInterpolatorOlc.interpolate(saaTiePointsOlc, tpiWeights, tpiIndexes);
	p.vzaOlc = tiePointInterpolatorOlc.interpolate(vzaTiePointsOlc, tpiWeights, tpiIndexes);
	p.vaaOlc = tiePointInterpolatorOlc.interpolate(vaaTiePointsOlc, tpiWeights, tpiIndexes);

	p.airPressure = tiePointInterpolatorOlc.interpolate(airPressureTiePoints, tpiWeights, tpiIndexes);
	p.ozone = tiePointInterpolatorOlc.interpolate(ozoneTiePoints, tpiWeights, tpiIndexes);
	if (waterVapourTiePoints.size() != 0) {
		p.waterVapour = tiePointInterpolatorOlc.interpolate(waterVapourTiePoints, tpiWeights, tpiIndexes);
	} else {
		p.waterVapour = 0.2;
	}

	tiePointInterpolatorSln.prepare(p.lat, p.lon, tpiWeights, tpiIndexes);
	p.vaaSln = tiePointInterpolatorSln.interpolate(vaaTiePointsSln, tpiWeights, tpiIndexes);
	p.vzaSln = tiePointInterpolatorSln.interpolate(vzaTiePointsSln, tpiWeights, tpiIndexes);

	tiePointInterpolatorSlo.prepare(p.lat, p.lon, tpiWeights, tpiIndexes);
	p.vaaSlo = tiePointInterpolatorSlo.interpolate(vaaTiePointsSlo, tpiWeights, tpiIndexes);
	p.vzaSlo = tiePointInterpolatorSlo.interpolate(vzaTiePointsSlo, tpiWeights, tpiIndexes);

	/*
	 * Anything else
	 */
	p.tau550 = Constants::FILL_VALUE_DOUBLE;
	p.tau550err = Constants::FILL_VALUE_DOUBLE;
	p.tau550Filtered = Constants::FILL_VALUE_DOUBLE;
	p.tau550errFiltered = Constants::FILL_VALUE_DOUBLE;
	p.alpha550 = Constants::FILL_VALUE_DOUBLE;
	p.amin = numeric_limits<short>::min();
	p.E2 = numeric_limits<double>::max();

	return p;
}

Aer::Aer() :
		BasicModule("AER"), initialNu(2), initialOmega(6), aerosolAngstromExponents(40) {
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
	readAuxdata(context);
	averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
	SegmentDescriptor& collocatedSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550_er"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("A550"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("AMIN"));
	averagedGrid = &averagedSegment->getGrid();

	em = shared_ptr<ErrorMetric>(new ErrorMetric(context));
}

void Aer::process(Context& context) {
	context.getLogging().progress("Performing aerosol retrieval...", getId());

	long firstL = context.getFirstComputableL(*averagedSegment, *this);
	long lastL = context.getLastComputableL(*averagedSegment, *this);
	if (lastL < averagedGrid->getMaxL()) {
		lastL -= 10;
	}

	valarray<Pixel> pixels(averagedGrid->getSize());
	pixels = getPixels(context, pixels);
	map<size_t, shared_ptr<Pixel> > pixelMap;
	map<size_t, shared_ptr<Pixel> > missingPixels;

	for (long l = firstL; l < lastL; l++) {
		context.getLogging().progress("Retrieving aerosol properties for line l = " + lexical_cast<string>(l), getId());
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				aer_s(pixels[index]);
			}
		}
	}

	/*
	foreach(shared_ptr<Pixel> p, pixels)
			{
				if (p->k == 0 && p->m == 0) {
					context.getLogging().debug("... for line l = " + lexical_cast<string>(p->l), getId());
				}
				aer_s(p);
				if (p->amin == 0) {
					missingPixels[p->index] = p;
				}
				pixelMap[p->index] = p;
			}

	 long N_b = 1;
	 long I = 0;

	 context.getLogging().progress("Filling missing pixels...", getId());
	 typedef pair<size_t, shared_ptr<Pixel> > Entry;
	 while (!missingPixels.empty()) {
		if (I >= 5 && I <= 12) {
			N_b++;
		}
		set<size_t> completedPixels;
		foreach(Entry entry, missingPixels)
				{
					shared_ptr<Pixel> p = entry.second;
					context.getLogging().progress("Filling pixel(index = " + lexical_cast<string>(p->index) + ")...", getId());
					double tau_550, deltaTau_500, alpha550 = 0;
					uint8_t amin = 0;
					uint32_t K = 0;
					const long k = p->k;
					const long l_prime = p->l;
					const long m_prime = p->m;
					const vector<long> iPrimeIndices = createIndices(p->l, N_b);
					const vector<long> jPrimeIndices = createIndices(p->m, N_b);
					long minIDiff = numeric_limits<long>::max();
					long minJDiff = numeric_limits<long>::max();
					foreach(long i_prime, iPrimeIndices)
							{
								foreach(long j_prime, jPrimeIndices)
										{
											if (averagedGrid->isValidPosition(k, i_prime, j_prime)) {

												const size_t pixelIndex = averagedGrid->getIndex(k, i_prime, j_prime);
												if (!contains(missingPixels, pixelIndex) || contains(completedPixels, pixelIndex)) {
													shared_ptr<Pixel> q = pixelMap[pixelIndex];

													tau_550 += q->tau550;
													deltaTau_500 += q->tau550err;
													alpha550 += q->alpha550;

													long iDiff = std::abs(i_prime - l_prime);
													if (iDiff < minIDiff) {
														minIDiff = iDiff;
													}
													long jDiff = std::abs(j_prime - m_prime);
													if (jDiff < minIDiff) {
														minIDiff = iDiff;
													}

													if (minIDiff == iDiff && minJDiff == jDiff) {
														amin = q->amin;
													}
													K++;
												}
											}
										}
							}
					if (K > 1) {
						p->tau550 = tau_550 / K;
						p->tau550err = deltaTau_500 / K;
						p->alpha550 = alpha550 / K;
						p->amin = amin;
						p->synFlags |= Constants::SY2_AEROSOL_FILLED_FLAG;
						completedPixels.insert(p->index);
					}
				}
		foreach(size_t index, completedPixels)
				{
					missingPixels.erase(index);
				}
		I++;
	}
	applyMedianFiltering(pixelMap, firstL, lastL);
	*/
	putPixels(pixels, firstL, lastL);
	context.setLastComputedL(*averagedSegment, *this, lastL);
	context.setFirstRequiredL(*averagedSegment, *this, lastL + 1);
}

valarray<Pixel>& Aer::getPixels(Context& context, valarray<Pixel>& pixels) const {
	const PixelInitializer pixelInitializer(context);
	for (long l = averagedGrid->getFirstL(); l < averagedGrid->getLastL(); l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);

				pixelInitializer.getPixel(k, l, m, pixels[index]);
			}
		}
	}
	return pixels;
}

const vector<long> Aer::createIndices(long base, long bound) const {
	vector<long> result;
	for (long index = base - bound; index <= base + bound; index++) {
		if (index != base) {
			result.push_back(index);
		}
	}
	return result;
}

void Aer::aer_s(Pixel& p) {
	const bool partlyCloudy = (p.synFlags & Constants::SY2_PARTLY_CLOUDY_FLAG) == Constants::SY2_PARTLY_CLOUDY_FLAG;
	const bool partlyWater = (p.synFlags & Constants::SY2_PARTLY_WATER_FLAG) == Constants::SY2_PARTLY_WATER_FLAG;

	if (partlyCloudy || partlyWater) {
		return;
	}

	for (size_t i = 0; i < amins.size(); i++) {
		const int16_t amin = amins[i];
		Pixel q = Pixel(p);
		q.nu[0] = initialNu[0];
		q.nu[1] = initialNu[1];
		for (size_t j = 0; j < initialOmega.size(); j++) {
			q.omega[j] = initialOmega[j];
		}
		q.tau550 = initialTau550;
		q.c1 = em->computeNdvi(q);
		q.c2 = 1.0 - q.c1;
		q.amin = amin;
		bool success = em->findMinimum(q);
		if (success && q.E2 < p.E2) {
			p.assign(q);
			p.alpha550 = aerosolAngstromExponents[amin];
			p.amin = amin;
		}
	}
	if (p.amin > 0) {
		double tau550 = p.tau550;
		if (tau550 > 0.0001) {
			double a = em->computeErrorSurfaceCurvature(p);
			if (a > 0) {
				p.tau550err = kappa * sqrt(p.E2 / a);
				if (tau550 > 0.1 && p.tau550err > 5 * tau550) {
					p.synFlags |= Constants::SY2_AEROSOL_HIGH_ERROR_FLAG;
				} else {
					p.synFlags |= Constants::SY2_AEROSOL_SUCCESS_FLAG;
				}
			} else {
				p.synFlags |= Constants::SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG;
			}
		} else {
			p.synFlags |= Constants::SY2_AEROSOL_TOO_LOW_FLAG;
		}
	}
}

void Aer::applyMedianFiltering(map<size_t, shared_ptr<Pixel> >& pixels, long firstL, long lastL) {
	valarray<double> tau550Values(9);
	valarray<double> tau550ErrValues(9);
	for (long l = firstL; l <= lastL; l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				shared_ptr<Pixel> p = pixels[index];
				size_t i = 0;
				for (long l = p->l - 1; l <= p->l + 1; l++) {
					for (long m = p->m - 1; m <= p->m + 1; m++) {
						if (averagedGrid->isValidPosition(p->k, l, m)) {
							tau550Values[i] = p->tau550;
							tau550ErrValues[i] = p->tau550err;
							i++;
						}
					}
				}
				std::nth_element(&tau550Values[0], &tau550Values[i / 2], &tau550Values[i + 1]);
				std::nth_element(&tau550ErrValues[0], &tau550ErrValues[i / 2], &tau550ErrValues[i + 1]);
				p->tau550Filtered = tau550Values[i / 2];
				p->tau550errFiltered = tau550ErrValues[i / 2];
			}
		}
	}
}

void Aer::readAuxdata(Context& context) {
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "OLC_R_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLN_R_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLO_R_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "t");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "rho_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "D");
	getLookupTable(context, "S3__SY_2_SYCPAX.nc", "weight_ang_tot");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "C_O3");

	AuxdataProvider& configurationAuxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYCPAX);
	AuxdataProvider& radiometricAuxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYRTAX);

	initialTau550 = configurationAuxdataProvider.getDouble("T550_ini");
	const valarray<int16_t>& amins = configurationAuxdataProvider.getVectorShort("AMIN");
	this->amins.resize(amins.size());
	this->amins = amins;
	initialNu = configurationAuxdataProvider.getVectorDouble("v_ini");
	initialOmega = configurationAuxdataProvider.getVectorDouble("w_ini");
	kappa = configurationAuxdataProvider.getDouble("kappa");
	aerosolAngstromExponents = radiometricAuxdataProvider.getVectorDouble("A550");
}

void Aer::putPixels(const valarray<Pixel>& pixels, long firstL, long lastL) const {
	Accessor& amin = averagedSegment->getAccessor("AMIN");
	Accessor& t550 = averagedSegment->getAccessor("T550");
	Accessor& t550err = averagedSegment->getAccessor("T550_er");
	Accessor& a550 = averagedSegment->getAccessor("A550");
	Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");

	for (long l = firstL; l < lastL; l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				const Pixel& p = pixels[index];

				amin.setUByte(p.index, p.amin);
				if (p.tau550Filtered == Constants::FILL_VALUE_DOUBLE) {
					t550.setFillValue(p.index);
				} else {
					t550.setDouble(p.index, p.tau550Filtered);
				}
				if (p.tau550errFiltered == Constants::FILL_VALUE_DOUBLE) {
					t550err.setFillValue(p.index);
				} else {
					t550err.setDouble(p.index, p.tau550errFiltered);
				}
				if (p.alpha550 == Constants::FILL_VALUE_DOUBLE) {
					a550.setFillValue(p.index);
				} else {
					a550.setDouble(p.index, p.alpha550);
				}
				synFlags.setUShort(p.index, p.synFlags);
			}
		}
	}
}

