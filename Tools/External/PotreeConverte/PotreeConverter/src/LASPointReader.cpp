

#include <fstream>
#include <iostream>
#include <vector>

#ifndef DISABLE_BOOST
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
#endif
#include "laszip_api.h"

#include "LASPointReader.h"

#ifndef DISABLE_BOOST
namespace fs = boost::filesystem;
#endif

using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
#ifndef DISABLE_BOOST
using boost::iequals;
#endif
using std::ios;

namespace Potree{

AABB LIBLASReader::getAABB(){
    AABB aabb;

    Point minp = transform(header->min_x, header->min_y, header->min_z);
    Point maxp = transform(header->max_x, header->max_y, header->max_z);
    aabb.update(minp.position);
    aabb.update(maxp.position);

    return aabb;
}

LASPointReader::LASPointReader(string path){
	this->path = path;

#ifndef DISABLE_BOOST
	if(fs::is_directory(path)){
		// if directory is specified, find all las and laz files inside directory

		for(fs::directory_iterator it(path); it != fs::directory_iterator(); it++){
			fs::path filepath = it->path();
			if(fs::is_regular_file(filepath)){
				if(iequals(fs::extension(filepath), ".las") || iequals(fs::extension(filepath), ".laz")){
					files.push_back(filepath.string());
				}
			}
		}
	}else{
		files.push_back(path);
	}
#else
    files.push_back(path);
#endif
	

	// read bounding box
	for (const auto &file : files) {
		LIBLASReader aabbReader(file);
		AABB lAABB = aabbReader.getAABB();
		
		aabb.update(lAABB.min);
		aabb.update(lAABB.max);

		aabbReader.close();
	}

	// open first file
	currentFile = files.begin();
	reader = new LIBLASReader(*currentFile);
//    cout << "let's go..." << endl;
}

LASPointReader::~LASPointReader(){
	close();
}

void LASPointReader::close(){
	if(reader != NULL){
		reader->close();
		delete reader;
		reader = NULL;
	}
}

long LASPointReader::numPoints(){
	return reader->header->number_of_point_records;
}

bool LASPointReader::readNextPoint(){

	bool hasPoints = reader->readPoint();

	if(!hasPoints){
		// try to open next file, if available
		reader->close();
		delete reader;
		reader = NULL;

		currentFile++;

		if(currentFile != files.end()){
			reader = new LIBLASReader(*currentFile);
			hasPoints = reader->readPoint();
		}
	}

	return hasPoints;
}

Point LASPointReader::getPoint(){
	Point const p = reader->GetPoint();
    return p;
}

AABB LASPointReader::getAABB(){
	return aabb;
}

Vector3<double> LASPointReader::getScale(){

	Vector3<double> scale;
	scale.x =reader->header->x_scale_factor;
	scale.y =reader->header->y_scale_factor;
	scale.z =reader->header->z_scale_factor;

	return scale;
}

}