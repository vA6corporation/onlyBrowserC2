#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <string.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *password_dialog;
GObject *view;
WebKitWebView *webView;

static gboolean
decide_policy_cb (WebKitWebView *webView,
                  WebKitPolicyDecision *decision,
                  WebKitPolicyDecisionType type)
{
    switch (type) {
    case WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION:
        //WebKitNavigationPolicyDecision *navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
        /* Make a policy decision here. */
        g_print("Abriendo link\n");
        break;
    case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
        //WebKitNavigationPolicyDecision *navigation_decision = WEBKIT_NAVIGATION_POLICY_DECISION (decision);
        /* Make a policy decision here. */
        g_print("Abriendo ventana\n");
        const gchar *aki = webkit_web_view_get_uri (webView);
        //const gchar *aki2 = webkit_web_view_load_html(webView, gchar *conte, NULL);
        g_print("%s\n", aki);

        break;
    case WEBKIT_POLICY_DECISION_TYPE_RESPONSE:
        //WebKitResponsePolicyDecision *response = WEBKIT_RESPONSE_POLICY_DECISION (decision);
        /* Make a policy decision here. */
        g_print("No estamos seguros\n");
        break;
    default:
        /* Making no decision results in webkit_policy_decision_use(). */
        return FALSE;
    }
    return TRUE;
}

gint
on_window_delete_event( GtkWidget *widget,
                   GdkEvent  *event,
                   gpointer   data )
{
        g_print ("delete event occurred\n");
        gtk_widget_show_all (GTK_WIDGET(password_dialog));
        return TRUE;
}


void
on_close_button_clicked (GtkButton *widget,
                            gpointer data)
{
    //gtk_main_quit ();
    gtk_window_close (GTK_WINDOW(window));
    //g_print("click\n");
}

void
on_pass_button_clicked (GtkWidget *widget,
                            gpointer data)
{
    gchar pass[] = "123";
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(data));

    int check = !strcmp(entry_text, pass);

    if (check == 1)
    {
        gtk_main_quit ();
    }
}

void
hide_window ()
{
    //gtk_widget_hide(exit_window);
}

void
go_home ()
{
    webkit_web_view_load_uri (webView, "http://google.com/");
  //system("thunar");
}

void
refresh ()
{
    webkit_web_view_reload (webView);
}

void
go_back ()
{
    webkit_web_view_go_back (webView);
}

int
main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "gui.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    password_dialog = GTK_WIDGET(gtk_builder_get_object (builder, "password_dialog"));
    view = gtk_builder_get_object (builder, "view");
    webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

    gtk_container_add (GTK_CONTAINER (view), GTK_WIDGET (webView));

    gtk_builder_connect_signals (builder,NULL);
    webkit_web_view_load_uri (webView, "http://munisatipo.gob.pe/index.php/23-ordenanza-municipal");
    gtk_window_set_default_size (GTK_WINDOW(window),800,800);
    gtk_widget_show(GTK_WIDGET(webView));

    gtk_main ();

    return 0;
}
