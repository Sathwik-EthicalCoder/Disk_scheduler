#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>



// Function to be called when the button is clicked
void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;

    // Get the text from each text entry field
    const gchar *header_text = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *targets_text = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *scheduler_text = gtk_entry_get_text(GTK_ENTRY(entries[2]));

    // Convert header and targets text to integers
    int header = atoi(header_text),scann=0;
    int target_locations[100]; // Assuming there will be at most 100 targets
    int num_targets = 0,total_seek_time=0,control=0;

    char *token = strtok((char *)targets_text, " ");
    while (token != NULL) {
        target_locations[num_targets++] = atoi(token);
        token = strtok(NULL, " ");
    }

    // Implement FCFS disk scheduling
    int sequence[100]; // Store the sequence of disk access
    if(strcmp(scheduler_text,"FCFS")==0)
    {
    total_seek_time = 0;
    sequence[0] = header;
    for (int i = 0; i < num_targets; i++) {
        sequence[i+1] = target_locations[i];
        total_seek_time += abs(sequence[i] - sequence[i + 1]);
    }
    }
    
    
// SSTF disk scheduling using the same variables

if (strcmp(scheduler_text, "SSTF") == 0)
{
			
int i,n=num_targets,k,mov=0,cp=header,index[100],min,j=0,mini,cp1;
cp1=cp;
for(k=0;k<n;k++)
{
for(i=0;i<n;i++)
{
    index[i]=abs(cp-target_locations[i]); // calculate distance of each request from current position
}
// to find the nearest request
min=index[0];
mini=0;
for(i=1;i<n;i++)
{
    if(min>index[i])
    {
        min=index[i];
        mini=i;
    }
}
sequence[j]=target_locations[mini];
j++;
cp=target_locations[mini]; // change the current position value to next request
target_locations[mini]=999;
} // the request that is processed its value is changed so that it is not processed again
mov=mov+abs(cp1-sequence[0]);    // head movement
for(i=1;i<n;i++)
{
    mov=mov+abs(sequence[i]-sequence[i-1]); ///head movement
}
control=1;
total_seek_time =mov;

}


else if (strcmp(scheduler_text, "SCAN") == 0)
{
		
    				 int n=num_targets,head=header,i,j,k,seek=0,max=100,diff,temp,queue1[100],queue2[100],temp1=0,temp2=0;
            		float avg;
            		for(i=1;i<=n;i++)
            		{
            					temp=target_locations[i-1];
                        		if(temp>=head)
                        		{
                                    		queue1[temp1]=temp;
                                    		temp1++;
                        		}
                        		else
                        		{
                                    		queue2[temp2]=temp;
                                    		temp2++;
                        		}
            		}
            		for(i=0;i<temp1-1;i++)
            		{
                        		for(j=i+1;j<temp1;j++)
                        		{
                                    		if(queue1[i]>queue1[j])
                                    		{
                                                		temp=queue1[i];
                                                		queue1[i]=queue1[j];
                                                		queue1[j]=temp;
                                    		}
                        		}
            		}
            		for(i=0;i<temp2-1;i++)
            		{
                        		for(j=i+1;j<temp2;j++)
                        		{
                                    		if(queue2[i]>queue2[j])
                                    		{
                                                		temp=queue2[i];
                                                		queue2[i]=queue2[j];
                                                		queue2[j]=temp;
                                    		}
                        		}
            		}
            		for(i=1,j=0;j<temp1;i++,j++)
            		sequence[i]=queue1[j];
            		sequence[i]=max;
            		sequence[i+1]=0;
            		for(i=temp1+3,j=0;j<temp2;i++,j++)
            		sequence[i]=queue2[j];
            		sequence[0]=head;
            		for(j=0;j<=n+1;j++)
            		{
                        		diff=abs(sequence[j+1]-sequence[j]);
                        		seek+=diff;
            		}
            		total_seek_time=seek;
            		scann=2;
}

    
    
    
    
    
    
    
    

    // Create a message dialog to show the output
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Disk Scheduling Output",
                                                    GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                    GTK_DIALOG_MODAL,
                                                    GTK_STOCK_OK,
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    // Create a string to store the message
    gchar *message = g_strdup_printf("Disk Scheduled Sequence: ");
    for (int i = 0; i <= num_targets-control+scann; i++) {
        gchar temp[10];
        sprintf(temp, "%d ", sequence[i]);
        message = g_strconcat(message, temp, NULL);
    }
    gchar temp[100];
    sprintf(temp, "\nTotal Seek Time: %d", total_seek_time);
    message = g_strconcat(message, temp, NULL);

    // Add a label with the message to the dialog
    GtkWidget *label = gtk_label_new(message);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);
    // Show all the widgets
    gtk_widget_show_all(dialog);

    // Run the dialog
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Clean up
    gtk_widget_destroy(dialog);
    g_free(message);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Disk Scheduling Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a box to hold the widgets vertically
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create three text entry fields
    GtkWidget *entries[3];
    for (int i = 0; i < 3; ++i) {
        entries[i] = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(box), entries[i], TRUE, TRUE, 0);
    }

    // Set placeholder text for the entry fields
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Header Location");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Target Locations (separated by spaces)");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[2]), "Scheduler Type (e.g., FCFS)");

    // Create the button
    GtkWidget *button = gtk_button_new_with_label("Calculate");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entries);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // Show all the widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
