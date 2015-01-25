#include "Configure.h"

const IZ_UINT Configure::SCREEN_WIDTH = 1280;
const IZ_UINT Configure::SCREEN_HEIGHT = 720;

const IZ_UINT Configure::RING_SLICES = 100;;

const IZ_FLOAT Configure::INNER_RADIUS = 28.0f;
const IZ_FLOAT Configure::OUTER_RADIUS = 30.0f;

const IZ_FLOAT Configure::RING_HEIGHT = 1.0f;

const IZ_COLOR Configure::RING_COLOR = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

const IZ_FLOAT Configure::Radius = (Configure::OUTER_RADIUS + Configure::INNER_RADIUS) * 0.5f;

const IZ_UINT Configure::ItemNum = 30;

const IZ_FLOAT Configure::ItemWidth = 5.5f;
const IZ_FLOAT Configure::ItemHeight = ItemWidth * 2.0f;
const IZ_FLOAT Configure::ItemDepth = (Configure::OUTER_RADIUS - Configure::INNER_RADIUS) * 0.5f;

const IZ_FLOAT Configure::ItemBoardMargin = 2.0f;

const IZ_FLOAT Configure::DefaultBevelR = 0.05f;

const IZ_UINT Configure::BoxMeshFlags = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

const IZ_UINT Configure::BoardMeshFlags = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

const IZ_COLOR Configure::DefaultColor = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

const IZ_FLOAT Configure::MaxAngleRate = IZ_MATH_PI2 / 60.0f;

const IZ_FLOAT Configure::Gradient = IZ_DEG2RAD(-10.0f);
