#include "Configure.h"

// Raduis.
const IZ_FLOAT Configure::RadiusDiff = 10.0f;
const IZ_FLOAT Configure::InnerRadius = 80.0f;
const IZ_FLOAT Configure::OuterRadius = Configure::InnerRadius + Configure::RadiusDiff;
const IZ_FLOAT Configure::MostOuterRadius = Configure::OuterRadius + Configure::RadiusDiff;

const IZ_FLOAT Configure::CameraDistance = 30.0f;
const IZ_FLOAT Configure::CameraRadius = Configure::RadiusDiff * 2.0f + Configure::CameraDistance;

// Height.
const IZ_FLOAT Configure::HeightPerFloor = 10.0f;
const IZ_FLOAT Configure::SeatHeight = Configure::HeightPerFloor * 2.0f;

const IZ_FLOAT Configure::Depth = 1.0f;

const IZ_FLOAT Configure::MaxAngleRate = IZ_MATH_PI2 / 60.0f;

const IZ_FLOAT Configure::ChangeViewDuration = 500.0f;  // [ms]
const IZ_FLOAT Configure::MoveToItemDuration = 750.0f;  // [ms]
const IZ_FLOAT Configure::FadeInDuration = 500.0f;      // [ms]

const IZ_FLOAT Configure::CameraDistanceFromItem = 10.0f;

const IZ_FLOAT Configure::FadeInHeight = 1.5f;
