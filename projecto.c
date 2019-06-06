#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

double   dt = 0;
double   posxb = 500, posyb = 200, pb;
double   posxm = 450, posym = 500, pm;
double   yb[401], ym[401], vbola[401], vmola[401] ;
double   v=10, vm=10, vi, vmi;
double   m=2, M=15, k=20;
double   t, erroescala, desenhomola, var;
gint     a, i, c=0, ct=1, grafp1=1, grafp2=1, opcao=1, grafv1=1, grafv2=1, w=1 ,r, eps;
GtkWidget *scale;
GtkWidget  *spin1, *spin2, *spin3, *spin4,*spin5;
GtkAdjustment *adj, *adj1, *adj2, *adj3, *adj4, *adj5;
GtkWidget  *check1, *check2,*grafico1,*grafico2;


void about (GtkWidget *widget, GtkWidget *win)
{
GtkWidget *dialog;

  dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG(dialog), "(c) Filipe Novais & Filipe Sousa");
  gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG(dialog), "OSCILADOR DE FERMI");
  gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(dialog),  "http://arjuna.ist.utl.pt/IC/");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}


void
quick_message (GtkWidget *w, GtkWidget *win) 
 
{

  GtkWidget *dialog, *label;
  gchar *message = "Deseja sair?";
  gint  resultado ;
      

  dialog = gtk_dialog_new_with_buttons ("Sair",
					GTK_WINDOW(win),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_STOCK_YES,
					GTK_RESPONSE_YES,
					GTK_STOCK_NO,
					GTK_RESPONSE_NO,
					NULL);
  gtk_window_set_default_size (GTK_WINDOW(dialog), 200, 100);
  gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
  label = gtk_label_new (message);
   

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
  gtk_widget_show_all (dialog);

  resultado = gtk_dialog_run (GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  if (resultado == GTK_RESPONSE_YES)
    gtk_main_quit();
 
}

void checkRadioButtons()
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(grafico1))==TRUE)
    r =1;
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(grafico2))==TRUE)
    r =2;
}

static void checkgraf1 (GtkWidget *widget, gpointer data)
{
  if(grafp1==1)
    grafp1=-1;
  else 
    grafp1=1;
}

static void checkgraf2 (GtkWidget *widget, gpointer data)
{
  if(grafp2==1)
    grafp2=-1;
  else 
    grafp2=1;
}

static void velocidadetempo (GtkWidget *widget, gpointer data)
{
  if(var==-1)
    {
      checkgraf1(NULL,NULL);
      checkgraf2(NULL,NULL); 
      var=1;
    }
}


static void posicaotempo (GtkWidget *widget, gpointer data)
{
  if(var==1)
    {
      checkgraf1(NULL,NULL);
      checkgraf2(NULL,NULL);     
      var=-1;
    }
}


gboolean slow1 (GtkWidget      *widget ,
		GdkEventExpose  *event ,
		gpointer         data   )
{
  dt = 0.00175;
  w=-1;
  return dt;
}

gboolean play1 (GtkWidget      *widget ,
		GdkEventExpose  *event ,
		gpointer         data   )
{
  
  if(dt==0.0035||dt==0.00175||dt==0.0070)
    dt = 0.0035;
  w=-1;
  return dt;
}


gboolean fast1 (GtkWidget      *widget ,
		GdkEventExpose  *event ,
		gpointer         data   )
{
  dt = 0.0070;
  w=-1;
  return dt;
}


gboolean
start_stop (GtkWidget      *widget ,
	    GdkEventExpose  *event ,
	    gpointer         data   )
{
  if(w==-1)
    {
      if(dt==0.0035||dt==0.00175||dt==0.007)
	{
	  dt = 0;
	  w = 1;
	}
      else
	dt = 0.0035;
    }
  if(w==1)
    {
      if(dt==0.0035)
	dt=0;
      else
	dt=0.0035;
      w=-1;
    }

  if(ct==1)
    {
      gtk_button_set_label(GTK_BUTTON(widget), "STOP");
      ct=-1;
    }
  else
    {
      ct=1;
      gtk_button_set_label(GTK_BUTTON(widget), "START");
    }
  k  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj1));
  m  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj2));
  M  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj3));
 
  return dt;
}

static void reset_positions (GtkWidget *widget, gpointer data)
{
  posyb = 200;
  posym = 500;
  pb = 600 - posyb;
  pm = 600 - posym;
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj), pb);
}
static void reset_graph (GtkWidget *widget, gpointer data)
{
  c=0;
}

static void reset (GtkWidget *widget, gpointer data)
{
  c     = 0;
  t     = 0;
  posyb = 200;
  posym = 500;
  k     = 20;
  m     = 2;
  M     = 10;
  v     = 10;
  vm    = 10;
  pb = 600 - posyb;
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj), pb);
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj1), k);
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj2), m);
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj3), M);
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj4), v);
  gtk_adjustment_set_value (GTK_ADJUSTMENT(adj5), vm);
}

static void change_scale (GtkWidget *widget, gpointer data)
{
  double val;
  val = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj));
  posyb = 600-val;
}

static void aplicar(GtkWidget *widget, gpointer data)
{
  k  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj1));
  m  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj2));
  M  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj3));
  v  = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj4));
  vm = gtk_adjustment_get_value (GTK_ADJUSTMENT(adj5));
}

gboolean
on_expose_event (GtkWidget       *widget ,
		 GdkEventExpose  *event  ,
		 gpointer         data   )
{
  cairo_t   *cr ;
  gint      width, height;
  gchar     tempo[1000], valposyb[1000], valposym[1000], valv[1000], valvm[1000];


  gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
  cr = gdk_cairo_create(widget->window);

  //Erro escala
  pm = 600 - posym + 20;
  erroescala=gtk_range_get_value(GTK_RANGE(scale));
  if(erroescala<pm+4)
    gtk_range_set_value(GTK_RANGE(scale),pm+4);


  // Erro da bola
  if(posyb+25 >= posym)
    posyb=posyb-1;
 
  //eixo_vertical
  cairo_move_to (cr, 415, 15);
  cairo_set_font_size (cr, 12);
  cairo_show_text (cr, "Posiçao (pixel's)");
  cairo_set_source_rgb (cr, 1, 0, 0);
  cairo_move_to(cr, 400, 0);
  cairo_line_to(cr, 400, 600);
  cairo_stroke(cr);
  cairo_move_to(cr, 390, 20);  
  cairo_line_to(cr, 400, 0);
  cairo_stroke(cr);
  cairo_move_to(cr, 410, 20);
  cairo_line_to(cr, 400, 0);
  cairo_stroke(cr);
  for (i=25; i<600 ; i=i+25)
    {
      cairo_set_line_width (cr, 3);
      cairo_move_to (cr, 392, i);
      cairo_line_to (cr, 409, i);
    }
  cairo_stroke(cr);
  
  //bola
  cairo_set_source_rgb (cr, 1, 0, 1.);
  cairo_arc (cr, posxb, posyb, 20, 0, 2. * M_PI);
  cairo_stroke_preserve (cr);
  cairo_fill (cr);

  //rectangulo azul
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 4.0);
  cairo_rectangle (cr, posxm, posym, 100, 10);
  cairo_stroke_preserve (cr);
  cairo_set_source_rgb (cr, 0, 0, 1);
  cairo_fill (cr);
  cairo_stroke(cr);

  //rectangulo branco
  cairo_rectangle (cr, 35, 331, 300, 60);
  cairo_stroke_preserve (cr);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_fill (cr);
  cairo_stroke(cr);

  //mola
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 2.5);

  cairo_move_to(cr,460,posym+12);
  desenhomola=(600-posym)/9;

  for(i=0;i<9;i++)
    {
      cairo_line_to(cr,540,(posym+12+i*desenhomola));
      cairo_line_to(cr,460,(posym+12+i*desenhomola));
    }
  cairo_stroke(cr);

  //Equacoes
  if(ct!=1)
    {
  
      for(i=0; i<100; i++)
	{
	  t=t+dt;

	  //colisao
	  if ((posyb+25) > posym)
	    {
	      vi = v;
	      vmi = vm;
	      
	      vm=( ((2*m)/(m+M))*vi + ((M-m)/(m+M))*vmi );
	      v=( ((m-M)/(m+M))*vi + ((2*M)/(m+M))*vmi );
	    }
	
	  v = v + 9.8 * dt;
	  posyb = posyb + v * dt;
	  
	  vm= vm-(posym-500)*(k/M)*dt;
	  posym = posym + vm * dt;
	}
    }

  //EIXOS

 
    
  //X
      
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1.25);
      
  for (i=25; i<426 ; i=i+5)
    {
      cairo_set_line_width (cr, 3);
      cairo_move_to (cr, 586, i);
      cairo_line_to (cr, 592, i);
    }
  cairo_stroke(cr);

  for (i=25; i<425 ; i=i+25)
    {
      cairo_set_line_width (cr, 3);
      cairo_move_to (cr, 582, i);
      cairo_line_to (cr, 596, i);
    }
  cairo_stroke(cr);
      
  cairo_move_to (cr, 950, 407);
  cairo_set_font_size (cr, 12);
  cairo_show_text (cr, "TEMPO");

  if(r==1)
    {
      //Y
	  
      for (i=25; i<400 ; i=i+5)
	{
	  cairo_set_line_width (cr, 3);
	  cairo_move_to (cr, 570+i, 426);
	  cairo_line_to (cr, 570+i, 418);
	}
      cairo_stroke(cr);

      for (i=25; i<400 ; i=i+25)
	{
	  cairo_set_line_width (cr, 3);
	  cairo_move_to (cr, 570+i, 430);
	  cairo_line_to (cr, 570+i, 412);
	}
      cairo_stroke(cr);

      cairo_move_to (cr, 597, 35);
      cairo_set_font_size (cr, 12);
      cairo_show_text (cr, "POSICAO");
    }
  
  if(r==2)
    {
      //Y
      
      for (i=25; i<400 ; i=i+5)
	{
	  cairo_set_line_width (cr, 3);
	  cairo_move_to (cr, 570+i, 246);
	  cairo_line_to (cr, 570+i, 238);
	}
      cairo_stroke(cr);
      
      for (i=25; i<400 ; i=i+25)
	{
	  cairo_set_line_width (cr, 3);
	  cairo_move_to (cr, 570+i, 249);
	  cairo_line_to (cr, 570+i, 234);
	}
      cairo_stroke(cr);

      cairo_move_to (cr, 597, 35);
      cairo_set_font_size (cr, 12);
      cairo_show_text (cr, "VELOCIDADE");
    }
      

  //GRAFICOS
  if(ct!=1)
    c++;
  if(c>400)
    c=0;

  
  checkRadioButtons();

  //grafico da bola
  yb[c]=posyb;

  if((grafp1==1)&&(r==1))
    {
      cairo_set_source_rgb (cr, 1, 0, 1.);
      cairo_move_to (cr, 590, yb[0]*0.7); 
      for(i=0;i<c-2;i++)
	{
	  cairo_line_to (cr, 590+i, yb[i+1]*0.7);
	}
      cairo_stroke(cr);
      cairo_arc (cr, 590+c-3, yb[c-1]*0.7, 3, 0, 2. * M_PI);
      cairo_stroke_preserve (cr);
      cairo_fill (cr);
      cairo_stroke(cr);
    
      cairo_set_source_rgb(cr, 1, 0, 1);
      cairo_select_font_face (cr, "Times New Roman", 			 
			      CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
      cairo_set_font_size (cr, 20);
      cairo_move_to (cr, 800, 470);
      sprintf(valposyb, "Posicao bola = %3.1lf ", 600-posyb);
      cairo_show_text(cr, valposyb);
   
    }
  
  //grafico da mola
  ym[c]=posym;
  
  if((grafp2==1)&&(r==1))
    {
      cairo_set_source_rgb (cr, 0, 0, 1.);
      cairo_move_to (cr, 590, ym[0]*0.7); 
      for(i=0;i<c-2;i++)
	{
	  cairo_line_to (cr, 590+i, ym[i+1]*0.7-5);
	}
      cairo_stroke(cr);
      cairo_set_source_rgb (cr, 0, 0, 0);
      cairo_set_line_width (cr, 1.5);
      cairo_rectangle (cr, 590+c-3, ym[c-1]*0.7-5, 15, 4);
      cairo_stroke_preserve (cr);
      cairo_set_source_rgb (cr, 0, 0, 1);
      cairo_fill (cr);
      cairo_stroke(cr);

      cairo_set_source_rgb(cr, 0, 0, 1);
      cairo_select_font_face (cr, "Times New Roman", 			 
			      CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
      cairo_set_font_size (cr, 20);
      cairo_move_to (cr, 800, 500);
      sprintf(valposym, "Posicao mola = %3.1lf ", 600-posym);
      cairo_show_text(cr, valposym);
    }

  
  //grafico da velocidade da bola
  vbola[c]=v;

  if((grafv1==1)&&(r==2)&&(grafp1==1))
    {
      cairo_set_source_rgb (cr, 1, 0, 1.);
      cairo_move_to (cr, 590, vbola[0]+250); 
      for(i=0;i<c-2;i++)
	{
	  cairo_line_to (cr, 590+i, vbola[i+1]+246);
	}
      cairo_stroke(cr);
      cairo_set_source_rgb(cr, 1, 0, 1);
      cairo_select_font_face (cr, "Times New Roman", 			 
			      CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
      cairo_set_font_size (cr, 20);
      cairo_move_to (cr, 770, 470);
      sprintf(valv, "Velocidade bola = %3.1lf ", v);
      cairo_show_text(cr, valv);
    }

  //grafico da velocidade da mola
  vmola[c]=vm;

  if((grafv2==1)&&(r==2)&&(grafp2==1))
    {
      cairo_set_source_rgb (cr, 0, 0, 1);
      cairo_move_to (cr, 590, vmola[0]+250); 
      for(i=0;i<c-2;i++)
	{
	  cairo_line_to (cr, 590+i, vmola[i+1]+240);
	}
      cairo_stroke(cr);
      cairo_set_source_rgb(cr, 0, 0, 1);
      cairo_select_font_face (cr, "Times New Roman", 			 
			      CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
      cairo_set_font_size (cr, 20);
      cairo_move_to (cr, 770, 500);
      sprintf(valvm, "Velocidade mola = %3.1lf ", vm);
      cairo_show_text(cr, valvm);
      
      
    }

  //Mostrar Tempo      
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_select_font_face (cr, "Times New Roman", 			 
			  CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 20);
  cairo_move_to (cr, 800, 30);
  sprintf(tempo, "t = %3.2lf unidades", t);
  cairo_show_text(cr, tempo);

  cairo_stroke(cr);
  cairo_destroy(cr);
  
  return FALSE;
}

static gboolean
time_handler (GtkWidget *widget)
{
  if (widget->window == NULL) 
    return FALSE;

  gtk_widget_queue_draw(widget);

  return TRUE;
}      

int main(int argc, char *argv[])
{

  GtkWidget  *window, *IST, *frame1, *frame2;
  GtkWidget  *hbox, *hbox1, *hbox11, *hbox12, *hboxmeio, *hbox31, *hbox3;
  GtkWidget  *vboxmeio, *vbox, *vbox2, *vbox3, *vbox31; 
  GtkWidget  *table, *label, *label1, *label2, *label3, *label4, *label5, *label6, *label7;
  GtkWidget  *button1, *button2, *button3, *button4, *button5, *button6, *button;
  GtkWidget  *table1, *image1; 
  GdkPixmap *bg;
  GdkPixbuf *image;
  GtkStyle  *style;
  
  gtk_init(&argc, &argv);
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);

  
  image = gdk_pixbuf_new_from_file ("b.jpg", NULL);
  gdk_pixbuf_render_pixmap_and_mask (image, &bg, NULL, 0);
  style = gtk_style_new();
  style->bg_pixmap[0] = bg;
  gtk_widget_set_style (GTK_WIDGET(window), GTK_STYLE(style));
  



  //________________________________________________________________boxs
  hbox     = gtk_hbox_new(0, 0);
  hbox11   = gtk_hbox_new(0, 0);
  hbox12   = gtk_hbox_new(0, 0);
  hbox1    = gtk_hbox_new(0, 0);
  hboxmeio = gtk_hbox_new(0, 0);
  hbox31   = gtk_hbox_new(0, 0);
  hbox3    = gtk_hbox_new(0, 0);

  vboxmeio = gtk_hbox_new(0, 0);
  vbox     = gtk_vbox_new(0, 0);
  vbox2    = gtk_vbox_new(0, 0);
  vbox3    = gtk_vbox_new(0, 0);
  vbox     = gtk_vbox_new(0, 0);
  vbox31   = gtk_vbox_new(0, 0);


  gtk_widget_set_size_request(hbox, 350, 600);

  //_______________________________________________________________frame
  gtk_widget_set_size_request(vbox, 50, 400);
  frame1 = gtk_frame_new("Parametros");
  gtk_widget_set_size_request(frame1, 350, 600);
  frame2 = gtk_frame_new("Graficos");
  gtk_widget_set_size_request(frame2, 400, 100);
  table = gtk_table_new(6, 3, 0);


  gtk_container_add(GTK_CONTAINER(frame1), vbox);
  gtk_container_add(GTK_CONTAINER(frame2), vbox31);
  
  //_____________________________________________________________________scale
  gtk_widget_set_size_request(hboxmeio, 92.2, 600); 
  adj = (GtkAdjustment *) gtk_adjustment_new (400, 0, 600, 1, 1, 1);
  scale = gtk_vscale_new (GTK_ADJUSTMENT (adj));
  g_signal_connect (adj, "value-changed", G_CALLBACK (change_scale), NULL);
  gtk_scale_set_value_pos(GTK_SCALE(scale),GTK_POS_RIGHT);
  gtk_scale_set_digits (GTK_SCALE (scale), 1);
  gtk_range_set_inverted(GTK_RANGE(scale), TRUE);
 
  gtk_box_pack_start(GTK_BOX(vboxmeio), scale, 1, 1, 5); 
  gtk_box_pack_start(GTK_BOX(vboxmeio), hboxmeio, 1, 1, 5);

  
  //___________________________________________________________________________IST
  IST = gtk_image_new_from_file("IST1.png");
  gtk_container_add(GTK_CONTAINER(hbox11), IST);
  label = gtk_label_new("Oscilador de Fermi");
  gtk_container_add(GTK_CONTAINER(hbox12), label);

  //______________________________________________________________________ ABOUT
  button = gtk_button_new();
  image1 = gtk_image_new_from_stock(GTK_STOCK_INFO, GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(button),image1);
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK(about), window); 
  gtk_widget_set_tooltip_text(button, "ABOUT");
  gtk_widget_set_size_request(button, 30, 30);
  gtk_box_pack_start (GTK_BOX (hbox12), button, FALSE, FALSE, 0);

  gtk_widget_show (button);


  //__________________________________________________________________buttoes de acancar no tempo

  GtkWidget *fast, *slow, *play;
  GtkWidget *event1, *event2, *event3;
     
  fast = gtk_image_new_from_file ("fast.png");
     
  event1 = gtk_event_box_new ();
     
  gtk_container_add (GTK_CONTAINER (event1), fast);
  gtk_widget_set_tooltip_text(fast, "Fast");

  g_signal_connect (G_OBJECT (event1),
		    "button_press_event",
		    G_CALLBACK (fast1),
		    fast);
	 
  slow = gtk_image_new_from_file ("slow.png");
  gtk_widget_set_tooltip_text(slow, "Slow");

  event2 = gtk_event_box_new ();
     
  gtk_container_add (GTK_CONTAINER (event2), slow);
       
  g_signal_connect (G_OBJECT (event2),
		    "button_press_event",
		    G_CALLBACK (slow1),
		    slow);
     
  play = gtk_image_new_from_file ("play.png");
  gtk_widget_set_tooltip_text(play, "Play");

  event3 = gtk_event_box_new ();
     
  gtk_container_add (GTK_CONTAINER (event3), play);
       
  g_signal_connect (G_OBJECT (event3),
		    "button_press_event",
		    G_CALLBACK (play1),
		    play);
     
  gtk_box_pack_start(GTK_BOX(hbox3), event2, 1, 0, 5);
  gtk_box_pack_start(GTK_BOX(hbox3), event3, 1, 0, 5);
  gtk_box_pack_start(GTK_BOX(hbox3), event1, 1, 0, 5);
  
 
  //______________________________________________________________________labels
  label1 = gtk_label_new("                      Constante elastica (k)");
  label2 = gtk_label_new("                      Massa do Bola (m)");
  label3 = gtk_label_new("                      Massa da Mola (M)");
  label4 = gtk_label_new("                      V inicial da Bola (v)");
  label5 = gtk_label_new("                      V inicial da Mola (vm)");

 
  gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 4, 5, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 5, 6, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label3, 0, 1, 6, 7, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label4, 0, 1, 8, 9, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label5, 0, 1, 9, 10, GTK_FILL, GTK_FILL, 5, 5);

  //________________________________________________________________spin_buttons
  adj1 = (GtkAdjustment *) gtk_adjustment_new (20.0, 1.0, 10000.0, 1, 5.0, 0.0);
  spin1 = gtk_spin_button_new (adj1, 0, 0);
  adj2 = (GtkAdjustment *) gtk_adjustment_new (2.0, 1.0, 100.0, 1, 5.0, 0.0);
  spin2 = gtk_spin_button_new (adj2, 0, 0);
  adj3 = (GtkAdjustment *) gtk_adjustment_new (10.0, 1.0, 100.0, 1, 5.0, 0.0);
  spin3 = gtk_spin_button_new (adj3, 0, 0);
  adj4 = (GtkAdjustment *) gtk_adjustment_new (10.0, 0.0, 100.0, 1, 5.0, 0.0);
  spin4 = gtk_spin_button_new (adj4, 0, 0);
  adj5 = (GtkAdjustment *) gtk_adjustment_new (10.0, 0.0, 100.0, 1, 5.0, 0.0);
  spin5 = gtk_spin_button_new (adj5, 0, 0);
   
  gtk_table_attach(GTK_TABLE(table), spin1, 1, 2, 4, 5, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), spin2, 1, 2, 5, 6, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), spin3, 1, 2, 6, 7, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), spin4, 1, 2, 8, 9, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), spin5, 1, 2, 9, 10, GTK_FILL, GTK_FILL, 5, 5);
  
  //_______________________________________________________________________buttons
  button1 = gtk_button_new_with_label("APLICAR PARAMETROS");
  gtk_container_add(GTK_CONTAINER(vbox3), button1);
  g_signal_connect(button1,"clicked",G_CALLBACK(aplicar), NULL);

  button2 = gtk_button_new_with_label("START");
  gtk_container_add(GTK_CONTAINER(vbox3), button2);
  g_signal_connect (button2, "clicked", G_CALLBACK(start_stop),(gpointer)NULL);
 
  button3 = gtk_button_new_with_label("RESET POSITIONS");
  gtk_container_add(GTK_CONTAINER(vbox3), button3);
  g_signal_connect (button3, "clicked", G_CALLBACK(reset_positions), NULL);
  
  button6 = gtk_button_new_with_label("RESET GRAPH");
  gtk_container_add(GTK_CONTAINER(vbox3), button6);
  g_signal_connect (button6, "clicked", G_CALLBACK (reset_graph), window);

  button4 = gtk_button_new_with_label("RESET ALL");
  gtk_container_add(GTK_CONTAINER(vbox3), button4);
  g_signal_connect (button4, "clicked", G_CALLBACK(reset), NULL);
  
  button5 = gtk_button_new_with_label("SAIR");
  gtk_container_add(GTK_CONTAINER(vbox3), button5);
  g_signal_connect (button5, "clicked", G_CALLBACK (quick_message), window);

 
  //_____________________________________________________________________zona dos graficos
  table1 = gtk_table_new(4, 2, 0);

  label6 = gtk_label_new ("Variáveis:");
  gtk_table_attach(GTK_TABLE(table1), label6, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 5, 5);
  label7 = gtk_label_new ("Gráfico:");
  gtk_table_attach(GTK_TABLE(table1), label7, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 5, 5);
  
  grafico1 = gtk_radio_button_new_with_label (NULL, "Posição-Tempo");
  grafico2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (grafico1), "Velocidade-Tempo");
  g_signal_connect(GTK_RADIO_BUTTON(grafico1), "clicked", G_CALLBACK(posicaotempo),NULL);
  g_signal_connect(GTK_RADIO_BUTTON(grafico2), "clicked", G_CALLBACK(velocidadetempo),NULL);
 
  gtk_table_attach(GTK_TABLE(table1), grafico1, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table1), grafico2, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 5, 5);

  check1 = gtk_check_button_new_with_label("Bola");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check1), TRUE); 
  g_signal_connect(GTK_TOGGLE_BUTTON(check1), "clicked", G_CALLBACK(checkgraf1),NULL);
  check2 = gtk_check_button_new_with_label("Mola");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check2), TRUE);
  g_signal_connect(GTK_TOGGLE_BUTTON(check2), "clicked", G_CALLBACK(checkgraf2),NULL);
  gtk_table_attach(GTK_TABLE(table1), check1, 3, 4, 0, 1, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table1), check2, 3, 4, 1, 2, GTK_FILL, GTK_FILL, 5, 5);



  gtk_box_pack_end(GTK_BOX(vbox31),   hbox31, 0, 0, 5); 
  gtk_box_pack_start(GTK_BOX(hbox31), table1, 0, 0, 5); 

  gtk_container_add(GTK_CONTAINER(vbox2), table);

  gtk_box_pack_start(GTK_BOX(hbox1), hbox11, 1, 1, 5); 
  gtk_box_pack_start(GTK_BOX(hbox1), hbox12, 1, 1, 5);

  gtk_box_pack_start(GTK_BOX(vbox), hbox1, 1, 0, 5);
  gtk_box_pack_start(GTK_BOX(vbox), vbox2, 1, 0, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, 1, 0, 5);  
  gtk_box_pack_start(GTK_BOX(vbox), vbox3, 1, 0, 5);
  

  gtk_box_pack_start(GTK_BOX(hbox), frame1, 1, 1, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vboxmeio, 1, 1, 5);
  gtk_box_pack_start(GTK_BOX(hbox), frame2, 1, 1, 5);
 
  g_signal_connect (window, "expose-event", G_CALLBACK(on_expose_event), NULL);
  g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW(window), "Oscilador de Fermi");
  gtk_widget_set_size_request (window, 1000, 600);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_widget_set_app_paintable (window, TRUE);
 
  g_timeout_add (10, (GSourceFunc) time_handler, (gpointer) window);

  gtk_container_add(GTK_CONTAINER(window), hbox);

  gtk_widget_show_all(window);
 
  gtk_main();
 
  return 0;
}
