#ifndef _WINDOWS
#include <unistd.h>
#endif
#include "gpx.h"

#include <expat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stack>

#ifdef _WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#define OPEN	_open
#define CLOSE	_close
#define READ	_read
#else
#define OPEN	open
#define CLOSE	close
#define READ	read
#endif

namespace SPEEDMETER {

/**
 *
 *
 */
enum GPX_XML_ELEMENTS {
	_GPX = 0, 
	_METADATA, 
	_TRK, 
	_NAME, 
	_TRKSEG, 
	_TRKPT, 
	_TRKELE, 
	_TRKTIME
};

/**
 *
 *
 */
static const char *GPX_ELEMENTS_STR[] = {
	"gpx",
	 "metadata", 
	"trk", 
	"name",
	"trkseg", 
	"trkpt", 
	"ele", 
	"time" 
};

/**
 *
 *
 */
struct XMLState {
	GPX_XML_ELEMENTS element;
	const char *text;
	std::stringstream content;

	explicit XMLState(GPX_XML_ELEMENTS ele) :
			element(ele), text(GPX_ELEMENTS_STR[ele]) {}
	explicit XMLState(const XMLState & e) :
			element(e.element), text(e.text), content(e.content.str()) {}
};

/**
 *
 *
 */
struct XMLUserData {
	GPX *gpx;
	GPX::TrkPoint point;
	std::stack<XMLState> xmlStack;
};

/**
 *
 *
 */
static const char *getAttribute(const XML_Char ** atts, const XML_Char * attname) {
	for (const XML_Char ** att = atts; *att; att += 2)
		if (!strcmp(*att, attname))
			return *(++att);
	return NULL;
}

/**
 *
 *
 */
static time_t getTimeFromUTC(const char *st) {
	// EJ: 2012-07-28T06:01:40Z
	struct tm _tm;
//#ifndef _WINDOWS
	strptime(st, "%Y-%m-%dT%H:%M:%SZ", &_tm);
//#else
//	scanf_s("%d-%d-%dT%d:%d:%dZ",&_tm.tm_year,&_tm.tm_mon,&_tm.tm_mday, &_tm.tm_hour,&_tm.tm_min,&_tm.tm_sec);
//#endif
	return mktime(&_tm);
}

/**
 *
 *
 */
GPX::GPX() :trkTime(0), trks_pos(0), trks_index(0),
	 maxEle(0.0), minEle(10000.0), maxLat(-360.0), 
	maxLon(-360.0), minLat(360.0), minLon(360.0), totalTrks(0) {
}

/**
 *
 *
 */
GPX::~GPX() {
}

/**
 *
 *
 */
void GPX::XMLStartElementHandler(void *userData, const XML_Char * name,	const XML_Char ** atts) {
	XMLUserData *data = static_cast<XMLUserData *>(userData);
	const XMLState & actualState = data->xmlStack.top();
	switch (actualState.element) {
	case _GPX:
		if (strcmp(name, "metadata") == 0) {
			XMLState st(_METADATA);
			data->xmlStack.push(st);
		} else if (strcmp(name, "trk") == 0) {
			XMLState st(_TRK);
			data->xmlStack.push(st);
		}
		break;
	case _METADATA:
		break;	// TODO: parse metadata fields
	case _TRK:
		if (strcmp(name, "name") == 0) {
			XMLState st(_NAME);
			data->xmlStack.push(st);
		} else if (strcmp(name, "trkseg") == 0) {
			XMLState st(_TRKSEG);
			data->xmlStack.push(st);
		}
		break;
	case _NAME:
		// ERROR
		break;
	case _TRKSEG:
		if (strcmp(name, "trkpt") == 0) {
			XMLState st(_TRKPT);
			data->xmlStack.push(st);
			GPX *gpx = data->gpx;
			memset(&data->point, 0, sizeof(GPX::TrkPoint));

			const char *latAttr = getAttribute(atts, "lat");
			if (latAttr) {
				double l = atof(latAttr);
				data->point.coord.lat = l;
				if (gpx->maxLat < l)
					gpx->maxLat = l;
				if (gpx->minLat > l)
					gpx->minLat = l;
				const char *lonAttr = getAttribute(atts, "lon");
				if (lonAttr) {
					double l = atof(lonAttr);
					if (gpx->maxLon < l)
						gpx->maxLon = l;
					if (gpx->minLon > l)
						gpx->minLon = l;
					data->point.coord.lon = l;
					break;
				}
			}
			// eRROR....
		}
		break;
	case _TRKPT:
		if (strcmp(name, "ele") == 0) {
			XMLState st(_TRKELE);
			data->xmlStack.push(st);
		} else if (strcmp(name, "time") == 0) {
			XMLState st(_TRKTIME);
			data->xmlStack.push(st);
		}
		break;
	case _TRKELE:
	case _TRKTIME:
		break;
	default:
		//eRROR
		;
	}
}

/**
 *
 *
 */
void GPX::XMLEndElementHandler(void *userData, const XML_Char * name) {
	XMLUserData *data = static_cast<XMLUserData *>(userData);
	const XMLState & actualState = data->xmlStack.top();

	if (strcmp(actualState.text, name) == 0) {
		GPX *gpx = data->gpx;
		switch (actualState.element) {
		case _TRKPT:
			gpx->trks.push_back(data->point);
			gpx->totalTrks++;
			break;
		case _TRKELE: {
			double e = atof(actualState.content.str().c_str());
			data->point.ele = e;
			if (e > gpx->maxEle)
				gpx->maxEle = e;
			if (e < gpx->minEle)
				gpx->minEle = e;
		}
			break;
		case _TRKTIME:
			data->point.time = getTimeFromUTC(
					actualState.content.str().c_str());
			break;
		}
		data->xmlStack.pop();
	}
}

/**
 *
 *
 */
void GPX::XMLCharactersHandler(void *userData, const XML_Char * s, int len) {
	XMLUserData *data = static_cast<XMLUserData *>(userData);
	XMLState & actualState = data->xmlStack.top();
	actualState.content.write(static_cast<const char *>(s), len);
}

/**
 *
 *
 */
const std::list<GPX::TrkPoint> &GPX::getTrackList() const {
	return trks;
}

/**
 *
 *
 */
bool GPX::load(const char *filename) {
	XML_Status ret = XML_STATUS_ERROR;
	int fd = OPEN(filename, O_RDONLY);
	if (fd > 0) {
		// initialize track point list
		trks.clear();

		// create XML parser
		XML_Parser parser = XML_ParserCreate(NULL);

		// setting XML handlers
		XML_SetElementHandler(parser, GPX::XMLStartElementHandler,
				GPX::XMLEndElementHandler);
		XML_SetCharacterDataHandler(parser, GPX::XMLCharactersHandler);

		// setting user data that gets passed to handlers
		XMLUserData userData;
		userData.gpx = this;
		memset(&userData.point, 0, sizeof(GPX::TrkPoint));
		XML_SetUserData(parser, &userData);

		// create first parser state
		XMLState st(_GPX);
		userData.xmlStack.push(st);

		// parsing loop
		for (;;) {
			void *buf = XML_GetBuffer(parser, 4192);
			int bufSize = READ(fd, buf, 4192);
			if (bufSize	> 0 && (ret = XML_ParseBuffer(parser, bufSize, bufSize != 4192)) == XML_STATUS_OK)
				continue;
			// Error
			break;
		}

		CLOSE(fd);
		XML_ParserFree(parser);
	}
	return ret == XML_STATUS_OK;
}
}
