namespace octet {


  /// Scene containing a box with octet.
  class colour_create : public resource {
    struct colour{
      unsigned char v[3];
      colour(unsigned char r, unsigned char g, unsigned char b){
        v[0] = r;
        v[1] = g;
        v[2] = b;
      }
    };

    colour lerp(colour c1, colour c2, float value){
      colour tcolor(0, 0, 0);

      for (int g = 0; g < 3; g++){
        if (c1.v[g] > c2.v[g]){
          tcolor.v[g] = c2.v[g] + (unsigned char)(float(c1.v[g] - c2.v[g])*value);
        }
        else{
          tcolor.v[g] = c1.v[g] + (unsigned char)(float(c2.v[g] - c1.v[g])*value);
        }
      }
      return (tcolor);
    }
  public :
   vec3 create_colour(float img, float max)
   {
     float diff = 2*max;

     colour waterlow(0, 0, 55), waterhigh(0, 0, 156); 
     colour pxl_colour(0, 0, 0);
     pxl_colour = lerp(waterlow, waterhigh, img/diff);
     vec3 mesh_colour = vec3((float)pxl_colour.v[0] / 255.0f, (float)pxl_colour.v[1] / 255.0f, (float)pxl_colour.v[2] / 255.0f);
     return mesh_colour;
   }
  };
  }