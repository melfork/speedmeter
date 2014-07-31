#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "basictypes.h"

#define MAP_FORMAT	"png"
#define MAP_SENSOR	"false"

/**
 *
 */
static char *createURL(int zoom, SPEEDMETER::Coordinate const &center,
		int width, int heigth) {
	char *url = new char[128];

	snprintf(url, 128, "http://maps.googleapis.com/maps/api/staticmap"
			"?center=%f,%f"
			"&zoom=%d"
			"&size=%dx%d"
			"&format="
			MAP_FORMAT
			"&sensor="
			MAP_SENSOR, center.lat, center.lon, zoom, width, heigth);
	return url;
}

/**
 *
 */
struct MemoryStruct {
	char *memory;
	size_t size;
	size_t allocSize;
};

/**
 *
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *) userp;

	if (realsize + mem->size > mem->allocSize) {
		mem->allocSize += 4096;
		mem->memory = (char*) realloc(mem->memory, mem->allocSize);
		if (mem->memory == NULL) {
			/* out of memory! */
			printf("not enough memory (realloc returned NULL)\n");
			return 0;
		}
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	return realsize;
}

// return the image size 
int getGoogleMapsImage(int zoom, SPEEDMETER::Coordinate const &center,int width, int heigth, char **image) {
	int ret = -1;
	CURLcode res;

	struct MemoryStruct chunk;

	chunk.allocSize = 4096;
	chunk.memory = (char*)malloc(chunk.allocSize); /* will be grown as needed by the realloc above */
	chunk.size = 0; /* no data at this point */

	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	CURL *curl_handle = curl_easy_init();

	/* specify URL to get */
	char *url = createURL(zoom, center, width, heigth);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	/* some servers don't like requests that are made without a user-agent
	 *      field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* get it! */
	res = curl_easy_perform(curl_handle);

	/* check for errors */
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",	curl_easy_strerror(res));
	} else {
		/*
		 * Now, our chunk.memory points to a memory block that is chunk.size
		 * bytes big and contains the remote file.
		 */
		*image = chunk.memory;
		ret = chunk.size;
	}

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	delete[] url;

	/* we're done with libcurl, so clean it up */
	curl_global_cleanup();

	return ret;
}

