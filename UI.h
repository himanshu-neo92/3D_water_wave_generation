namespace octet {

  class UI : public resource{

    text_overlay *overlay;
    /** @var  text_overlay *overlay
    *   @brief text overlay for the UI.
    */
    mesh_text *UI_top;
    /** @var   mesh_text *UI_top
    *   @brief text mesh for the top UI.
    */
    mesh_text *UI_bot;
    /** @var   mesh_text *UI_bot
    *   @brief text mesh for the bottom UI.
    */
    mesh_text *UI_popup;
    /** @var  mesh_text *UI_popup
    *   @brief text mesh for the popup UI.
    */
    mesh_text *target;
    /** @var  mesh_text *target
    *   @brief text mesh for the target UI.
    */

  public:
    int pop_up_var = 0;
    bool clear_popup = false;
    float temp_popup_time;
    float total_popup_time;
    
   

    UI() {
    }
    void initUI(int vx, int vy)
    {
     
      overlay = new text_overlay();
      // get the defualt font.
      bitmap_font *font = overlay->get_default_font();
      // create a box containing text (in pixels)
      aabb bb_bot(vec3(vx*19/32 , -500, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_bot = new mesh_text(font, "", &bb_bot);

      aabb bb_top(vec3(vx*6/8 , 200, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_top = new mesh_text(font, "", &bb_top);

      aabb bb_popup(vec3(vx*61/64, -240, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_popup = new mesh_text(font, "", &bb_popup);

      aabb bb_target(vec3(vx, -100, 0.0f), vec3(vx, 100.0f, 0.0f));
      target = new mesh_text(font, "+", &bb_target);
      // add the mesh to the overlay.

      overlay->add_mesh_text(UI_bot);
      overlay->add_mesh_text(UI_top);
      overlay->add_mesh_text(UI_popup);
      overlay->add_mesh_text(target);
    }
    /** @fn  void initUI()
    *   @brief This function is used to init the UI variables ie overlay textmesh....
    */

    void setup_pop_up(int num)
    {
      total_popup_time = 2.0f;
      temp_popup_time = 0.0f;
      pop_up_var = num;
      clear_popup = true;
    }
    void pop_up_clear()
    {
      if (clear_popup)
      {
        if (total_popup_time - temp_popup_time <= 0)
        {
          clear_popup = false;
          pop_up_var = 0;
        }
        else
        {
          temp_popup_time += 0.03f;
        }
      }
    }

    void updateUI(int vx, int vy, int rule_number, vec2 wave_vector[], float phase[], float amplitude[], float wavetime[],int frames,int selected_param,int selected_wave)
    {
      
      UI_bot->clear();
      UI_bot->format(
        
        "Press Up,Down keys to select the wave function, Press the Left,Right keys to select the parameter, Press delete,backspace keys to change the value of the parameter,\nPress P to save current configuration, and R to load configuration, Press F tto toggle Wireframe mode, Press G to toggle colour mode, Press Q to toggle normal mode");
      
      UI_bot->update();


      string current_param;
      if (selected_wave<8)
      {
        switch (selected_param)
      {
        case 0: current_param = "Wave Function X";
        break;
        case 1: current_param = "Wave Function Y";
        break;
        case 2: current_param = "Phase Difference";
        break;
        case 3:current_param = "Time For Wave";
        break;
        case 4:current_param = "Wave Constant";
        break;
      }
        UI_top->clear();
        UI_top->format(
          "Current Wave : %d, Current Parameter : %s, Wave Function X : %f, Wave Function Y : %f,\n              Phase Difference : %f, Time For Wave : %f, Wave Constant : %f", selected_wave, current_param, wave_vector[selected_wave].x(), wave_vector[selected_wave].y(), phase[selected_wave], wavetime[selected_wave], amplitude[selected_wave]);
      }
      else
      {
        if (selected_wave==8)//file to load
        {
          UI_top->clear();
          UI_top->format(
            "Current File : %d", rule_number);
        }
        else//animation frame
        {
          UI_top->clear();
          if (frames!=-1)
          UI_top->format(
            "Number of frames : %d", frames);
            else
              UI_top->format(
              "Number of frames : infinite loop");
        }      
      }      
      UI_top->update();


      UI_popup->clear();
      switch (pop_up_var)
      {
      case 0:UI_popup->format("");
        break;
      case 1:UI_popup->format("Saving configuration");

        break;
      case 2:UI_popup->format("Loading configuration");
        break;

      case 3:UI_popup->format("Rule Not Found");
        break;
      }
      UI_popup->update();
      // draw the text overlay
      overlay->render(vx, vy);
    }

  };
}
