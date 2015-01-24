#include "Configure.h"

const IZ_UINT Configure::SCREEN_WIDTH = 1280;
const IZ_UINT Configure::SCREEN_HEIGHT = 720;

const IZ_UINT Configure::RING_SLICES = 100;;

const IZ_FLOAT Configure::INNER_RADIUS = 28.0f;
const IZ_FLOAT Configure::OUTER_RADIUS = 30.0f;

const IZ_FLOAT Configure::RING_HEIGHT = 2.0f;

const IZ_COLOR Configure::RING_COLOR = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

const IZ_FLOAT Configure::Radius = (Configure::OUTER_RADIUS + Configure::INNER_RADIUS) * 0.5f;

const IZ_UINT Configure::ItemNum = 30;

const IZ_FLOAT Configure::ItemWidth = 5.0;
const IZ_FLOAT Configure::ItemHeight = ItemWidth * 2.0f;
const IZ_FLOAT Configure::ItemDepth = (Configure::OUTER_RADIUS - Configure::INNER_RADIUS) * 0.5f;

const IZ_FLOAT Configure::ItemBoardMargin = 2.0f;

const IZ_UINT Configure::BoxMeshFlags = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

const IZ_UINT Configure::BoardMeshFlags = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

const IZ_COLOR Configure::DefaultColor = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);
const IZ_COLOR Configure::PhotoItemSideAndTopFaceColor = IZ_COLOR_RGBA(0x80, 0x80, 0x80, 0xff);
const IZ_COLOR Configure::SeatColor = IZ_COLOR_RGBA(0x20, 0x20, 0x20, 0xff);
const IZ_COLOR Configure::BGColor = IZ_COLOR_RGBA(0x20, 0x20, 0x20, 0xff);

// Raduis.
const IZ_FLOAT Configure::RadiusDiff = 10.0f;
const IZ_FLOAT Configure::InnerRadius = 80.0f;
const IZ_FLOAT Configure::MidRadius = Configure::InnerRadius + Configure::RadiusDiff;
const IZ_FLOAT Configure::OuterRadius = Configure::MidRadius + Configure::RadiusDiff;

const IZ_FLOAT Configure::CameraDistance = 30.0f;

// Height.
const IZ_FLOAT Configure::HeightPerFloor = 10.0f;
const IZ_FLOAT Configure::SeatHeight = Configure::HeightPerFloor * 2.0f;

const IZ_FLOAT Configure::Depth = 1.0f;

const IZ_FLOAT Configure::MaxAngleRate = IZ_MATH_PI2 / 60.0f;

const IZ_FLOAT Configure::ChangeViewDuration = 500.0f;  // [ms]
const IZ_FLOAT Configure::MoveToItemDuration = 750.0f;  // [ms]
const IZ_FLOAT Configure::FadeInDuration = 500.0f;      // [ms]

const IZ_FLOAT Configure::CameraDistanceFromItem = 10.0f;

//const IZ_UINT Configure::MaxTextureSize = 1280;
const IZ_UINT Configure::MaxTextureSize = 640;

const IZ_FLOAT Configure::FadeInHeight = 1.5f;

const IZ_FLOAT Configure::PhotoItemShownRange = 30.0f;
