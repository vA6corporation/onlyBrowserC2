#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <string.h>

GtkBuilder *builder;
GtkWidget *main_window;
GtkWidget *password_dialog;
GtkWidget *password_entry;
GtkWidget *webkit_view;
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
// handler button signals
void
on_aceptar_button_clicked (GtkButton *button,
                            gpointer user_data)
{
    g_print("hola");
    gchar pass[] = "123";
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(password_entry));
    //strcmp es el metodo para comparar 2 arrelgo de caracteres
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

    gtk_container_add (GTK_CONTAINER (webkit_view), GTK_WIDGET (webView));

    gtk_builder_connect_signals (builder,NULL);
    webkit_web_view_load_uri (webView, "http://google.com/");
    gtk_window_set_default_size (GTK_WINDOW(main_window),800,800);
    gtk_widget_show(GTK_WIDGET(webView));

    gtk_main ();

    return 0;
}
