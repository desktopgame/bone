#include <glib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
        GString *my_string =
            g_string_new("This Hello world is %d chars long\n");
        g_print(my_string->str, my_string->len);
        g_string_free(my_string, TRUE);
        return 0;
}