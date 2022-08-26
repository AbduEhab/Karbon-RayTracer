#pragma once

#include "Constants.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "FileOperations.hpp"

#include "Matrix.hpp"

#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

#include "Lights/Light.hpp"
#include "Lights/PointLight.hpp"

#include "Intersection.hpp"

#include "Shapes/Cube.hpp"
#include "Shapes/Shape.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/XYPlane.hpp"
#include "Shapes/XZPlane.hpp"
#include "Shapes/YZPlane.hpp"

#include "Patterns/Pattern.hpp"

#include "Material.hpp"

#include "Computation.hpp"

#include "Camera.hpp"
#include "World.hpp"

#include "Scene.hpp"