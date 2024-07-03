#include "OCCTPProxy.h"

bool OCCTProxy::InitViewer(System::IntPtr theWnd) {
    try
    {
        Handle(Aspect_DisplayConnection) aDisplayConnection;
        myGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }
    catch (Standard_Failure)
    {
        return false;
    }

    myViewer() = new V3d_Viewer(myGraphicDriver());
    myViewer()->SetDefaultLights();
    myViewer()->SetLightOn();
    myView() = myViewer()->CreateView();
    Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
    myView()->SetWindow(aWNTWindow);
    Standard_Integer w = 100, h = 100;
    aWNTWindow->Size(w, h);
    if (!aWNTWindow->IsMapped())
    {
        aWNTWindow->Map();
    }
    myAISContext() = new AIS_InteractiveContext(myViewer());
    myAISContext()->UpdateCurrentViewer();
    myView()->Redraw();
    myView()->MustBeResized();

    TColgp_Array1OfPnt nodes = TColgp_Array1OfPnt(0, pointCount - 1);
    for (int i = 0; i < pointCount; i++) {
        nodes(i) = points[i];
    }
    Poly_Array1OfTriangle ptri = Poly_Array1OfTriangle(0, triangleCount - 1);
    for (int i = 0; i < triangleCount; i++) {
        ptri(i) = triangles[i];
    }

    Handle(Poly_Triangulation) tria1 = new Poly_Triangulation(
        nodes,
        ptri
    );

    Handle(AIS_Triangulation) tria1Ais = new AIS_Triangulation(tria1);
    myAISContext()->Display(tria1Ais, true);
    myAISContext()->UpdateCurrentViewer();
    myView()->Redraw();

    return true;
}

bool OCCTProxy::ImportBrep(System::String^ theFileName)
{
    return ImportBrep(toAsciiString(theFileName));
}
