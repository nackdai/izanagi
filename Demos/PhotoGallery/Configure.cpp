#include "Configure.h"

// Raduis.
const IZ_FLOAT Configure::RadiusDiff = 10.0f;
const IZ_FLOAT Configure::InnerRadius = 100.0f;
const IZ_FLOAT Configure::OuterRadius = Configure::InnerRadius + Configure::RadiusDiff;
const IZ_FLOAT Configure::MostOuterRadius = Configure::OuterRadius + Configure::RadiusDiff;

const IZ_FLOAT Configure::CameraDistance = 30.0f;
const IZ_FLOAT Configure::CameraRadius = Configure::RadiusDiff * 2.0f + Configure::CameraDistance;

// Height.
const IZ_FLOAT Configure::HeightPerFloor = 10.0f;
const IZ_FLOAT Configure::SeatHeight = Configure::HeightPerFloor * 2.0f;

const IZ_FLOAT Configure::Depth = 2.0f;

const IZ_FLOAT Configure::MaxAngleRate = IZ_MATH_PI2 / 60.0f;

const IZ_FLOAT Configure::ChangeViewTime = 500.0f; // [ms]