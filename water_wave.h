////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene containing a box with octet.
  struct my_vertex {
    vec3p pos;
    vec3p nor;
    uint32_t color;
  };
  static uint32_t make_color(float r, float g, float b) {
    return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
  }
  class water_wave : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

    param_shader *shader;
    material *mat_water;
    mesh *water_wave_mesh;
    size_t num_vertices;
    size_t num_indices;
    my_vertex *vtx;
    uint32_t *idx;
    scene_node *water_wave_mesh_node;
   

    wave_create wavefnc;
    colour_create colourfnc;
    inputs inputs;
    UI Game_UI;
    sky_box Sky_Box;

    wave_file file_handler;
    float time_wave=0.0f;


    int number_of_frames = -1;
    int current_wave = 0;
    int current_param = 0;
    int rule_number =0;
    bool isfile_loaded;

    vec2 tempparam[8];
    float tempphase[8];
    float tempwaveamp[8];
    float temptimewave[8];

    int currframe_nu;
    int tempframe_nu = 0;

    bool isadded = false;
    bool timeforwavechanged = true;
    bool paramchanged = true;

    bool istextured=true;
    bool isnormal=true;
    bool iswireframe=false;
    bool is1st = true;

    void setup_camera()
    {
      mat4t &camera_mat = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      camera_mat.translate(150, 150, 150);
      camera_mat.rotateY(-90);
      camera_mat.rotateX(0);
    }
    void input_keys()
    {
      if (inputs.F_KEY())
      {
        iswireframe = !iswireframe;
      }
      
      if (inputs.Q_KEY())
      {
        isnormal =!isnormal;
      }
      if (inputs.G_KEY())
      {
        istextured=!istextured;
      }
      if (inputs.P_KEY())
      {  
        file_handler.putrule(tempparam, tempphase, tempwaveamp, temptimewave);
        Game_UI.setup_pop_up(1);
      }
      if (inputs.R_KEY())
      {        
        isfile_loaded = file_handler.getrule(rule_number, tempparam, tempphase, tempwaveamp, temptimewave);
        if (isfile_loaded)
        {
          Game_UI.setup_pop_up(2);
          is1st = true;
        }
        else
        {
          Game_UI.setup_pop_up(3);
        }
      }
      if (inputs.PLUS_KEY())
      {
        if (current_wave<8)
        {
          switch (current_param)
          {
          case 0: tempparam[current_wave] = vec2(tempparam[current_wave].x() + 0.1f, tempparam[current_wave].y());
            paramchanged = true;
            break;
          case 1: tempparam[current_wave] = vec2(tempparam[current_wave].x(), tempparam[current_wave].y() + 0.1f);
            paramchanged = true;
            break;
          case 2: tempphase[current_wave] += 0.5f;
            break;
          case 3:temptimewave[current_wave] += 0.1f;    
            timeforwavechanged=true;
            break;
          case 4:tempwaveamp[current_wave] += 0.05f;    
            paramchanged = true;
            break;
          }
        }
        else
        {
          if (current_wave == 8)
          {            
              rule_number++;
            
          }
          else
          {           
              number_of_frames++;            
          }
        }
      }
      if (inputs.MINUS_KEY())
      {
        if (current_wave<8)
        {
          switch (current_param)
          {
          case 0: tempparam[current_wave] = vec2(tempparam[current_wave].x() - 0.1f, tempparam[current_wave].y());
            paramchanged = true;
            break;
          case 1: tempparam[current_wave] = vec2(tempparam[current_wave].x(), tempparam[current_wave].y() - 0.1f);
            paramchanged = true;
            break;
          case 2: tempphase[current_wave]-=0.5f;
            break;
          case 3: if (temptimewave[current_wave]>0.2f)
                   { 
                  temptimewave[current_wave]-=0.1f;
                  timeforwavechanged = true;
                   }
            break;
          case 4:if (tempwaveamp[current_wave]>0.1f)
                  {
                  tempwaveamp[current_wave]-=0.05f;
                  paramchanged = true;
                  }
            break;
          }
        }
        else
        {
          if (current_wave==8)
          {
            if (rule_number > 0)
            {
              rule_number--;
            }
          }
          else
          {
            if (number_of_frames > -1)
            {
              number_of_frames--;
            }
          }
        }
      }
      if (is_key_going_up(key::key_right))
      {
        if (current_wave<8)
        {
          if (current_param<4)
          {
            current_param++;
          }
          else
          {
            current_param = 0;
          }
        }
      }
      if (is_key_going_up(key::key_left))
      {
        if (current_wave<8)
        {
          if (current_param>0)
          {
            current_param--;
          }
          else
           {
            current_param = 4;
           }
        }
      }
      if (is_key_going_up(key::key_up))
      {
        if (current_wave<9)
        {
          current_wave++;
        }
        else
        {
          current_wave=0;
        }
      }
      if (is_key_going_up(key::key_down))
      {
        if (current_wave>0)
        {
          current_wave--;
        }
        else
        {
          current_wave = 9;
        }
      }
    }   

  public:   
    /// this is called when we construct the class before everything is initialised.
    water_wave(int argc, char **argv) : app(argc, argv) {
    }

    ~water_wave()
    {
      
      delete shader;
      delete mat_water;
      delete water_wave_mesh;
      delete water_wave_mesh_node;
    }
    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();      
      app_scene->get_camera_instance(0)->set_far_plane(10000);
      setup_camera();

      
      Sky_Box.InitDayNightCycle(app_scene);
      inputs.init(this);
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      Game_UI.initUI(vx,vy);

      shader = new param_shader("shaders/default.vs", "shaders/simple_color.fs");
      mat_water = new material(vec4(0.6f, 0.298f, 0.0f, 1.0f), shader);
      water_wave_mesh = new mesh();
      num_vertices = height_image * width_image;
      num_indices = 6 * (num_vertices - height_image) - width_image;
      water_wave_mesh->allocate(sizeof(my_vertex)* num_vertices, sizeof(uint32_t)* num_indices);
      water_wave_mesh->set_params(sizeof(my_vertex), num_indices, num_vertices, GL_TRIANGLES, GL_UNSIGNED_INT);
      water_wave_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
      water_wave_mesh->add_attribute(attribute_normal, 3, GL_FLOAT, 12);
      water_wave_mesh->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 24, GL_TRUE);

      water_wave_mesh_node = new scene_node();
      app_scene->add_child(water_wave_mesh_node);
      tempframe_nu = get_frame_number();


      isfile_loaded=file_handler.getrule(0, tempparam, tempphase, tempwaveamp, temptimewave);
      
     generate_wave(0.1f,istextured,isnormal,iswireframe);
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);
      // draw the scene
      app_scene->render((float)vx / vy);


      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      inputs.key_presses(camera);
      inputs.mouse_control(camera);

      Sky_Box.DayNightCycle(app_scene);
      Game_UI.updateUI(vx, vy, rule_number, tempparam, tempphase, tempwaveamp, temptimewave, number_of_frames, current_param, current_wave);
      Game_UI.pop_up_clear();
      input_keys();

      currframe_nu=get_frame_number();
      if (currframe_nu == tempframe_nu+3)
      {
      time_wave+=0.1f;      
      if (number_of_frames!=-1)
      {
        if(time_wave > number_of_frames*10)
        {
          time_wave=0.0f;
        }
      }
      generate_wave(time_wave, istextured, isnormal, iswireframe);
      tempframe_nu = get_frame_number();
      }
    }
  
   float max=0.0f;
    void generate_wave(float frame,bool istextured, bool isnormal, bool iswireframe)
    {      
      if (isadded)
      {
        app_scene->pop_mesh_instance();
      }      
      gl_resource::wolock vl(water_wave_mesh->get_vertices());
      vtx = (my_vertex *)vl.u8();
      gl_resource::wolock il(water_wave_mesh->get_indices());
      idx = idx = il.u32();
      if (iswireframe)
      {
        water_wave_mesh->set_params(sizeof(my_vertex), num_indices, num_vertices, GL_LINES, GL_UNSIGNED_INT);
      }
      else
      {
        water_wave_mesh->set_params(sizeof(my_vertex), num_indices, num_vertices, GL_TRIANGLES, GL_UNSIGNED_INT);
      }
      for (int i = 0; i < height_image; ++i) {     // z
        for (int j = 0; j < width_image; ++j) { // x 
          vec3 new_pos;
          if (!is1st)
          {
          new_pos = wavefnc.wave_gen(vec3(i, 0.0f, j), 8, tempparam, tempphase, tempwaveamp, temptimewave, frame, timeforwavechanged, current_wave, paramchanged, current_wave,max);
            }
            else
            {
              new_pos = wavefnc.wave_gen(vec3(i, 0.0f, j), 8, tempparam, tempphase, tempwaveamp, temptimewave, frame, timeforwavechanged, -1, paramchanged, -1,max);
              is1st=false;
            }
            
            if (istextured)
            {
              vec3 mesh_colour = colourfnc.create_colour(new_pos.y(),max);
              vtx->color = make_color(mesh_colour.x(), mesh_colour.y(), mesh_colour.z());
            }
            else
            {vtx->color = make_color(.5f, 0.5f, 0.5f);}    

            vtx->pos = vec3p(new_pos);
            if (isnormal)
            {vtx->nor = vec3p(new_pos);}//making normals 
            else
            {vtx->nor = vec3p(vec3(0,0,0));}
            vtx++;           
          }
        }       
      int tempvert = 0;
      for (int i = 0; i < height_image - 1; ++i) {
        for (int j = 0; j < width_image - 1; ++j) {

          idx[0] = tempvert;
          idx[1] = tempvert + 1;
          idx[2] = tempvert + width_image + 1;
          idx += 3;

          idx[0] = tempvert;
          idx[1] = tempvert + width_image + 1;
          idx[2] = tempvert + width_image;
          idx += 3;

          tempvert++;
        }
        tempvert++;
      }    
            
      app_scene->add_mesh_instance(new mesh_instance(water_wave_mesh_node, water_wave_mesh, mat_water));
      isadded=true;
      
    }
    
  };
}
