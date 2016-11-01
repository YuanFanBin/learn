/*
 * Copyright (C) YuanFanBin
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/* module structure */
typedef struct {
    ngx_open_file_t     *file;
    time_t               disk_full_time;
    time_t               error_log_time;
} ngx_http_log_t;

typedef struct {
    ngx_array_t     *logs;  /* array of ngx_http_log_t */
    ngx_unit_t       off;    /* unsgined off:1 */
} ngx_http_ylog_loc_conf_t;

typedef struct {
    u_char          *start;
    u_char          *pos;
    u_char          *last;

    ngx_event_t     *event;
    ngx_msec_t       flush;
} ngx_http_ylog_buf_t;

/* 函数声明 */
static char *ngx_http_ylog(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_ylog_init(ngx_conf_t *cf);
static ngx_int_ylog_handler(ngx_http_request_t *r);

/* commands */
static ngx_command_t ngx_http_ylog_commands[] = {
    { ngx_string("ylog"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
        ngx_http_ylog,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_ylog_loc_conf_t, logs),
        NULL },

    ngx_null_command
};

/* http context of the module */
static ngx_http_module_t ngx_http_ylog_module_ctx = {
    NULL,                           /* preconfiguration */
    ngx_http_ylog_init,             /* postconfiguration */

    ngx_http_ylog_create_main_conf, /* create main configuration */
    NULL,                           /* init main configuration */

    NULL,                           /* create server configuration */
    NULL,                           /* create location configuration */

    ngx_http_ylog_create_loc_conf,  /* create location configuration */
    ngx_http_ylog_merge_loc_conf,   /* merge_location configuration */
};

/* module */
ngx_module_t ngx_http_ylog_module = {
    NGX_MODULE_V1,
    &ngx_http_ylog_module_ctx, /* module context */
    ngx_http_log_commands,     /* module directives */
    NGX_HTTP_MODULE,           /* module type */
    NULL,                      /* init master */
    NULL,                      /* init module */
    NULL,                      /* init process */
    NULL,                      /* init pthread */
    NULL,                      /* exit thread */
    NULL,                      /* exit process */
    NULL,                      /* exit master */
    NGX_MODULE_V1_PADDING
};

/* 读取命令[前], preconfiguration */

/* 读取命令[时]，直接打开文件 */
static char *
ngx_http_ylog(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_ylog_t          *log;
    ngx_http_log_main_conf_t *ymcf;
    ngx_http_ylog_loc_conf_t *ylcf = conf;

    ngx_str_t       *value;

    value = cf->args->elts;

    /* ylog off; */
    if (ngx_strncmp(value[1].data, "off", 3) == 0) {
        ylcf->off = 1;
        if (cf->args->nelts == 2) {
            return NGX_CONF_OK;
        }
        ngx_conf_log_err(NGX_LOG_EMERG, cf, 0, 
                         "invalid parameter \"%V\"", &value[2]);
        return NGX_CONF_ERROR;
    }

    /* ylog file; */
    if (ylcf->logs == NULL) {
        ylcf->logs = ngx_array_create(cf->pool, 2, sizeof(ngx_http_ylog_t));
        if (ylcf->logs == NULL) {
            return NGX_CONF_ERROR;
        }
    }

    ymcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_ylog_module);

    log = ngx_array_push(ylcf->logs);
    if (log == NULL) {
        return NGX_CONF_ERROR;
    }

    ngx_memzero(log, sizeof(ngx_http_ylog_t));

    /* ylog file; */
    log->file = ngx_conf_open_file(cf->cycle, &value[1]);
    if (ylcf->file == NULL) {
        return NGX_CONF_ERROR;
    }
    return NGX_CONF_OK;
}

/* 读取命令[后], postconfiguration */
static ngx_int_t
ngx_http_ylog_init(ngx_conf_t *cf)
{
    ngx_http_core_main_conf_t   *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_LOG_PHASE].handlers);
    if (h == NULL) {
        return ngx_ERROR;
    }
    *h = ngx_http_ylog_handler;
    return NGX_OK;
}

static ngx_int_t
ngx_http_ylog_handler(ngx_http_request_t *r)
{
    size_t                       len;
    ngx_uint_t                   l, i;
    ngx_http_ylog_buf_t         *buffer;
    ngx_http_ylog_loc_conf_t    *ylcf;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, 
                   "http log handler");

    ylcf = ngx_http_get_module_loc_conf(r, ngx_http_log_module);
    if (ylcf->off) {
        return NGX_OK;
    }
    log = ylcf->logs->elts;
    for (l = 0; l < ylcf->logs->nelts; ++l) {
        if (ngx_time() == log[l].disk_full_time) {
            continue;
        }
        len = 1; //NGX_LINEFEED_SIZE;
        buffer = log[l].file ? log[l].file->data : NULL;
        if (buffer) {
            if (len > (size_t) (buffer->last - buffer->pos)) {
                ngx_http_ylog_write(r, &log[l], buffer->start,
                                   buffer->pos - buffer->start);
                buffer->pos = buffer->start;
            }
            ngx_linefeed(p);
        }
    }
}

static void
ngx_http_ylog_write(ngx_http_request_t *r, ngx_http_log_t *log, u_char *buf,
    size_t len)
{
}
