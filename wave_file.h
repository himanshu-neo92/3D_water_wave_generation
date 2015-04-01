namespace octet {

  class wave_file
  {
    FILE *wave_Rules_r;
    FILE *wave_Rules_w;
  public :
    wave_file()
    {
      wave_Rules_r = fopen("water_param.txt", "r");
      wave_Rules_w = fopen("water_param.txt", "a");
    }
    ~wave_file()
    {
      fclose(wave_Rules_r);
      fclose(wave_Rules_w);
    }

   bool getrule(int rule_number,vec2 wave_vector[],float phase[],float amplitude[],float wavetime[])
    {
      char str[200];
      int current_rule_number = 0;      
      int curr_wave=0;
      float tempx;
      float tempy;
      
      float divfac = 10.0f;

      fseek(wave_Rules_r, 0, 0);
      while (!feof(wave_Rules_r))
      {
        char initsym = fgetc(wave_Rules_r);//chk for the symbols
        switch (initsym)
        {
        case 'X':
          initsym = fgetc(wave_Rules_r);//remove the :

          if (current_rule_number == rule_number)
          {
            fscanf(wave_Rules_r, "%s", str);
            int lenofarr = strlen(str);
            tempx=0.0f;
            divfac=10.0f;
            int i=0;
            for (; str[i] != '.'; i++)
            {
              tempx += (str[i] - 48);
              if (str[i + 1] != '.')
                tempx *= 10;
            }
            i++;
            for (; i<lenofarr; i++)
            {
              tempx += (str[i] - 48) / divfac;
              divfac*=10.0f;
            }  
            
            wave_vector[curr_wave] = vec2(tempx,wave_vector[curr_wave].y());
          }
          else
          {
            fscanf(wave_Rules_r, "%s", str);
           
          }
          initsym = fgetc(wave_Rules_r);//remove the \n
          break;

        case 'Y':
          initsym = fgetc(wave_Rules_r);//remove the :

          if (current_rule_number == rule_number)
          {
            fscanf(wave_Rules_r, "%s", str);
            int lenofarr = strlen(str);
            tempy = 0.0f;
            divfac = 10.0f;
            int i = 0;
            for (; str[i] != '.'; i++)
            {
              tempy += (str[i] - 48);
              if (str[i + 1] != '.')
                tempy *= 10;
            }
            i++;
            for (; i<lenofarr; i++)
            {
              tempy += (str[i] - 48) / divfac;
              divfac *= 10.0f;
            }
            wave_vector[curr_wave] = vec2(wave_vector[curr_wave].x(), tempy);
          }
          else
          {
            fscanf(wave_Rules_r, "%s", str);

          }
          initsym = fgetc(wave_Rules_r);//remove the \n
          break;

        case 'T':
          initsym = fgetc(wave_Rules_r);//remove the :

          if (current_rule_number == rule_number)
          {
            fscanf(wave_Rules_r, "%s", str);
            int lenofarr = strlen(str);
            wavetime[curr_wave]=0.0f;
            divfac = 10.0f;
            int i = 0;
            for (; str[i] != '.'; i++)
            {
              wavetime[curr_wave] += (str[i] - 48);
              if (str[i + 1] != '.')
                wavetime[curr_wave] *= 10;
            }
            i++;
            for (; i<lenofarr; i++)
            {
              wavetime[curr_wave] += (str[i] - 48) / divfac;
              divfac *= 10.0f;
            }            
          }
          else
          {
            fscanf(wave_Rules_r, "%s", str);

          }
          initsym = fgetc(wave_Rules_r);//remove the \n
          break;
        
        case 'P':
          initsym = fgetc(wave_Rules_r);//remove the :

          if (current_rule_number == rule_number)
          {
            fscanf(wave_Rules_r, "%s", str);
            int lenofarr = strlen(str);
            phase[curr_wave] = 0.0f;
            divfac = 10.0f;
            int i = 0;
            for (; str[i] != '.'; i++)
            {
              phase[curr_wave] += (str[i] - 48);
              if (str[i + 1] != '.')
                phase[curr_wave] *= 10;
            }
            i++;
            for (; i<lenofarr; i++)
            {
              phase[curr_wave] += (str[i] - 48) / divfac;
              divfac *= 10.0f;
            }
          }
          else
          {
            fscanf(wave_Rules_r, "%s", str);

          }
          initsym = fgetc(wave_Rules_r);//remove the \n
          break;

        case 'A':
          initsym = fgetc(wave_Rules_r);//remove the :

          if (current_rule_number == rule_number)
          {
            fscanf(wave_Rules_r, "%s", str);
            int lenofarr = strlen(str);
            amplitude[curr_wave] = 0.0f;
            divfac = 10.0f;
            int i = 0;
            for (; str[i] != '.'; i++)
            {
              amplitude[curr_wave] += (str[i] - 48);
              if (str[i + 1] != '.')
                amplitude[curr_wave] *= 10;
            }
            i++;
            for (; i<lenofarr; i++)
            {
              amplitude[curr_wave] += (str[i] - 48) / divfac;
              divfac *= 10.0f;
            }
          }
          else
          {
            fscanf(wave_Rules_r, "%s", str);

          }
          initsym = fgetc(wave_Rules_r);//remove the \n
          break;

        case '/':
          char temp1 = fgetc(wave_Rules_r);
          temp1 = fgetc(wave_Rules_r);
          if (temp1 == '/'){
            fscanf(wave_Rules_r, "%s", str);
            initsym = fgetc(wave_Rules_r);
          current_rule_number++;
          }  
          else
          {
            curr_wave++;
          }
          if (current_rule_number>rule_number)
          {
            return true;
          }
          break;
        }

      }
      return false;
    }


    void putrule(vec2 wave_vector[], float phase[], float amplitude[], float wavetime[])
    {
      fseek(wave_Rules_w, 0,SEEK_END);
      fprintf(wave_Rules_w, "\n");
      for (int i=0;i<8;i++)
      {
        fprintf(wave_Rules_w, "X:%f\n", wave_vector[i].x());
        fprintf(wave_Rules_w, "Y:%f\n", wave_vector[i].y());
        fprintf(wave_Rules_w, "T:%f\n", wavetime[i]);
        fprintf(wave_Rules_w, "P:%f\n", phase[i]);
        fprintf(wave_Rules_w, "A:%f\n", amplitude[i]);
        fprintf(wave_Rules_w, "//");
        if (i==7)
        {
          fprintf(wave_Rules_w, "///");
        }
        else
        {
          fprintf(wave_Rules_w, "\n");
        }
      }
    }
  };

}