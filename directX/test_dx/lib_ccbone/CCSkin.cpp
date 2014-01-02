#include "CCSkin.h"
#include "CCData.h"
#include "CCBone.h"

CCSkin::CCSkin()
{

}

CCSkin::~CCSkin()
{

}

bool CCSkin::init( CCBaseData* data, CCBone* bone )
{
	if (data == NULL) return false;
	if (bone == NULL) return false;

	m_data = data;

	setScaleX(m_data->scaleX);
	setScaleY(m_data->scaleY);
	setRotationX(CC_RADIANS_TO_DEGREES(m_data->skewX));
	setRotationY(CC_RADIANS_TO_DEGREES(-m_data->skewY));
	setPosition(CCPointMake(m_data->x, m_data->y));

	m_skin_transform = nodeToParentTransform();
	m_sTransform = CCAffineTransformConcat(m_skin_transform, bone->m_world_transform);
	return true;
}

void CCSkin::updateTransform()
{
	// If it is not visible, or one of its ancestors is not visible, then do nothing:
	if( !m_bVisible)
	{
		//m_sQuad.br.vertices = m_sQuad.tl.vertices = m_sQuad.tr.vertices = m_sQuad.bl.vertices = vertex3(0, 0, 0);
	}
	else
	{
		//
		// calculate the Quad based on the Affine Matrix
		//

		CCSize &size = m_obRect.size;

		float x1 = m_obOffsetPosition.x;
		float y1 = m_obOffsetPosition.y;

		float x2 = x1 + size.width;
		float y2 = y1 + size.height;

		float x = m_sTransform.tx;
		float y = m_sTransform.ty;

		float cr = m_sTransform.a;
		float sr = m_sTransform.b;
		float cr2 = m_sTransform.d;
		float sr2 = -m_sTransform.c;
		float ax = x1 * cr - y1 * sr2 + x;
		float ay = x1 * sr + y1 * cr2 + y;

		float bx = x2 * cr - y1 * sr2 + x;
		float by = x2 * sr + y1 * cr2 + y;

		float cx = x2 * cr - y2 * sr2 + x;
		float cy = x2 * sr + y2 * cr2 + y;

		float dx = x1 * cr - y2 * sr2 + x;
		float dy = x1 * sr + y2 * cr2 + y;

		//SET_VERTEX3F( m_sQuad.bl.vertices, RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), m_fVertexZ );
		//SET_VERTEX3F( m_sQuad.br.vertices, RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), m_fVertexZ );
		//SET_VERTEX3F( m_sQuad.tl.vertices, RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), m_fVertexZ );
		//SET_VERTEX3F( m_sQuad.tr.vertices, RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), m_fVertexZ );
	}

	// MARMALADE CHANGE: ADDED CHECK FOR NULL, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
	//if (m_pobTextureAtlas)
	//{
	//	m_pobTextureAtlas->updateQuad(&m_sQuad, m_pobTextureAtlas->getTotalQuads());
	//}
}
