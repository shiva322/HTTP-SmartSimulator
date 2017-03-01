#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/ioctl.h>
#include <net/if.h>
//#include <gtk-image-viewer.h>

int No_C = 3;//No. Of Clients//             //Default number of clients if we havn't provide any no.of_clients parameter. 
int **arry;

int No_int=7;// number of intervals//
int tfe_int=2; //time for each interval in sec//

static void error(const char *msg)
{
    perror(msg);
    exit(0);
}

gint delete_event(GtkWidget *widget,GdkEvent *event,gpointer data){
gtk_main_quit();
return FALSE;

}

static void destroy(GtkWidget *widget,gpointer window)
{
gtk_main_quit();
}


struct GG{
GtkWidget *SW;
GtkTextBuffer *BU;
};
typedef struct GG GtkWid_get;


struct BB{
GtkTextBuffer *B1;
GtkTextBuffer *B2;
GtkWidget *image;
};


typedef struct BB Gtk_buff;



struct THTH{
GtkTextBuffer *buerff;
long int tid;
};
typedef struct THTH threadst; //used to send parametrs to thread function



pthread_mutex_t mutexsum;

static void insert_response (GtkTextBuffer *buffer,gchar *text)
{
GtkTextIter iter;
gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
gtk_text_buffer_insert (buffer, &iter,(gchar *)text, -1);
}


//Thread function for each client

void *thread_func(void *klllk){ //sending thread id as parameter in the form of "threadst" structure which was declared above
threadst *stt = (threadst *)malloc(sizeof(struct THTH));
stt = (threadst *)klllk;
int thid = stt->tid;
GtkTextBuffer *res_buffer=(GtkTextBuffer *)(stt->buerff);
printf("in thread %d\n",thid);
int sockfd, portno, n;
   	struct sockaddr_in serv_addr;
	
    	char buffer[256],buffer7[256],buffer_demo[256],ip_temp[20],port_temp[6];
	FILE *fd1 = NULL;
	fd1 = fopen("test_client.txt","r");
        fgets(ip_temp,sizeof(ip_temp),fd1);
	fgets(port_temp,sizeof(port_temp),fd1);
        portno = atoi(port_temp);
	printf("server_ip---->%s\t",ip_temp);
	printf("server_port--->%d\n",portno);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
	{printf("u fool");
        error("ERROR opening socket");
    }
/*        struct ifreq ifr;
        memset(&ifr, 0, sizeof(struct ifreq));
        snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth1");
        ioctl(sockfd, SIOCGIFINDEX, &ifr);

    setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE,(void *)&ifr,(socklen_t) sizeof(ifr));
*/

//setsockopt(sockfd,SOL_SOCKET,SO_BINDTODEVICE,"eth0\0",5);
struct sockaddr_in clii;
bzero((char *) &clii, sizeof(clii));
   int  portno11 = 37677;
    clii.sin_family = AF_INET;
	int last_num=2;
	last_num+=thid;
	char ip_string[13];
   sprintf(ip_string,"12.12.12.%d",last_num);
    clii.sin_addr.s_addr = inet_addr(ip_string);
  //  clii.sin_port = htons(portno11);

//IF We Want to Bind the Clients to a particular ip-v4address we should use the below line of code 

//bind(sockfd,(struct sockaddr *)&clii,sizeof(clii));


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip_temp);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        printf("ERROR connecting");
   // printf("Please enter the message: ");
    bzero(buffer,255);
    fgets(buffer,255,fd1);
    //bcopy(st,buffer,16);
	fclose(fd1);	
	printf("\n%s",buffer);
	strcpy(buffer7,buffer);
	if(thid==1){
	strcpy(buffer7,"GET /text2.html HTTP/1.1\r\n");
	}	

	int i =0;
	int nb=0;
	int q=1,w0=0,w1=0,w2=0;
	static int count=0;
sleep(1);

while((i++)!= No_int){

	struct timeval tv1,tv2;
	gettimeofday(&tv1,NULL);
	unsigned long num = (tv1.tv_sec); 
	//printf("%lu",num);
	unsigned long num1=num;
	int n1;
//pthread_mutex_lock(&mutexsum);
//printf("%lu\n",tv1.tv_sec);
//printf("%lu\n",((tv1.tv_usec))/1000);
	while(num < (num1 + tfe_int)){
    n1 = write(sockfd,buffer7,strlen(buffer7));
    if (n1 < 0) 
         printf("ERROR writing to socket");
    bzero(buffer,255);
 
   n = read(sockfd,buffer,255);
    if (n < 0) 
         printf("ERROR reading from socket");
    	/*
	pthread_mutex_lock(&mutexsum);
   insert_response((GtkTextBuffer *)res_buffer,(char *)buffer);	
	pthread_mutex_unlock(&mutexsum);
	*/
	nb = nb+n;

	gettimeofday(&tv2,NULL);
	//num = ((tv2.tv_usec)/1000);
	num = (tv2.tv_sec);	

//printf("%lu\n",tv2.tv_sec);
//printf("%lu\n",((tv2.tv_usec))/1000);
 
	}
//if(thid==0){
arry[thid][w0++]=(nb/(1024));
//}
//else if(thid ==1 )  arry[1][w1++]=nb;
//else arry[2][w2++]=nb;
printf("No.of bytes recieved to client--->%d in the %d interval: %d\n",thid,q++,(nb/1024));
	nb = 0;	
pthread_mutex_lock(&mutexsum);
if(++count==No_C){
count=0;
printf("-------------------------------------------------------------------\n");
}
pthread_mutex_unlock(&mutexsum);
sleep(1);
}
    close(sockfd);
pthread_exit(NULL);
}


static void func_main(GtkTextBuffer *res_buffer){

pthread_t *threads;
threads = calloc(No_C,sizeof(pthread_t));
int t,u;
pthread_mutex_init(&mutexsum,NULL);
threadst* taskkids[No_C];

for(t=0;t<No_C;t++){
taskkids[t] = (threadst *)malloc(sizeof(struct THTH));
(taskkids[t])->buerff = res_buffer;
(taskkids[t])->tid = t; 
printf("tid:%lu",taskkids[t]->tid);
pthread_create(&threads[t],NULL,thread_func,(void *)taskkids[t]);

}

for(u=0;u<No_C;u++){
pthread_join(threads[u],NULL);
}

int v1,v3;

 for(v1=0;v1<No_int;v1++)
{
for(v3=0;v3<No_C;v3++)
printf("%d--",arry[v3][v1]);

printf(">%d-Interval\n",v1);
}
   
  FILE *pipe_gp = popen("gnuplot", "w");

  fputs("set terminal png size 600,380\n", pipe_gp);
  fputs("set output 'plot.png' \n", pipe_gp);
  fprintf(pipe_gp,"set xtics %d\n",tfe_int);
  fprintf(pipe_gp,"set xrange[0:%d]\n",(tfe_int)*(No_int));

if(No_C>=20){
 fputs("set ytics 450\n", pipe_gp);
fputs("set yrange[100:9000]\n", pipe_gp);
}  
else
{ 
fputs("set ytics 50\n", pipe_gp);
fputs("set yrange[100:900]\n", pipe_gp);
}

 fputs("set xlabel 'Time_interval(sec)'\n", pipe_gp);
  fputs("set ylabel 'No_of_K-Bytes'\n", pipe_gp);
  //fputs("set ytics 50\n", pipe_gp);
  //fputs("set yrange[100:900]\n", pipe_gp);
  fputs("set style data linespoints\n",pipe_gp);
//  fputs("set yrange[100:900]\n", pipe_gp);

 /*fputs("set style line 1 linecolor rgb '#8b1a0e' pointtype 7 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
   fputs("set style line 2 linecolor rgb '#09ad00' pointtype 3 pointsize 1 linetype 2 linewidth 1\n", pipe_gp);
fputs("set style line 3 linecolor rgb '#ff00ff' pointtype 5 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 4 linecolor rgb '#008080' pointtype 1 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 5 linecolor rgb '#ff0000' pointtype 2 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 6 linecolor rgb '#ffff00' pointtype 4 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 7 linecolor rgb '#191970' pointtype 1 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 8 linecolor rgb '#fffafa' pointtype 7 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 9 linecolor rgb '#800080' pointtype 3 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
fputs("set style line 10 linecolor rgb '#708090' pointtype 3 pointsize 1 linetype 1 linewidth 1 \n", pipe_gp);
*/	
  //fputs("plot for [n=1:3] '-' u 1:2 notitle with linespoints ls n\n", pipe_gp);
 // fprintf(pipe_gp,"plot for [n=1:%d] '-' u 1:2 notitle with linespoints ls n\n",No_C);


int v2=1;
if(No_C!=1){
fprintf(pipe_gp,"plot '-' u 1:2 title 'Client-%d' with linespoints,\\\n",v2-1,v2);

for(v2=2;v2<(No_C);v2++){
//v2++;
fprintf(pipe_gp,"'-' u 1:2 title 'Client-%d' with linespoints,\\\n",v2-1,v2);
}
//v2++;
fprintf(pipe_gp,"'-' u 1:2 title 'Client-%d' with linespoints\n",v2-1,v2);
}
else fprintf(pipe_gp,"plot '-' u 1:2 title 'client-0' with linespoints\n");
int cbb = No_C;
while((cbb--)){
  int r;
  fprintf(pipe_gp, "%d %d\n", 0 , 0);
  for (r=0; r<No_int;++r) {

    fprintf(pipe_gp, "%d %d\n", (r+1)*tfe_int , arry[cbb][r]);
  //  fprintf(pipe_gp, "%d %d\n", r+1 , arry[1][r]);

//      fprintf(pipe_gp, "%d %d\n", r+1 , arry[2][r]);

}
  fputs("e\n", pipe_gp);
}
  pclose(pipe_gp);

pthread_mutex_destroy(&mutexsum);
//pthread_exit(NULL);
}




static void enter_callback(GtkWidget *widget,GtkWidget *entry){
const gchar *entry_text;
entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
printf("Entry contens of IP: %s \n",entry_text);
FILE *fd = NULL;
fd = fopen("test_client.txt","w");
char et[16];
sprintf(et,"%s\n",entry_text);
fprintf(fd,"%s",et);
fclose(fd);
}
static void enter_callback1(GtkWidget *widget,GtkWidget *entry){
const gchar *entry_text;
entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
printf("Entry contens of Port: %s \n",entry_text);
int et = atoi(entry_text);
FILE *fd = NULL;
fd = fopen("test_client.txt","a");
fprintf(fd,"%d\n",et);
fclose(fd);
}
static void enter_callback2(GtkWidget *widget,GtkWidget *entry){
const gchar *entry_text;
entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
printf("Number of Clients:%s \n",entry_text);
int et = atoi(entry_text);
No_C = et;
}

static void enter_callback3(GtkWidget *widget,GtkWidget *entry){
const gchar *entry_text;
entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
printf("Number of Intervals:%s \n",entry_text);
int et = atoi(entry_text);
No_int = et;
}

static void enter_callback4(GtkWidget *widget,GtkWidget *entry){
const gchar *entry_text;
entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
printf("Time for each interval:%s sec\n",entry_text);
int et = atoi(entry_text);
tfe_int = et;
}


static void get_text(GtkTextBuffer *buffer){
GtkTextIter start,end;
//gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
gtk_text_buffer_get_bounds(buffer,&start,&end);
gchar *st = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
printf("%s",st);
}
static void image_func(Gtk_buff *buffer1){
	GtkWidget* image = buffer1->image;
	gtk_image_set_from_file(GTK_IMAGE(image),"process.png");
}

static void clicked(GtkWidget *widget,Gtk_buff *buffer1)
{	
	image_func(buffer1);
	GtkWidget* image = buffer1->image;
	//gtk_image_set_from_file(GTK_IMAGE(image),"plot.png");
	GtkTextIter start,end;
	gtk_text_buffer_get_bounds(buffer1->B1,&start,&end);
	const gchar *st = gtk_text_buffer_get_text(buffer1->B1,&start,&end,FALSE);
	//printf("%s",st);	
	FILE *fd = NULL;
	fd = fopen("test_client.txt","a");
	fprintf(fd,"%s\n",st);
	fclose(fd);
	arry = (int**)malloc(No_C*sizeof(int*));
	int f;
	for(f=0;f<No_C;f++)
	*(arry+f)=(int *)malloc(No_int*sizeof(int*));
	func_main((GtkTextBuffer *)buffer1->B2);
	gtk_image_set_from_file(GTK_IMAGE(image),"plot.png");
	



}

static void insert_text (GtkTextBuffer *buffer)
{
GtkTextIter iter;
gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
gtk_text_buffer_insert (buffer, &iter,
"GET /text.html HTTP/1.1\n"
, -1);
}


/* Create a scrolled text area that displays a "message" */
GtkWid_get *create_text( void )
{
GtkWid_get *temp = malloc(sizeof(struct GG));
GtkWidget *scrolled_window;
GtkWidget *view;
GtkTextBuffer *buffer;
view = gtk_text_view_new();
buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
scrolled_window = gtk_scrolled_window_new (NULL, NULL);
gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
GTK_POLICY_AUTOMATIC,
GTK_POLICY_AUTOMATIC);
gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW (scrolled_window),GTK_SHADOW_ETCHED_OUT);
gtk_container_add (GTK_CONTAINER (scrolled_window), view);
insert_text (buffer);
gtk_widget_show_all(scrolled_window);
//return scrolled_window;
temp->SW = scrolled_window;
temp->BU = buffer;
return temp;
}


int main(int argc,char *argv[])
{
/*
    pthread_t hosttid;
	pthread_create(&hosttid,NULL,thread_main,NULL);
	pthread_join(hosttid,NULL);
*/
	
    GtkWidget *window,*button,*button1,*vbox,*entry,*hbox,*hbox1,*hbox2,*ip_label,*port_label,*vpaned,*label,*hseparator,*num_cli,*hbox3,*hbox4,*num_interval,*timeforinterval,*image;
    GtkWid_get *tttt;
GtkWid_get *text1;
	Gtk_buff *bufff;
	bufff = (Gtk_buff *)malloc(sizeof(bufff));
	GtkWidget *vbox1,*hseparator1;
	//g_thread_init(NULL);
	//gdk_threads_init();
	//gdk_threads_enter();

    gtk_init(&argc,&argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),600,600);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window),"Client");
g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(delete_event),NULL);
//g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy),(gpointer)window);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);	
	
	/* create a vpaned widget and add it to our toplevel window */
	vpaned = gtk_vpaned_new();
	gtk_container_add (GTK_CONTAINER(window), vpaned);
	gtk_widget_show(vpaned);
	/* Now create the contents of the two halves of the window */

	vbox = gtk_vbox_new(FALSE,0);
	//gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_paned_add1 (GTK_PANED(vpaned), vbox);
	gtk_widget_show(vbox);


	hbox1 = gtk_hbox_new(FALSE,0);
	ip_label = gtk_label_new("Server_IP_Addr:");
	gtk_box_pack_start(GTK_BOX(hbox1),ip_label,TRUE,TRUE,0);
	gtk_widget_show(ip_label);	
	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry),20);
	//gtk_entry_set_text(GTK_ENTRY(entry),"IP-address");  
 	g_signal_connect(G_OBJECT(entry),"activate",G_CALLBACK(enter_callback),(gpointer)entry);
	gtk_box_pack_start(GTK_BOX(hbox1),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);  
	
	gtk_widget_show(hbox1);	
	gtk_box_pack_start(GTK_BOX(vbox),hbox1,TRUE,TRUE,0);

	hbox2 = gtk_hbox_new(FALSE,00);
	port_label = gtk_label_new("Server_Port_No:");
	gtk_box_pack_start(GTK_BOX(hbox2),port_label,TRUE,TRUE,0);
	gtk_widget_show(port_label);
	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry),6);
	//gtk_entry_set_text(GTK_ENTRY(entry),"Port");  
 	g_signal_connect(G_OBJECT(entry),"activate",G_CALLBACK(enter_callback1),(gpointer)entry);
	gtk_box_pack_start(GTK_BOX(hbox2),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);  
	
	gtk_widget_show(hbox2);	
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,TRUE,TRUE,0);
		


hbox3 = gtk_hbox_new(FALSE,00);
	num_cli = gtk_label_new("Num_of_Clients:");
	gtk_box_pack_start(GTK_BOX(hbox3),num_cli,TRUE,TRUE,0);
	gtk_widget_show(num_cli);
	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry),2);
	//gtk_entry_set_text(GTK_ENTRY(entry),"Port");  
 	g_signal_connect(G_OBJECT(entry),"activate",G_CALLBACK(enter_callback2),(gpointer)entry);
	gtk_box_pack_start(GTK_BOX(hbox3),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);  
	
	gtk_widget_show(hbox3);	
	gtk_box_pack_start(GTK_BOX(vbox),hbox3,TRUE,TRUE,0);



hbox4 = gtk_hbox_new(TRUE,00);
	num_interval = gtk_label_new("Num_of_Intervals:");
	gtk_box_pack_start(GTK_BOX(hbox4),num_interval,FALSE,TRUE,0);
	gtk_widget_show(num_interval);
	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry),2);
	//gtk_entry_set_text(GTK_ENTRY(entry),"Port");  
 	g_signal_connect(G_OBJECT(entry),"activate",G_CALLBACK(enter_callback3),(gpointer)entry);
	gtk_box_pack_start(GTK_BOX(hbox4),entry,FALSE,TRUE,0);
	gtk_widget_show(entry);  
	

timeforinterval = gtk_label_new("Interval_time(sec):");
	gtk_box_pack_start(GTK_BOX(hbox4),timeforinterval,FALSE,TRUE,0);
	gtk_widget_show(timeforinterval);
	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry),2);
	//gtk_entry_set_text(GTK_ENTRY(entry),"Port");  
 	g_signal_connect(G_OBJECT(entry),"activate",G_CALLBACK(enter_callback4),(gpointer)entry);
	gtk_box_pack_start(GTK_BOX(hbox4),entry,FALSE,TRUE,0);
	gtk_widget_show(entry);  


	gtk_widget_show(hbox4);	
	gtk_box_pack_start(GTK_BOX(vbox),hbox4,TRUE,TRUE,0);




	hbox = gtk_hbox_new(FALSE,100);	
	
	button = gtk_button_new_with_label("Connect Server");
//	gtk_widget_set_size_request(button,30,10);
	
	gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
	gtk_widget_show(button);  

	/*button1 = gtk_button_new_with_label("Stop Server");
	gtk_widget_set_size_request(button1,100,50);
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(clicked1),"stopped u fool haha");
	gtk_box_pack_start(GTK_BOX(hbox),button1,TRUE,TRUE,0);
	gtk_widget_show(button1);   
	*/
	
	gtk_widget_show(hbox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,0);
	
	hseparator = gtk_hseparator_new();
	gtk_widget_show(hseparator);
	gtk_box_pack_start(GTK_BOX(vbox),hseparator,TRUE,TRUE,0);
		
	

	vbox1 = gtk_vbox_new(FALSE,1);
	//gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_paned_add2(GTK_PANED(vpaned),vbox1);
	gtk_widget_show(vbox1);



	label = gtk_label_new("!!Enter Request--->!!");
	gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
	gtk_label_set_pattern(GTK_LABEL(label),"_____________________");
	gtk_widget_show(label);	
	gtk_box_pack_start(GTK_BOX(vbox1),label,FALSE,FALSE,1);

	tttt = create_text();
	GtkWidget *sw = tttt->SW;
	bufff->B1 = tttt->BU;
	gtk_box_pack_start(GTK_BOX(vbox1),(GtkWidget *)sw,FALSE,FALSE,0);
	

	//GtkTextBuffer *bu = tttt->BU;
	

	/*GtkWidget *stext; 
	GtkTextBuffer *buffer1;
	stext = gtk_text_view_new();
	buffer1 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (stext));
	gtk_widget_show(stext);	
	gtk_box_pack_start(GTK_BOX(vbox1),(GtkWidget *)stext,TRUE,TRUE,0);
*/
	hseparator1 = gtk_hseparator_new();
	gtk_widget_show(hseparator1);
	gtk_box_pack_start(GTK_BOX(vbox1),hseparator1,FALSE,FALSE,2);	

	label = gtk_label_new("!! Response--->!!");
	gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
	gtk_label_set_pattern(GTK_LABEL(label),"_____________________");
	gtk_widget_show(label);	
	gtk_box_pack_start(GTK_BOX(vbox1),label,FALSE,FALSE,2);
	
	/*text1 = create_text();
	bufff->B2 = text1->BU;
	gtk_box_pack_start(GTK_BOX(vbox1),(GtkWidget *)text1->SW,TRUE,TRUE,0);
*/
	//gtk_paned_add2(GTK_PANED(vpaned), (GtkWidget *)text->SW);
	//gtk_widget_show (text);
    

	image = gtk_image_new();
        bufff->image = image;
	gtk_image_set_from_file(GTK_IMAGE(image),"welcome.png");
	//image_viewer=gtk_image_new_from_file("plot.png");
	gtk_widget_show (image);
	gtk_box_pack_start(GTK_BOX(vbox1),(GtkWidget *)image,TRUE,TRUE,0);	
	

	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(clicked),(Gtk_buff *)bufff);

	gtk_widget_show(window);
  gtk_main();
	//sleep(50); 
  

	
    return 0;
}


