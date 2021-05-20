// Wrapper library over CGAL for convenient use with libgeom

#pragma once

#include <geometry.hh>

namespace JetWrapper {

using namespace Geometry;

// radius = 0                     means   KNN search
// radius > 0 and neighbors = 0   means   unlimited search in a fuzzy sphere
// radius > 0 and neighbors > 0   means   fuzzy sphere search with limited # of elements
class Nearest {
public:
  Nearest(const PointVector &points, size_t neighbors = 20, double radius = 0);
  ~Nearest();
  PointVector operator()(const Point3D &p) const;

private:
  const PointVector &points;
  size_t neighbors;
  double radius;
  void *tree; // to avoid a bunch of CGAL includes
};

struct JetData {
  Vector3D normal;       // Unit normal vector
  Vector3D d_min, d_max; // Principal curvature directions
  double k_min, k_max;   // Principal curvature values
};

std::vector<JetData> fit(const PointVector &points, const Nearest &nearest, size_t degree = 2);

// Example usage:
//   auto jet = JetWrapper::fit(points, JetWrapper::Nearest(points));
// Now jet[i].normal is the normal vector at points[i] etc.

}
