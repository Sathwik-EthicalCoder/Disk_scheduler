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
    int header = atoi(header_text);
    int target_locations[100]; // Assuming there will be at most 100 targets
    int num_targets = 0;

    char *token = strtok((char *)targets_text, " ");
    while (token != NULL) {
        target_locations[num_targets++] = atoi(token);
        token = strtok(NULL, " ");
    }

    // Implement FCFS disk scheduling
    int sequence[100]; // Store the sequence of disk access
    int total_seek_time = 0;
    sequence[0] = header;
    for (int i = 0; i < num_targets; i++) {
        sequence[i + 1] = target_locations[i];
        total_seek_time += abs(sequence[i] - sequence[i + 1]);
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
    for (int i = 0; i <= num_targets; i++) {
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
