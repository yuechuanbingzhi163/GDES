#include "StdAfx.h"
#include "JointDraw.h"

// ×¢Òâ£ºÓÐÐÞ¸Ä
ACRX_NO_CONS_DEFINE_MEMBERS ( JointDraw, MineGEDraw )

JointDraw::JointDraw()
{

}

void JointDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_inserPt );
}

void JointDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_inserPt );
}

void JointDraw::extendByLength( double length )
{

}