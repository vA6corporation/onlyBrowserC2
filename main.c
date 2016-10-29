#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <string.h>

GtkBuilder *builder;
GtkWidget *main_window;
GtkWidget *password_dialog;
GtkWidget *password_entry;
GtkWidget *notebook;
GtkWidget *webkit_view;
GtkWidget *new_viewport;

WebKitWebView *webView;
WebKitWebView *webView_tmp;

WebKitNavigationPolicyDecision *navigation_decision;
WebKitResponsePolicyDecision *response;
WebKitNavigationAction *navigation_action;
WebKitURIRequest *uri_request;

void
create_note()
{
    new_viewport = gtk_viewport_new (NULL, NULL);
    webView_tmp = WEBKIT_WEB_VIEW(webkit_web_view_new_with_related_view (webView));
    gtk_container_add (GTK_CONTAINER (new_viewport),
                                GTK_WIDGET (webView_tmp));
    gtk_widget_show_all(new_viewport);
    gtk_notebook_append_page (GTK_NOTEBOOK(notebook),
                                        new_viewport,
                                        NULL);
    gtk_notebook_reorder_child (GTK_NOTEBOOK(notebook),
                                new_viewport,
                                    -1);
    gtk_notebook_set_show_tabs (GTK_NOTEBOOK(notebook),TRUE);
    //gtk_notebook_next_page (GTK_NOTEBOOK(notebook));
}

static gboolean
decide_policy_cb (WebKitWebView *webView,
                  WebKitPolicyDecision *decision,
                  WebKitPolicyDecisionType type)
{

    switch (type)
    {
        case WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION:
            navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
            /* Make a policy decision here. */
            g_print("Abriendo link\n");
            break;
        case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
            navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
            /* Make a policy decision here. */
            g_print("Abriendo ventana\n");
            navigation_action = webkit_navigation_policy_decision_get_navigation_action
                                                                    (navigation_decision);
            uri_request = webkit_navigation_action_get_request
                                            (navigation_action);
            const gchar *uri = webkit_uri_request_get_uri (uri_request);
            g_print("%s\n", uri);
            create_note();
            webkit_web_view_load_uri (webView_tmp, "https://www.google.com.pe/?gfe_rd=cr&ei=3N4UWLrkGYS_-wXptazwBA&gws_rd=ssl");
            break;
        case WEBKIT_POLICY_DECISION_TYPE_RESPONSE:
            response = WEBKIT_RESPONSE_POLICY_DECISION (decision);
            /* Make a policy decision here. */
            g_print("No estamos seguros\n");
            break;
        default:
            /* Making no decision results in webkit_policy_decision_use(). */
            return FALSE;
    }
    return TRUE;
}

// handler button signals
void
on_aceptar_button_clicked (GtkButton *button,
                            gpointer user_data)
{
    g_print("hola");
    gchar pass[] = "123";
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(password_entry));
    //strcmp es el metodo para comparar 2 arreglos de caracteres
    int check = !strcmp(entry_text, pass);

    if (check == 1)
    {
        gtk_main_quit ();
    }
}

void
on_close_button_clicked (GtkWidget *widget,
                            gpointer data)
{
    gtk_window_close (GTK_WINDOW(main_window));
}

void
on_cancelar_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
    gtk_widget_hide(password_dialog);
}

void
on_home_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
    webkit_web_view_load_uri (webView, "http://www.munisatipo.gob.pe/index.php/galerias");
  //system("thunar");
}

void
on_refresh_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
    webkit_web_view_reload (webView);
}

void
on_back_button_clicked (GtkButton *button,
                            gpointer   user_data)
{
    webkit_web_view_go_back (webView);
}
// handler signals-------------------------------------------
gint
on_window_delete_event( GtkWidget *widget,
                   GdkEvent  *event,
                   gpointer   data )
{
        g_print ("delete event occurred\n");
        gtk_widget_show_all (GTK_WIDGET(password_dialog));
        return TRUE;
}

gint
on_password_dialog_delete_event(GtkWidget *widget,
                   GdkEvent  *event,
                   gpointer   user_data)
{
    gtk_widget_hide(widget);
}
//-----------------------------------------------------------
int
main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "gui.glade", NULL);

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    password_dialog = GTK_WIDGET(gtk_builder_get_object (builder, "password_dialog"));
    webkit_view = GTK_WIDGET(gtk_builder_get_object (builder, "viewport"));
    webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    password_entry = GTK_WIDGET(gtk_builder_get_object (builder, "password_entry"));
    notebook = GTK_WIDGET(gtk_builder_get_object (builder, "notebook"));

    gtk_container_add (GTK_CONTAINER (webkit_view), GTK_WIDGET (webView));

    gtk_builder_connect_signals (builder,NULL);
    webkit_web_view_load_uri (webView, "http://google.com/");
    gtk_window_set_default_size (GTK_WINDOW(main_window),800,800);
    gtk_widget_show(GTK_WIDGET(webView));
    g_signal_connect (webView, "decide-policy", G_CALLBACK (decide_policy_cb), NULL);

    gtk_main ();

    return 0;
}
