#include<math.h>
#include<random>
namespace octet {

#define width_image 128
#define height_image 128
  class wave_create : public resource {    
    
    const float g_earth = 9.8;    
    
    float omega[8];
    float wave_const[8];

  public :
    

    vec3 wave_gen(vec3 pos, vec3 &norm,bool isnorm,int nu_K, vec2 k_param[],float k_phi[], float wave_amp[], float time, bool &timeforwavechanged,int nu_changed , bool &k_paramchanged, int nu_k_param,float &max)
    {
      vec2 horplane = vec2(0,0);
      vec2 _norm = vec2(0,0);
      float height =0.0f;
      if (timeforwavechanged)
      {
        if (nu_changed=-1)
        {
                 
          for (int i=0;i<nu_K;i++)
          {                        
            omega[i] = sqrt(g_earth*k_param[i].length());
          }
        }
        else
        {
          omega[nu_changed] = sqrt(g_earth*k_param[nu_changed].length());
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
        if (isnorm)
        {
        _norm += (k_param[i])*wave_const[i] * cosf(dot(k_param[i], vec2(pos.x(), pos.z()) - omega[i] * time + k_phi[i]));
        }
      }
      if (isnorm)
      {
      _norm.normalize();
      norm = vec3(_norm.x(),1,_norm.y());
      }
      return vec3(pos.x()-horplane.x(),height,pos.z()-horplane.y());    
    }
  };
  
  }