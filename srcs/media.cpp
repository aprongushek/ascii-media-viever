#include "media.h"

void MediaObject::show () { }

cv::Mat *MediaObject::getData ()
{
	return nullptr;
}

Photo::Photo (std::string path)
{
	image = cv::imread(path, cv::IMREAD_GRAYSCALE);
	if (image.empty())
		throw LOADING_ERROR;
}

Photo::~Photo () 
{
	image.release();
}

void Photo::show () 
{
	cv::imshow("image", image);
	cv::waitKey(0);
}

cv::Mat *Photo::getData ()
{
	return &image;
}

MediaLoader *MediaLoader::mediaLoader = nullptr;

MediaLoader::MediaLoader ()
{
	formats["jpg"] = PHOTO;
	formats["png"] = PHOTO;
}

MediaLoader::~MediaLoader ()
{
	if (mediaLoader)
		delete mediaLoader;
}

MediaLoader *MediaLoader::getInstance ()
{
	if (!mediaLoader)
		mediaLoader = new MediaLoader;

	return mediaLoader;
}

MediaObject *MediaLoader::load (std::string path)
{
	int pos = path.find('.');
	if (pos == std::string::npos)
		throw INCORRECT_FILENAME;

	std::string fileFormatStr = path;
	fileFormatStr.erase(0, pos + 1);

	MediaFormat fileFormat = formats[fileFormatStr];
	switch (fileFormat) {
		case PHOTO:
			return new Photo(path);
		default:
			throw INCORRECT_FORMAT;
	}
}

