#pragma once

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#pragma db object
class PumpType 
{
public:
	friend class odb::access;
	PumpType() {}

//public:
//	bool operator==( const PumpType& tt )
//	{
//		return (type == tt.type && absP == tt.absP && factName == tt.factName &&
//			    weight == tt.weight && width == tt.width && length == tt.length && height == tt.height);
//	}

public:
	#pragma db id auto
	unsigned long id_;

	std::string type;
	std::string factName;
	int absP;
	int weight;
	int length;
	int width;
	int height;
};

#pragma db object
class PumpProperty 
{
public:
	friend class odb::access;
	PumpProperty() {}

//public:
//	bool operator==( const PumpProperty& pt )
//	{
//		return (speed == pt.speed && power == pt.power && maxQ == pt.maxQ && maxP == pt.maxP && absP == pt.absP );
//	}

public:
	#pragma db id auto
	unsigned long id_;

	int speed;
	double power;
	double maxQ;
	int maxP;
	int absP;
};