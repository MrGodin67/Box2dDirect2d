#include "Box2DDraw.h"

DebugDrawGDI::DebugDrawGDI()
{
}

// set w to the box2D world AABB
// use this to help scale/transform our world
void DebugDrawGDI::GetBoundBox2DBounds(b2AABB *w, b2World *world)
{
	// iterate over ALL the bodies, and set the w to max/min
	b2Body *b;
	b2Fixture *fix;
	b2AABB bound;
	float minX, maxX, minY, maxY;

	minX = minY = 1000000.0;
	maxX = maxY = -1000000.0;

	b = world->GetBodyList();
	while (b)
	{
		fix = b->GetFixtureList();
		while (fix)
		{
			bound = fix->GetAABB(0);
			if (bound.lowerBound.x < minX)
				minX = bound.lowerBound.x;
			if (bound.upperBound.x > maxX)
				maxX = bound.upperBound.x;
			if (bound.lowerBound.y < minY)
				minY = bound.lowerBound.y;
			if (bound.upperBound.y > maxY)
				maxY = bound.upperBound.y;

			fix = fix->GetNext();
		}

		b = b->GetNext();
	}

	maxX += 2.0;
	maxY += 2.0;
	minX -= 2.0;
	minY -= 2.0;
	w->left = (long)minX;
	w->right = (long)maxX;
	w->top = (long)maxY;
	w->bottom = (long)minY;

}

//Now that we know how big everything is we can calculate our transform :
//Hide   Shrink   Copy Code

// set w to the box2D world AABB
// use this to help scale/transform our world
void DebugDrawGDI::GetBoundBox2DBounds(RECT *w, b2World *world)
{
	// iterate over ALL the bodies, and set the w to max/min
	b2Body *b;
	b2Fixture *fix;
	b2AABB bound;
	float minX, maxX, minY, maxY;

	minX = minY = 1000000.0;
	maxX = maxY = -1000000.0;

	b = world->GetBodyList();
	while (b)
	{
		fix = b->GetFixtureList();
		while (fix)
		{
			bound = fix->GetAABB(0);
			if (bound.lowerBound.x < minX)
				minX = bound.lowerBound.x;
			if (bound.upperBound.x > maxX)
				maxX = bound.upperBound.x;
			if (bound.lowerBound.y < minY)
				minY = bound.lowerBound.y;
			if (bound.upperBound.y > maxY)
				maxY = bound.upperBound.y;

			fix = fix->GetNext();
		}

		b = b->GetNext();
	}

	maxX += 2.0;
	maxY += 2.0;
	minX -= 2.0;
	minY -= 2.0;
	w->left = (long)minX;
	w->right = (long)maxX;
	w->top = (long)maxY;
	w->bottom = (long)minY;

}

//Then in our main drawing loop we just have to set the transform, something like this:
//
//renderTarget->SetTransform(matrixTransform);

//Where renderTarget is a pointer to our current DirectX 2D render factory object.With the transform in place our drawing functions look like this:
//Hide   Shrink   Copy Code

/// Draw a solid closed polygon provided in CCW order.
void DebugDrawGDI::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	int i;
	ID2D1PathGeometry *geo;
	ID2D1GeometrySink *sink;
	ID2D1SolidColorBrush *brush;
	D2D1::ColorF dColor(color.r, color.g, color.b);
	D2D1_POINT_2F *points = new D2D1_POINT_2F[vertexCount + 1];
	HRESULT hr;

	// create a direct2d pathGeometry
	hr = factory->CreatePathGeometry(&geo);
	hr = geo->Open(&sink);
	sink->SetFillMode(D2D1_FILL_MODE_WINDING);
	// first point
	sink->BeginFigure(D2D1::Point2F(vertices[0].x, vertices[0].y), D2D1_FIGURE_BEGIN_FILLED);
	// middle points
	vertices++;
	vertexCount--;
	for (i = 0; i < vertexCount; i++, vertices++)
	{
		points[i].x = vertices->x;
		points[i].y = vertices->y;
	}
	points[vertexCount].x = points[0].x;
	points[vertexCount].y = points[0].y;
	sink->AddLines(points, vertexCount);
	// close it
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	SafeRelease(&sink);

	renderTarget->CreateSolidColorBrush(dColor, &brush);
	renderTarget->FillGeometry(geo, brush);

	delete points;
	SafeRelease(&geo);
}

Note that we do no scaling or mapping in the drawing function.The transform takes care of it all for us.All that is left is to setup some animation and timing animation code.We have to change the default message loop so it doesn't just wait for messages. Not much animation happens if your drawing code only gets called when a key is pressed. Use this code to process messages:
Hide   Copy Code

// prime the message structure
PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
// run till completed
while (msg.message != WM_QUIT)
{
	// is there a message to process?
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// dispatch the message
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	else
	{
		// no windows messages, do our game stuff
		if (ajrMain.MainLogic())
		{
			ajrMain.MainDraw();
		}
	}
}
