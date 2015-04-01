#include<math.h>
#include<random>
namespace octet {

#define width_image 128
#define height_image 128
  class wave_create : public resource {
    
    const float _4pisq = 39.4784176044f;
    const float g_earth = 9.8;    
    
    float omega[8];
    float wave_const[8];

  public :
    

    vec3 wave_gen(vec3 pos, int nu_K, vec2 k_param[],float k_phi[], float wave_amp[],float timeforwave[], float time, bool &timeforwavechanged,int nu_changed , bool &k_paramchanged, int nu_k_param,float &max)
    {
      vec2 horplane = vec2(0,0);
      float height =0.0f;
      if (timeforwavechanged)
      {
        if (nu_changed=-1)
        {
                 
          for (int i=0;i<nu_K;i++)
          {                        
            omega[i] = sqrt((_4pisq / (timeforwave[i] * timeforwave[i])));
          }
        }
        else
        {
          omega[nu_changed] = sqrt((_4pisq / (timeforwave[nu_changed] * timeforwave[nu_changed])));
        }
        timeforwavechanged=false;
      }
      
      if (k_paramchanged)
      {
        
        if (nu_changed = -1)
        {
          for (int i = 0; i<nu_K; i++)
          {
            wave_const[i] = wave_amp[i]/k_param[i].length();
            max += wave_const[i];           
          }
        }
        else
        {
          max -= wave_const[nu_changed];          
          wave_const[nu_changed] = wave_amp[nu_changed] / k_param[nu_changed].length();
          max += wave_const[nu_changed];         
        }
        k_paramchanged = false;
      }

      for (int i=0;i<nu_K;i++)
      {
        height += wave_const[i] * cosf(dot(k_param[i],vec2(pos.x(),pos.z()) - omega[i]*time + k_phi[i]));
        horplane+=  (k_param[i]/k_param[i].length())*wave_const[i] * sinf(dot(k_param[i], vec2(pos.x(), pos.z()) - omega[i] * time + k_phi[i]));        
      }


      return vec3(pos.x()-horplane.x(),height,pos.z()-horplane.y());    
    }
  };
  
  }