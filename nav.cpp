#include "nav.h"
#include "tbutil.h"
#include <math.h>

namespace nav {
	float distance_between(float lat1, float lon1, float lat2, float lon2) {
		// returns distance in meters between two positions, both specified
		// as signed decimal-degrees latitude and longitude. Uses great-circle
		// distance computation for a hypothised sphere of radius 6372795 meters.
		float delta = math::deg2rad(lon1-lon2);
		float sdlon = sin(delta);
		float cdlon = cos(delta);
		lat1 = math::deg2rad(lat1);
		lat2 = math::deg2rad(lat2);
		float slat1 = sin(lat1);
		float clat1 = cos(lat1);
		float slat2 = sin(lat2);
		float clat2 = cos(lat2);
		delta = (clat1 * slat2) - (slat1 * clat2 * cdlon);
		delta = math::sq(delta);
		delta += math::sq(clat2 * sdlon);
		delta = sqrt(delta);
		float denom = (slat1 * slat2) + (clat1 * clat2 * cdlon);
		delta = atan2(delta, denom);
		return delta * 6372795.0; 
	}

	float heading_fromto(float lat1, float lon1, float lat2, float lon2) {
		// returns heading in degrees (North=0, West=270) from
		// position 1 to position 2, both specified as signed decimal-degrees
		// latitude and longitude.
		float dlon = math::deg2rad(lon2-lon1);
		lat1 = math::deg2rad(lat1);
		lat2 = math::deg2rad(lat2);
		float a1 = sin(dlon) * cos(lat2);
		float a2 = sin(lat1) * cos(lat2) * cos(dlon);
		a2 = cos(lat1) * sin(lat2) - a2;
		a2 = atan2(a1, a2);
		if (a2 < 0.0) {
			a2 += M_PI*2.0;			// poor man's fmod() 
		}
		return math::rad2deg(a2);
	}
}

