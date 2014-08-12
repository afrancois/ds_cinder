#include "camera_params.h"

namespace ds {

/**
 * \class ds::PerspCameraParams
 */
PerspCameraParams::PerspCameraParams()
		: mFov(60.0f)
		, mNearPlane(0.0f)
		, mFarPlane(1000.0f) {
}

bool PerspCameraParams::operator==(const PerspCameraParams& o) const {
	if (this == &o) return true;

	return mPosition == o.mPosition && mTarget == o.mTarget && mFov == o.mFov && mNearPlane == o.mNearPlane && mFarPlane == o.mFarPlane;
}

}