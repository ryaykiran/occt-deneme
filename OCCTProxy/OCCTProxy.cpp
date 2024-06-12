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

    //const Standard_Real theWedgeDX = 10.0;
    //const Standard_Real theWedgeDY = 10.0;
    //const Standard_Real theWedgeDZ = 10.0;
    //const Standard_Real theWedgeLtx = 10.0;

    //BRepPrimAPI_MakeWedge aWedgeMaker(theWedgeDX, theWedgeDY, theWedgeDZ, theWedgeLtx);
    //TopoDS_Solid aShape = aWedgeMaker.Solid();
    //Handle(AIS_Shape) aShapePrs = new AIS_Shape(aShape); // creation of the presentable object
    //myAISContext()->Display(aShapePrs, Standard_False);   // display the presentable object and redraw 3d 
    //myAISContext()->UpdateCurrentViewer();

    //Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner();
    //Handle(Select3D_SensitiveTriangle) tr = new Select3D_SensitiveTriangle(anOwner, p1, p2, p3);

    //NCollection_Vector<gp_Pnt> nodesDyn1 = NCollection_Vector<gp_Pnt>();
    //nodesDyn1.Append(gp_Pnt(-5, 0, 0));
    //nodesDyn1.Append(gp_Pnt(5, 0, 0));
    //nodesDyn1.Append(gp_Pnt(5, 0, 1));
    //nodesDyn1.Append(gp_Pnt(-5, 0, 1));

    //coy from dynamic
    TColgp_Array1OfPnt nodes1 = TColgp_Array1OfPnt(points()->operator()(0), 0, points()->Upper());
    //
    NCollection_Vector<gp_Pnt> nodesDyn2 = NCollection_Vector<gp_Pnt>();
    nodesDyn2.Append(gp_Pnt(0, -5, 0));
    nodesDyn2.Append(gp_Pnt(0, 5, 0));
    nodesDyn2.Append(gp_Pnt(0, 5, 5));
    nodesDyn2.Append(gp_Pnt(0, -5, 5));
    nodesDyn2.Append(gp_Pnt(0, 0, 2.5));

    //coy from dynamic
    TColgp_Array1OfPnt nodes2 = TColgp_Array1OfPnt(nodesDyn2(0), 0, nodesDyn2.Upper());

    NCollection_Vector<Poly_Triangle> ptri1Dyn = NCollection_Vector<Poly_Triangle>();
    ptri1Dyn.Append(Poly_Triangle(1, 2, 3));
    ptri1Dyn.Append(Poly_Triangle(3, 4, 1));


    NCollection_Vector<Poly_Triangle> ptri2Dyn = NCollection_Vector<Poly_Triangle>();
    ptri2Dyn.Append(Poly_Triangle(1, 2, 5));
    ptri2Dyn.Append(Poly_Triangle(2, 3, 5));
    ptri2Dyn.Append(Poly_Triangle(3, 4, 5));
    ptri2Dyn.Append(Poly_Triangle(4, 1, 5));

    //coy from dynamic
    Poly_Array1OfTriangle ptri1 = Poly_Array1OfTriangle(ptri1Dyn(0), 0, ptri1Dyn.Upper());
    //Poly_Array1OfTriangle ptri2 = Poly_Array1OfTriangle(ptri2Dyn(0), 0, ptri2Dyn.Upper());
    //

    TColgp_Array1OfPnt nodes3 = TColgp_Array1OfPnt(0, points->Length - 1);
    //Poly_Array1OfTriangle ptri1 = Poly_Array1OfTriangle(0, pointsIndices->Length - 1);


    for (int i = 0; i < points->Length; i++) {
        auto item = points[i];
        nodes3.ChangeValue(i) = gp_Pnt(item->x, item->y, item->z);
    }

    //for (int i = 0; i < pointsIndices->Length; i++) {
    //    auto item = pointsIndices[i];
    //    ptri1.ChangeValue(i) = Poly_Triangle(item->x, item->y, item->z);
    //}

    Handle(Poly_Triangulation) tria1 = new Poly_Triangulation(
        nodes3,
        ptri1
    );

    /*Handle(Poly_Triangulation) tria2 = new Poly_Triangulation(
        nodes2,
        ptri2
    );*/

    //
    Handle(AIS_Shape) aisPoint1 = new AIS_Shape(BRepBuilderAPI_MakeVertex(nodes1[0]));/*
    Handle(AIS_Shape) aisPoint2 = new AIS_Shape(BRepBuilderAPI_MakeVertex(nodes1[1]));
    Handle(AIS_Shape) aisPoint3 = new AIS_Shape(BRepBuilderAPI_MakeVertex(nodes1[2]));*/

    // Display points
    //myAISContext()->Display(aisPoint1, Standard_True);
    //myAISContext()->Display(aisPoint2, Standard_True);
    //myAISContext()->Display(aisPoint3, Standard_True);



    // Create polygon to represent triangulation
    Handle(AIS_Triangulation) tria1Ais = new AIS_Triangulation(tria1);
    Handle(TColStd_HArray1OfInteger) colors = new TColStd_HArray1OfInteger(0, 3);
    colors->ChangeValue(0) = Quantity_NOC_AQUAMARINE1;
    colors->ChangeValue(1) = Quantity_NOC_ALICEBLUE;
    colors->ChangeValue(2) = Quantity_NOC_ANTIQUEWHITE;
    colors->ChangeValue(3) = Quantity_NOC_BROWN;

    tria1Ais->SetColors(colors);
    //Handle(AIS_Triangulation) tria2Ais = new AIS_Triangulation(tria2);
    //Handle(AIS_Shape) aisPolygon = new AIS_Shape(tria1);
    //tria2Ais->SetColors(colors);

    // Display polygon
    myAISContext()->Display(tria1Ais, Standard_True);
    //myAISContext()->Display(tria2Ais, Standard_True);
    //myView()->FitAll();
    //myView()->Redraw();
    //myView()->MustBeResized();
    //

    //myAISContext()->AddOrRemoveCurrentObject(new AIS_Triangulation(tria1),false);
    //myAISContext()->AddOrRemoveCurrentObject(new AIS_Triangulation(tria2),false);
    //myAISContext()->DisplayAll(true);
    //Handle(AIS_Shape) tria1AisPrs = new AIS_Shape(tria1);


    //Handle(AIS_Triangulation) tria2Ais = new AIS_Triangulation(tria2);
    ////Handle(AIS_Shape) aShapePrs = new AIS_Shape(direct tria1);
    //myAISContext()->Display(tria1Ais, false);
    //myAISContext()->Display(tria2Ais, false);
    ////myViewer()->Redraw();
    ////myView()->Redraw();
    ////myAISContext()->Display(tria1Ais, AIS_Shaded, 0, false, AIS_DS_Displayed);
    //myAISContext()->UpdateCurrentViewer();
    //myView()->Update();
    //myAISContext()->UpdateCurrentViewer();
    //myView()->Redraw();
    //myView()->MustBeResized();
    //nodes.get()
    //myViewer()->pol
    //myAISContext()->AddOrRemoveCurrentObject(tria1, true);
    //myAISContext()->Display(tria1);

    // read the data and create a data source
    //Handle(Poly_Triangulation) aSTLMesh = RWStl::ReadFile(aFileName);
    //Handle(MeshVS_DataSource) aDataSource = new MeshVS_DataSource3D(tria1);

    //// create mesh
    //Handle(MeshVS_Mesh) aMeshPrs = new MeshVS_Mesh();
    //aMeshPrs->SetDataSource(aDataSource);

    //// use default presentation builder
    //Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMeshPrs);
    //aMeshPrs->AddBuilder(aBuilder, true);

    return true;
}

bool OCCTProxy::ReadFile(IntPtr theWnd)
{
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

    //const Standard_Real theWedgeDX = 10.0;
    //const Standard_Real theWedgeDY = 10.0;
    //const Standard_Real theWedgeDZ = 10.0;
    //const Standard_Real theWedgeLtx = 10.0;

    //BRepPrimAPI_MakeWedge aWedgeMaker(theWedgeDX, theWedgeDY, theWedgeDZ, theWedgeLtx);
    //TopoDS_Solid aShape = aWedgeMaker.Solid();
    //Handle(AIS_Shape) aShapePrs = new AIS_Shape(aShape); // creation of the presentable object
    //myAISContext()->Display(aShapePrs, Standard_False);   // display the presentable object and redraw 3d 
    //myAISContext()->UpdateCurrentViewer();

    //Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner();
    //Handle(Select3D_SensitiveTriangle) tr = new Select3D_SensitiveTriangle(anOwner, p1, p2, p3);

    NCollection_Vector<gp_Pnt> nodesDyn1 = NCollection_Vector<gp_Pnt>();
    nodesDyn1.Append(gp_Pnt(-5, 0, 0));
    nodesDyn1.Append(gp_Pnt(5, 0, 0));
    nodesDyn1.Append(gp_Pnt(5, 0, 1));
    nodesDyn1.Append(gp_Pnt(-5, 0, 1));

    //coy from dynamic
    TColgp_Array1OfPnt nodes1 = TColgp_Array1OfPnt(nodesDyn1(0), 0, nodesDyn1.Upper());
    //
    NCollection_Vector<gp_Pnt> nodesDyn2 = NCollection_Vector<gp_Pnt>();
    nodesDyn2.Append(gp_Pnt(0, -5, 0));
    nodesDyn2.Append(gp_Pnt(0, 5, 0));
    nodesDyn2.Append(gp_Pnt(0, 5, 5));
    nodesDyn2.Append(gp_Pnt(0, -5, 5));
    nodesDyn2.Append(gp_Pnt(0, 0, 2.5));

    //coy from dynamic
    TColgp_Array1OfPnt nodes2 = TColgp_Array1OfPnt(nodesDyn2(0), 0, nodesDyn2.Upper());

    NCollection_Vector<Poly_Triangle> ptri1Dyn = NCollection_Vector<Poly_Triangle>();
    ptri1Dyn.Append(Poly_Triangle(1, 2, 3));
    ptri1Dyn.Append(Poly_Triangle(3, 4, 1));


    NCollection_Vector<Poly_Triangle> ptri2Dyn = NCollection_Vector<Poly_Triangle>();
    ptri2Dyn.Append(Poly_Triangle(1, 2, 5));
    ptri2Dyn.Append(Poly_Triangle(2, 3, 5));
    ptri2Dyn.Append(Poly_Triangle(3, 4, 5));
    ptri2Dyn.Append(Poly_Triangle(4, 1, 5));

    //coy from dynamic
    Poly_Array1OfTriangle ptri1 = Poly_Array1OfTriangle(ptri1Dyn(0), 0, ptri1Dyn.Upper());
    Poly_Array1OfTriangle ptri2 = Poly_Array1OfTriangle(ptri2Dyn(0), 0, ptri2Dyn.Upper());
    //

    Handle(Poly_Triangulation) tria1 = new Poly_Triangulation(
        nodes1,
        ptri1
    );

    Handle(Poly_Triangulation) tria2 = new Poly_Triangulation(
        nodes2,
        ptri2
    );

    //
    Handle(AIS_Shape) aisPoint1 = new AIS_Shape(BRepBuilderAPI_MakeVertex(nodes1[0]));
    Handle(AIS_Shape) aisPoint2 = new AIS_Shape(BRepBuilderAPI_MakeVertex(nodes1[1]));
    Handle(AIS_Shape) aisPoint3 = new AIS_Shape(BRepBuilderAPI_MakeVertex(nodes1[2]));

    // Display points
    //myAISContext()->Display(aisPoint1, Standard_True);
    //myAISContext()->Display(aisPoint2, Standard_True);
    //myAISContext()->Display(aisPoint3, Standard_True);



    // Create polygon to represent triangulation
    Handle(AIS_Triangulation) tria1Ais = new AIS_Triangulation(tria1);
    Handle(TColStd_HArray1OfInteger) colors = new TColStd_HArray1OfInteger(0, 3);
    colors->ChangeValue(0) = Quantity_NOC_AQUAMARINE1;
    colors->ChangeValue(1) = Quantity_NOC_ALICEBLUE;
    colors->ChangeValue(2) = Quantity_NOC_ANTIQUEWHITE;
    colors->ChangeValue(3) = Quantity_NOC_BROWN;

    tria1Ais->SetColors(colors);
    Handle(AIS_Triangulation) tria2Ais = new AIS_Triangulation(tria2);
    //Handle(AIS_Shape) aisPolygon = new AIS_Shape(tria1);
    //tria2Ais->SetColors(colors);

    // Display polygon
    myAISContext()->Display(tria1Ais, Standard_True);
    myAISContext()->Display(tria2Ais, Standard_True);
    //myView()->FitAll();
    //myView()->Redraw();
    //myView()->MustBeResized();
    //

    //myAISContext()->AddOrRemoveCurrentObject(new AIS_Triangulation(tria1),false);
    //myAISContext()->AddOrRemoveCurrentObject(new AIS_Triangulation(tria2),false);
    //myAISContext()->DisplayAll(true);
    //Handle(AIS_Shape) tria1AisPrs = new AIS_Shape(tria1);


    //Handle(AIS_Triangulation) tria2Ais = new AIS_Triangulation(tria2);
    ////Handle(AIS_Shape) aShapePrs = new AIS_Shape(direct tria1);
    //myAISContext()->Display(tria1Ais, false);
    //myAISContext()->Display(tria2Ais, false);
    ////myViewer()->Redraw();
    ////myView()->Redraw();
    ////myAISContext()->Display(tria1Ais, AIS_Shaded, 0, false, AIS_DS_Displayed);
    //myAISContext()->UpdateCurrentViewer();
    //myView()->Update();
    //myAISContext()->UpdateCurrentViewer();
    //myView()->Redraw();
    //myView()->MustBeResized();
    //nodes.get()
    //myViewer()->pol
    //myAISContext()->AddOrRemoveCurrentObject(tria1, true);
    //myAISContext()->Display(tria1);

    // read the data and create a data source
    //Handle(Poly_Triangulation) aSTLMesh = RWStl::ReadFile(aFileName);
    //Handle(MeshVS_DataSource) aDataSource = new MeshVS_DataSource3D(tria1);

    //// create mesh
    //Handle(MeshVS_Mesh) aMeshPrs = new MeshVS_Mesh();
    //aMeshPrs->SetDataSource(aDataSource);

    //// use default presentation builder
    //Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMeshPrs);
    //aMeshPrs->AddBuilder(aBuilder, true);

    return true;
}
bool OCCTProxy::ImportBrep(System::String^ theFileName)
{
    return ImportBrep(toAsciiString(theFileName));
}
