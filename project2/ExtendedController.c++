// ExtendedController.c++

#include "ExtendedController.h"
#include "ModelView.h"

ExtendedController::ExtendedController(const std::string& name, int rcFlags):
	GLFWController(name, rcFlags)
{
}

void ExtendedController::handleMouseMotion(int x, int y)
{
	// compute (dx,dy), the offset in pixel space from last event
	int dx = (x - screenBaseX);
	int dy = (y - screenBaseY);
	// update so that next call will get the offset from the location of this event
	screenBaseX = x;
	screenBaseY = y;
	if (mouseMotionIsTranslate)
	{
		// convert (dx, dy) into LDS (-1..+1) range
		int vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);
		double ldsPerPixelX = 2.0/vp[2];
		double ldsPerPixelY = 2.0/vp[3];
		ModelView::addToGlobalPan(dx*ldsPerPixelX, -dy*ldsPerPixelY, 0.0);
		redraw();
	}
	else if (mouseMotionIsRotate)
	{
		// convert (dx,dy) into rotation angles about y and x axis, respectively
		const double pixelsToDegrees = 360.0 / 500.0;
		double screenRotY = pixelsToDegrees * dx;
		double screenRotX = pixelsToDegrees * dy;
		ModelView::addToGlobalRotationDegrees(screenRotX, screenRotY, 0.0);
		redraw();
	}
}
