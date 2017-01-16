

#ifndef POINTREADER_H
#define POINTREADER_H

#ifndef DISABLE_BOOST
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
#endif

#include "Point.h"
#include "AABB.h"

#ifndef DISABLE_BOOST
namespace fs = boost::filesystem;

using boost::iequals;
#endif

namespace Potree{

class PointReader{
public:

	virtual ~PointReader(){};

	virtual bool readNextPoint() = 0;

	virtual Point getPoint() = 0;

	virtual AABB getAABB() = 0;

	virtual long numPoints() = 0;

	virtual void close() = 0;
};

}

#endif