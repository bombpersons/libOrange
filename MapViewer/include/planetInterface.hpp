#include <glm/glm.hpp>

bool GenerateTerrain(double _seed,
  const glm::uvec2& _resolution,
  const glm::vec2& _top, const glm::vec2& _bottom,
  float _altitude,
  unsigned char* _data
  );