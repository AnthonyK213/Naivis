#ifndef _Naivis_Widget_OcctViewer_HeaderFile
#define _Naivis_Widget_OcctViewer_HeaderFile

#ifdef _WIN32
#include <windows.h>
#endif

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_ViewController.hxx>
#include <AIS_ViewCube.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRep_Tool.hxx>
#include <IMeshTools_Parameters.hxx>
#include <MeshVS_DataSource3D.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <MeshVS_MeshSelectionMethod.hxx>
#include <MeshVS_SelectionModeFlags.hxx>
#include <Message.hxx>
#include <OSD_Path.hxx>
#include <OpenGl_Context.hxx>
#include <OpenGl_FrameBuffer.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Poly_Triangulation.hxx>
#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

#include <QApplication>
#include <QColorSpace>
#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QStringList>

#include <NaiveDoc/NaiveDoc_Document.hxx>

class Widget_OcctViewer : public QOpenGLWidget, public AIS_ViewController {
  Q_OBJECT

public:
  Widget_OcctViewer(QWidget *theParent = nullptr);

  virtual ~Widget_OcctViewer();

  const Handle(V3d_Viewer) & Viewer() const { return myViewer; }

  const Handle(V3d_View) & View() const { return myView; }

  const Handle(AIS_InteractiveContext) & Context() const { return myContext; }

  /// @return OpenGL informations.
  const QString &getGlInfo() const { return myGlInfo; }

  /// @return Widget minimum size.
  virtual QSize minimumSizeHint() const override { return QSize(200, 200); }

  /// @return Widget default size.
  virtual QSize sizeHint() const override { return QSize(720, 480); }

  /// @brief Display the viewCube
  void displayViewCube();

protected:
  virtual void initializeGL() override;

  virtual void paintGL() override;

  // virtual void resizeGL(int , int ) override;

protected:
  virtual void closeEvent(QCloseEvent *theEvent) override;

  virtual void keyPressEvent(QKeyEvent *theEvent) override;

  virtual void mousePressEvent(QMouseEvent *theEvent) override;

  virtual void mouseReleaseEvent(QMouseEvent *theEvent) override;

  virtual void mouseMoveEvent(QMouseEvent *theEvent) override;

  virtual void wheelEvent(QWheelEvent *theEvent) override;

private:
  void dumpGlInfo(bool theIsBasic, bool theToPrint);

  void updateView();

  virtual void handleViewRedraw(const Handle(AIS_InteractiveContext) & theCtx,
                                const Handle(V3d_View) & theView) override;

  qreal screenScale() const;

signals:
  void selectionChanged(QStringList theNameList);

private:
  Handle(V3d_Viewer) myViewer;
  Handle(V3d_View) myView;
  Handle(AIS_InteractiveContext) myContext;
  Handle(AIS_ViewCube) myViewCube;
  QString myGlInfo;
  Standard_Boolean myIsCoreProfile;
  
  Handle(NaiveDoc_Document) myDoc;
};

#endif
