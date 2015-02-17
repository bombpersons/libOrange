extern "C" {
  #include <planet.h>
}
#include <glm/glm.hpp>
#include <sstream>

bool GenerateTerrain(double _seed,
  const glm::uvec2& _resolution,
  const glm::vec2& _top, const glm::vec2& _bottom,
  float _altitude,
  unsigned char* _data
  ) {

  // Okay we need to call the generate function and then
  // get the results into a sane format.
  
  // Create the arguments
  char seed[32];
  sprintf(seed, "%.10f", _seed);

  char resolutionX[32];
  sprintf(resolutionX, "%u", _resolution.x);

  char resolutionY[32];
  sprintf(resolutionY, "%u", _resolution.y);

  // We need to figure out how exactly to get the area we want.
  // For now just don't pass in anything..
  char latitude[32];
  char longitude[32];
  char magnification[32];

  // Altitude
  char altitude[32];
  sprintf(altitude, "%f", _altitude);

  char* args[] = {
    "", // This should be the exe location.
    "-s", seed,
    "-w", resolutionX,
    "-h", resolutionY,
    //"-m", (char*)(magnification.str().c_str()),
    //"-l", (char*)(latitude.str().c_str()),
    //"-L", (char*)(longitude.str().c_str()),
    "-i", altitude,
    "-p", "c"
  };

  // Okay, call the function
  return generate_map(sizeof(args) / sizeof(char*), args, _data) == 0; // return true if succesful.
}
