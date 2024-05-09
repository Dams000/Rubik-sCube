#include "utils.h"

bool colorsEqual(Color color1, Color color2) {
 return color1.a == color2.a && color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}
