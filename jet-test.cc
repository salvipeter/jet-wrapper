#include <fstream>

#include "jet-wrapper.hh"

using namespace Geometry;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.obj>" << std::endl;
    return 1;
  }
  auto mesh = TriMesh::readOBJ(argv[1]);
  const auto &points = mesh.points();
  auto jet = JetWrapper::fit(points, JetWrapper::Nearest(points));
  size_t n = points.size();

  for (const auto &tri : mesh.triangles()) {
    auto n = ((points[tri[1]] - points[tri[0]]) ^ (points[tri[2]] - points[tri[0]])).normalize();
    for (size_t i = 0; i < 3; ++i)
      if (n * jet[tri[i]].normal < 0)
        jet[tri[i]].normal *= -1;
  }

  std::ofstream f("output.vtk");
  f << "# vtk DataFile Version 2.0" << std::endl;
  f << "Mesh with principal curvature values & directions" << std::endl;
  f << "ASCII" << std::endl;
  f << "DATASET POLYDATA" << std::endl;
  f << "POINTS " << n << " float" << std::endl;
  for (size_t i = 0; i < n; ++i)
    f << points[i] << std::endl;
  f << "POLYGONS " << mesh.triangles().size() << ' ' << mesh.triangles().size() * 4 << std::endl;
  for (const auto &tri : mesh.triangles())
    f << "3 " << tri[0] << ' ' << tri[1] << ' ' << tri[2] << std::endl;
  f << "POINT_DATA " << n << std::endl;
  f << "NORMALS normal float" << std::endl;
  for (size_t i = 0; i < n; ++i)
    f << jet[i].normal << std::endl;
  f << "SCALARS k_max float 1" << std::endl;
  f << "LOOKUP_TABLE default" << std::endl;
  for (size_t i = 0; i < n; ++i)
    f << jet[i].k_max << std::endl;
  f << "SCALARS k_min float 1" << std::endl;
  f << "LOOKUP_TABLE default" << std::endl;
  for (size_t i = 0; i < n; ++i)
    f << jet[i].k_min << std::endl;
  f << "NORMALS d_max float" << std::endl;
  for (size_t i = 0; i < n; ++i)
    f << jet[i].d_max << std::endl;
  f << "NORMALS d_min float" << std::endl;
  for (size_t i = 0; i < n; ++i)
    f << jet[i].d_min << std::endl;
}
