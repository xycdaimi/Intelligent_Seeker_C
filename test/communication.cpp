#include "log_system/LoggingSystem.h"
#include "Communication/mongoose.h"

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    // Serve local dir
    struct mg_http_serve_opts opts = {.root_dir = "."};
    if (ev == MG_EV_HTTP_MSG)
    {
        mg_http_serve_dir(c, (struct mg_http_message *)ev_data, &opts);
    }
}

int main(int argc, char const *argv[])
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    // Setup listener
    mg_http_listen(&mgr, "http://localhost:8000", fn, nullptr);

    // Event loop
    while (true)
    {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);

    return 0;
}
