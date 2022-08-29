#ifndef MEDIA_H
#define MEDIA_H

#include <string>
#include <map>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

enum Error {
	INCORRECT_FILENAME,
	INCORRECT_FORMAT,
	LOADING_ERROR
};

class MediaObject {
public:
	virtual void show ();
	virtual cv::Mat *getData ();
};

class Photo : public MediaObject {
private:
	cv::Mat image;

public:
	Photo (std::string path);
	~Photo ();

	virtual void show ();
	virtual cv::Mat *getData ();
};

class MediaLoader {
private:
	enum MediaFormat {
		PHOTO
	};

	static MediaLoader *mediaLoader;

	std::map<std::string, MediaFormat> formats;

	MediaLoader ();
	~MediaLoader ();

public:
	static MediaLoader *getInstance ();

	MediaObject *load (std::string path);
};

#endif