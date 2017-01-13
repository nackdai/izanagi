
#ifndef LASPOINTWRITER_H
#define LASPOINTWRITER_H

#include <string>
#include <iostream>
#include <fstream>

#include "laszip_dll.h"
#include <boost/algorithm/string/predicate.hpp>

#include "AABB.h"
#include "PointWriter.hpp"
#include "Point.h"

using std::string;
using std::fstream;
using std::ios;
using boost::algorithm::iends_with;

namespace Potree{

class LASPointWriter : public PointWriter{

public:
	string file;
	int numPoints = 0;
	AABB aabb;
	laszip_POINTER writer = NULL;
	laszip_header header;
	laszip_point* point;
	double coordinates[3];

	LASPointWriter(string file, AABB aabb, double scale) {
		this->file = file;
		this->aabb = aabb;
		numPoints = 0;
		

		memset(&header, 0, sizeof(laszip_header));
		strcpy(header.generating_software, "potree");

		header.version_major = 1;
		header.version_minor = 2;
		header.header_size = 227;
		header.offset_to_point_data = 227;
		header.point_data_format = 2;
		header.min_x = aabb.min.x;
		header.min_y = aabb.min.y;
		header.min_z = aabb.min.z;
		header.max_x = aabb.max.x;
		header.max_y = aabb.max.y;
		header.max_z = aabb.max.z;
		header.x_scale_factor = scale;
		header.y_scale_factor = scale;
		header.z_scale_factor = scale;
		header.point_data_record_length = 26;
		header.number_of_point_records = 111;


		laszip_create(&writer);

		laszip_BOOL compress = iends_with(file, ".laz") ? 1 : 0;
		if(compress){
			laszip_BOOL request_writer = 1;
			laszip_request_compatibility_mode(writer, request_writer);
		}

		laszip_set_header(writer, &header);
		laszip_open_writer(writer, file.c_str(), compress);

		laszip_get_point_pointer(writer, &point);
	}

	~LASPointWriter(){
		close();
	}

	void write(const Point &point);

	void close(){

		if(writer != NULL){
			laszip_close_writer(writer);
			laszip_destroy(writer);
			writer = NULL;

			fstream *stream = new fstream(file, ios::out | ios::binary | ios::in );
			stream->seekp(107);
			stream->write(reinterpret_cast<const char*>(&numPoints), 4);
			stream->close();
			delete stream;

		}

	}

};

}

#endif


